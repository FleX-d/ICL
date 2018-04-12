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
#include "FleXdLogger.h"
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <iostream>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <unistd.h>


namespace flexd {
    namespace ilc {
        namespace epoll {

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
                buffer() 
                {
                  
                }

                ~Epoll() = default;
                
                int efd;
                size_t maxEventh;
                std::vector<epoll_event> ev;
                std::map<int, EpollClient> buffer;
            };

            FleXdEpoll::FleXdEpoll(size_t maxEventh)
            :  m_epoll(std::make_unique<FleXdEpoll::Epoll>(maxEventh)),
               m_safeStop(false)
            {
                FLEX_LOG_INIT("FleXdEpoll");
                FLEX_LOG_TRACE("FleXdEpoll() -> Start");
            }

            FleXdEpoll::~FleXdEpoll() {
                FLEX_LOG_TRACE("FleXdEpoll() -> Destroyed");
            }

            bool FleXdEpoll::addEvent(int fd, std::function<void(Event)> onEvent) {
                FLEX_LOG_TRACE("FleXdEpoll::addEvent() -> ", fd) ;
                auto itBuffer = m_epoll->buffer.find(fd);
                if (itBuffer == m_epoll->buffer.end()) {
                    EpollClient epollClient;
                    auto ret = m_epoll->buffer.insert(std::make_pair(fd, epollClient));
                    if (!ret.second) {
                        FLEX_LOG_ERROR("FleXdEpoll::addEvent() -> Insert fd fail: ", fd);
                        return false;
                    } else {
                        FLEX_LOG_TRACE("FleXdEpoll::addEvent() -> Insert fd success: ", fd) ;
                    }
                    ret.first->second.onEvent = onEvent;
                    epoll_event& ev = ret.first->second.epollEvent;
                    ev.data.u64 = 0;
                    ev.data.fd = fd;
                    ev.events |= EPOLLIN | EPOLLPRI | EPOLLHUP;
                    //TODO add fd to epoll
                    std::ignore = epoll_ctl(m_epoll->efd, EPOLL_CTL_ADD, fd, &ev); // TODO check return
                    
                    return true;
                }
                return false;
            }

            bool FleXdEpoll::rmEvent(int fd) {
                FLEX_LOG_TRACE("FleXdEpoll::rmEvent() -> ", fd);
                auto itBuffer = m_epoll->buffer.find(fd);
                if (itBuffer != m_epoll->buffer.end()) {
                    std::ignore = epoll_ctl(m_epoll->efd, EPOLL_CTL_DEL, fd, &(itBuffer->second.epollEvent)); // TODO check return
//                    close(fd);
                    std::ignore = m_epoll->buffer.erase(itBuffer);
                    FLEX_LOG_TRACE("FleXdEpoll::rmEvent() -> Remove fd success: ", fd) ;
                    return true;
                } else {
                    FLEX_LOG_WARN("FleXdEpoll::rmEvent() -> fd not found in map!");
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
                FLEX_LOG_TRACE("FleXdEpoll::loop() -> Start") ;
                int fd = 0;
                uint32_t flags = 0;
                std::queue<int> rmEvQueue{};
                while (!m_safeStop) {
                    int nfds = epoll_wait(m_epoll->efd, m_epoll->ev.data(), m_epoll->maxEventh, -1);
                    
                    for (int i = 0; i < nfds; i++) {
                        
                        fd = m_epoll->ev.at(i).data.fd;
                        flags = m_epoll->ev.at(i).events;
                        auto itBuffer = m_epoll->buffer.find(fd);
                        if (itBuffer == m_epoll->buffer.end()) {
                            rmEvQueue.push(fd);
                            continue;
                        }
                        auto onEvent = itBuffer->second.onEvent;
                        
                        if ((flags & EPOLLERR) || (flags & EPOLLHUP)) {
                            FLEX_LOG_ERROR("FleXdEpoll::loop() -> ERROR: ", flags);
                            onEvent(Event(fd, flags, EpollEvent::Enum::EpollError));
                            rmEvQueue.push(fd);
                            continue;
                        } else if ((flags & EPOLLIN) || (flags & EPOLLRDNORM)) {
                            onEvent(Event(fd, flags, EpollEvent::Enum::EpollIn));
                            FLEX_LOG_TRACE("FleXdEpoll::loop() -> IN: ", flags);
                        } else if ((flags & EPOLLOUT) || (flags & EPOLLWRNORM)) {
                            onEvent(Event(fd, flags, EpollEvent::Enum::EpollOut));
                            FLEX_LOG_TRACE("FleXdEpoll::loop() -> OUT: ", flags);
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
                FLEX_LOG_TRACE("FleXdEpoll::loop() -> Stop") ;
            }

        } // namespace epoll
    } // namespace ilc
} // namespace flexd
