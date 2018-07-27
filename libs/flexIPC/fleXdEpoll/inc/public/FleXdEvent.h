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
#include <sys/eventfd.h>

struct itimerspec;
namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdEvent {
            public:
                explicit FleXdEvent(FleXdEpoll& poller, std::function<void(eventfd_t)> onEvent = nullptr);
                virtual ~FleXdEvent();

                /**
                 * Function initializes the event (adds event to FleXdEpoll)
                 * @return true on success
                 */
                bool init();
                /**
                 * Function uninitializes the event (removes event from FleXdEpoll)
                 * @return true on success
                 */
                bool uninit();
                /**
                 * Function triggers the event with defined value. Value must be greater than 0.
                 * @return true on success
                 */
                bool trigger(eventfd_t value = 1);
                /**
                 * Function returns file descriptor of the event
                 * @return file descriptor
                 */
                int getFd() const;
                /**
                 * Function sets the callback function for event
                 * @param onEvent - callback function
                 */
                void setOnEvent(std::function<void(eventfd_t)> onEvent);

                FleXdEvent(const FleXdEpoll&) = delete;
                FleXdEvent& operator=(const FleXdEpoll&) = delete;
                FleXdEvent(const FleXdEpoll&&) = delete;
                FleXdEvent& operator=(const FleXdEpoll&&) = delete;

                virtual void onEvent(eventfd_t) {}

            private:
                void onEvent(FleXdEpoll::Event e);

            protected:
                FleXdEpoll& m_poller;
                std::function<void(eventfd_t)> m_onEvent;
                int m_fd;
            };

            class FleXdTermEvent : public FleXdEvent {
            public:
                explicit FleXdTermEvent(FleXdEpoll& poller);
                virtual ~FleXdTermEvent();

                bool init();
                bool trigger();

                FleXdTermEvent(const FleXdEpoll&) = delete;
                FleXdTermEvent& operator=(const FleXdEpoll&) = delete;
                FleXdTermEvent(const FleXdEpoll&&) = delete;
                FleXdTermEvent& operator=(const FleXdEpoll&&) = delete;

                virtual void onEvent(eventfd_t) override;
            };

        } // namespace ipc
    } // namespace icl
} // namespace flexd

#endif /* FLEXDEVENT_H */
