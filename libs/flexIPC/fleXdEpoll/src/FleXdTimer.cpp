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
 * File:   FleXdTimer.cpp
 * Author: Martin Strenger
 *
 * Created on June 07, 2018, 14:49 PM
 */

#include "FleXdTimer.h"
#include <sys/timerfd.h>
#include <unistd.h>

#define NSEC_MAX 999999999

namespace flexd {
    namespace icl {
        namespace ipc {

            FleXdTimer::FleXdTimer(FleXdEpoll& poller, time_t sec, long nsec /*= 0*/, bool periodic /*= true*/, std::function<void()> onTimer /*= nullptr*/)
            : m_poller(poller),
              m_timer({ { periodic ? (sec < 0 ? 0 : sec) : 0,
                          periodic ? (nsec > NSEC_MAX ? NSEC_MAX : (nsec < 0 ? 0 : nsec)) : 0 },
                        { sec < 0 ? 0 : sec,
                          nsec > NSEC_MAX ? NSEC_MAX : nsec < 0 ? 0 : nsec } }),
              m_onTimer(onTimer),
              m_fd(-1) {
            }

            FleXdTimer::~FleXdTimer() {
                if (m_fd != -1) {
                    m_poller.rmEvent(m_fd);
                    ::close(m_fd);
                }
            }

            bool FleXdTimer::start() {
                if (m_fd == -1) {
                    m_fd = ::timerfd_create (CLOCK_MONOTONIC, TFD_NONBLOCK);
                    if (m_fd > -1) {
                        if (::timerfd_settime(m_fd, 0, &m_timer, nullptr) < 0) {
                            ::close(m_fd);
                            return false;
                        }
                        return m_poller.addEvent(m_fd, [this](FleXdEpoll::Event e) { onTimer(e); });
                    }
                }
                return false;
            }

            bool FleXdTimer::stop() {
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

            int FleXdTimer::getFd() const {
                return m_fd;
            }

            void FleXdTimer::setOnTimer(std::function<void()> onTimer) {
                m_onTimer = onTimer;
            }

            void FleXdTimer::onTimer(FleXdEpoll::Event e) {
                if (e.type == EpollEvent::EpollIn && e.fd == m_fd) {
                    uint64_t buf = 0;
                    if (::read(m_fd, &buf, sizeof(buf)) != -1) {
                        if (m_onTimer) {
                            m_onTimer();
                        } else {
                            onTimer();
                        }
                    }
                }
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
