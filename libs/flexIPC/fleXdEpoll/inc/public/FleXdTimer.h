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
 * File:   FleXdTimer.h
 * Author: Martin Strenger
 *
 * Created on June 07, 2018, 14:55 PM
 */

#ifndef FLEXDTIMER_H
#define FLEXDTIMER_H

#include "FleXdEpoll.h"

struct itimerspec;
namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdTimer {
            public:
                explicit FleXdTimer(FleXdEpoll& poller, time_t sec, long nsec = 0, bool periodic = true, std::function<void()> onTimer = nullptr);
                virtual ~FleXdTimer();

                bool start();
                bool stop();
                int getFd() const;

                FleXdTimer(const FleXdEpoll&) = delete;
                FleXdTimer& operator=(const FleXdEpoll&) = delete;
                FleXdTimer(const FleXdEpoll&&) = delete;
                FleXdTimer& operator=(const FleXdEpoll&&) = delete;

                virtual void onTimer() {}

            private:
                void onTimer(FleXdEpoll::Event e);

            private:
                FleXdEpoll& m_poller;
                itimerspec m_timer;
                std::function<void()> m_onTimer;
                int m_fd;
            };

        } // namespace ipc
    } // namespace icl
} // namespace flexd

#endif /* FLEXDTIMER_H */
