/*
Copyright (c) 2017, Globallogic s.r.o.
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Globallogic s.r.o. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC S.R.O. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * File:   FleXdEpoll.cpp
 * Author: Adrian Peniak
 *
 * Created on January 31, 2018, 9:55 AM
 */

#include "FleXdEpoll.h"
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <unistd.h>

namespace flexd {
    namespace icl {
        namespace ipc {

            class EpollClient {
            public:
                EpollClient()
                : epollEvent(),
                  onEvent(nullptr) {
                }
                ~EpollClient() = default;

                epoll_event epollEvent;
                std::function<void(FleXdEpoll::Event)> onEvent;
            };

            class FleXdEpoll::Epoll {
            public:
                Epoll(size_t _maxEventh)
                : efd(epoll_create(_maxEventh)),
                  maxEventh(_maxEventh),
                  ev(_maxEventh),
                  buffer() {
                }
                ~Epoll() = default;

                int efd;
                size_t maxEventh;
                std::vector<epoll_event> ev;
                std::map<int, EpollClient> buffer;
            };

            FleXdEpoll::FleXdEpoll(size_t maxEventh)
            :  m_epoll(std::make_unique<FleXdEpoll::Epoll>(maxEventh)),
               m_safeStop(false) {
            }

            FleXdEpoll::~FleXdEpoll() {
            }

            bool FleXdEpoll::addEvent(int fd, std::function<void(Event)> onEvent) {
                auto itBuffer = m_epoll->buffer.find(fd);
                if (itBuffer == m_epoll->buffer.end()) {
                    auto ret = m_epoll->buffer.insert(std::make_pair(fd, EpollClient()));
                    if (!ret.second) return false;

                    ret.first->second.onEvent = onEvent;
                    epoll_event& ev = ret.first->second.epollEvent;
                    ev.data.u64 = 0;
                    ev.data.fd = fd;
                    ev.events |= EPOLLIN | EPOLLPRI | EPOLLHUP;
                    return (0 == epoll_ctl(m_epoll->efd, EPOLL_CTL_ADD, fd, &ev)); // TODO check return
                }
                return false;
            }

            bool FleXdEpoll::rmEvent(int fd) {
                auto itBuffer = m_epoll->buffer.find(fd);
                if (itBuffer != m_epoll->buffer.end()) {
                    bool ret = (0 == epoll_ctl(m_epoll->efd, EPOLL_CTL_DEL, fd, &(itBuffer->second.epollEvent))); // TODO check return
                    std::ignore = m_epoll->buffer.erase(itBuffer);
                    return ret;
                } else {
                    return false;
                }
                return false;
            }

            void FleXdEpoll::dumpFd() const {
                for (auto& it : m_epoll->buffer) {
                    std::cout << it.first << std::endl;
                }
            }

            void FleXdEpoll::loop() {
                int fd = 0;
                uint32_t flags = 0;
                std::queue<int> rmEvQueue{};
                while (!m_safeStop) {
                    int nfds = epoll_wait(m_epoll->efd, m_epoll->ev.data(), m_epoll->maxEventh, 1000);

                    for (int i = 0; i < nfds; i++) {
						const epoll_event& evItem = m_epoll->ev.at(i);

                        fd = evItem.data.fd;
                        flags = evItem.events;
                        auto itBuffer = m_epoll->buffer.find(fd);
                        if (itBuffer == m_epoll->buffer.end()) {
                            rmEvQueue.push(fd);
                            continue;
                        }
                        auto onEvent = itBuffer->second.onEvent;

                        if ((flags & EPOLLERR) || (flags & EPOLLHUP)) {
                            onEvent(Event(fd, flags, EpollEvent::Enum::EpollError));
                            rmEvQueue.push(fd);
                            continue;
                        } else if ((flags & EPOLLIN) || (flags & EPOLLRDNORM)) {
                            onEvent(Event(fd, flags, EpollEvent::Enum::EpollIn));
                        } else if ((flags & EPOLLOUT) || (flags & EPOLLWRNORM)) {
                            onEvent(Event(fd, flags, EpollEvent::Enum::EpollOut));
                        } else {
                            onEvent(Event(fd, flags, EpollEvent::Enum::UndefinedError));
                            rmEvQueue.push(fd);
                            continue;
                        }
                    }
                    while (!rmEvQueue.empty()) {
                        rmEvent(rmEvQueue.front());
                        rmEvQueue.pop();
                    }
                }
                m_safeStop.store(false);
            }

            void FleXdEpoll::endLoop(bool blocking) {
                m_safeStop.store(true);
                if (blocking) {
                    // TODO check if loop stop
                }
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
