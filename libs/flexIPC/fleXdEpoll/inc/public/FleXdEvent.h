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
 * File:   FleXdEvent.h
 * Author: Martin Strenger
 *
 * Created on June 08, 2018, 09:45 AM
 */

#ifndef FLEXDEVENT_H
#define FLEXDEVENT_H

#include "FleXdEpoll.h"

struct itimerspec;
namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdEvent {
            public:
                explicit FleXdEvent(FleXdEpoll& poller, std::function<void()> onEvent = nullptr);
                virtual ~FleXdEvent();

                bool init();
                bool trigger();
                int getFd() const;

                FleXdEvent(const FleXdEpoll&) = delete;
                FleXdEvent& operator=(const FleXdEpoll&) = delete;
                FleXdEvent(const FleXdEpoll&&) = delete;
                FleXdEvent& operator=(const FleXdEpoll&&) = delete;

                virtual void onEvent() {}

            private:
                void onEvent(FleXdEpoll::Event e);

            protected:
                FleXdEpoll& m_poller;
                std::function<void()> m_onEvent;
                int m_fd;
            };

            class FleXdTermEvent : public FleXdEvent {
            public:
                explicit FleXdTermEvent(FleXdEpoll& poller);
                virtual ~FleXdTermEvent();

                bool init();
                bool trigger() { return true; }

                FleXdTermEvent(const FleXdEpoll&) = delete;
                FleXdTermEvent& operator=(const FleXdEpoll&) = delete;
                FleXdTermEvent(const FleXdEpoll&&) = delete;
                FleXdTermEvent& operator=(const FleXdEpoll&&) = delete;

                virtual void onEvent() override;
            };

            class FleXdSignalHandler {
            public:
                ~FleXdSignalHandler() = default;

                static FleXdSignalHandler& getInstance();
                static void setEventFd(int eventFd);
                static void signalHandler(int signal);

                FleXdSignalHandler(const FleXdEpoll&) = delete;
                FleXdSignalHandler& operator=(const FleXdEpoll&) = delete;
                FleXdSignalHandler(const FleXdEpoll&&) = delete;
                FleXdSignalHandler& operator=(const FleXdEpoll&&) = delete;

            private:
                FleXdSignalHandler();
                static std::atomic<int> m_eventFd;
            };

        } // namespace ipc
    } // namespace icl
} // namespace flexd

#endif /* FLEXDEVENT_H */
