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
 * File:   FleXdEpoll.h
 * Author: Adrian Peniak
 *
 * Created on January 31, 2018, 9:55 AM
 */

#ifndef FLEXDEPOLL_H
#define FLEXDEPOLL_H

#include "FleXdIPCTypes.h"
#include <memory>
#include <atomic>
#include <functional>

namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdEpoll {
            public:
                struct Event {
                    Event(int _fd, int _flag, EpollEvent::Enum _type = EpollEvent::Enum::None)
                    : fd(_fd), flag(_flag), type(_type) {}
                    int fd;
                    int flag;
                    EpollEvent::Enum type;
                };

            public:
                explicit FleXdEpoll(size_t maxEventh);
                virtual ~FleXdEpoll();

                void loop();
                void endLoop(bool blocking = false);
                bool addEvent(int fd, std::function<void(Event)> onEvent);
                bool rmEvent(int fd);
                void dumpFd() const;

                FleXdEpoll(const FleXdEpoll&) = delete;
                FleXdEpoll& operator=(const FleXdEpoll&) = delete;

            private:
                class Epoll;
                std::unique_ptr<Epoll> m_epoll;
                std::atomic<bool> m_safeStop;
            };

        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDEPOLL_H */
