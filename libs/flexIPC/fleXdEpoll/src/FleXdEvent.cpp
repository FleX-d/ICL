/*
Copyright (c) 2018, Globallogic s.r.o.
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
 * File:   FleXdEvent.cpp
 * Author: Martin Strenger
 *
 * Created on June 08, 2018, 09:45 AM
 */

#include "FleXdEvent.h"
#include <unistd.h>
#include <signal.h>

namespace {
    std::atomic<int> sigEventFd;

    void signalHandler(int signal) {
        if (sigEventFd != -1) {
            ::eventfd_write(sigEventFd, signal);
        } else {
            switch (signal) {
                case SIGABRT:
                case SIGTERM:
                case SIGINT:
                    exit(EXIT_FAILURE);
                default:
                    break;
            }
        }
    }
}

namespace flexd {
    namespace icl {
        namespace ipc {

            FleXdEvent::FleXdEvent(FleXdEpoll& poller, std::function<void(eventfd_t)> onEvent /*= nullptr*/)
            : m_poller(poller),
              m_onEvent(onEvent),
              m_fd(-1) {
            }

            FleXdEvent::~FleXdEvent() {
                if (m_fd != -1) {
                    m_poller.rmEvent(m_fd);
                    ::close(m_fd);
                }
            }

            bool FleXdEvent::init() {
                if (m_fd == -1) {
                    m_fd = ::eventfd(0, EFD_NONBLOCK);
                    if (m_fd > -1) {
                        return m_poller.addEvent(m_fd, [this](FleXdEpoll::Event e) { onEvent(e); });
                    }
                }
                return false;
            }

            bool FleXdEvent::uninit() {
                if (m_fd == -1) {
                    return true;
                }
                if (m_poller.rmEvent(m_fd)) {
                    ::close(m_fd);
                    m_fd = -1;
                    return true;
                };
                return false;
            }

            bool FleXdEvent::trigger(eventfd_t value /*= 1*/) {
                if (m_fd != -1) {
                    if (::eventfd_write(m_fd, value) == 0) {
                        return true;
                    }
                }
                return false;
            }

            int FleXdEvent::getFd() const {
                return m_fd;
            }

            void FleXdEvent::setOnEvent(std::function<void(eventfd_t)> onEvent) {
                m_onEvent = onEvent;
            }

            void FleXdEvent::onEvent(FleXdEpoll::Event e) {
                if (e.type == EpollEvent::EpollIn && e.fd == m_fd) {
                    eventfd_t value;
                    if (::eventfd_read(m_fd, &value) == 0) {
                        if (m_onEvent) {
                            m_onEvent(value);
                        } else {
                            onEvent(value);
                        }
                    }
                }
            }

            FleXdTermEvent::FleXdTermEvent(FleXdEpoll& poller)
            : FleXdEvent(poller) {
                sigEventFd.store(-1);
                signal(SIGABRT, signalHandler);
                signal(SIGTERM, signalHandler);
                signal(SIGINT, signalHandler);
            }

            FleXdTermEvent::~FleXdTermEvent() {
            }

            bool FleXdTermEvent::init() {
                if (FleXdEvent::init())
                {
                    sigEventFd.store(FleXdEvent::getFd());
                    return true;
                }
                return false;
            }

            bool FleXdTermEvent::trigger() {
                return ::raise(SIGTERM) == 0;
            }

            void FleXdTermEvent::onEvent(eventfd_t) {
                m_poller.endLoop();
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
