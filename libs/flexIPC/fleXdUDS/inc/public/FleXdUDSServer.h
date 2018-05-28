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
 * File:   FleXdUDSServer.h
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 *
 * Created on February 1, 2018, 2:45 PM
 */

#ifndef FLEXDUDSSERVER_H
#define FLEXDUDSSERVER_H

#include "FleXdEpoll.h"
#include "FleXdIPCBuffer.h"
#include "FleXdUDS.h"
#include <memory>
#include <string>
#include <atomic>
#include <list>
#include <map>

namespace flexd {
    namespace icl {
        namespace epoll {

            class FleXdUDSServer : public FleXdUDS {
            public:
                explicit FleXdUDSServer(const std::string& socPath, FleXdEpoll& poller);
                virtual ~FleXdUDSServer();

                virtual bool initialization() override;
                virtual void sendMsg(pSharedFleXdIPCMsg msg, int fd) override;
                virtual void onMsg(pSharedFleXdIPCMsg msg) override;

                FleXdUDSServer(const FleXdUDSServer&) = delete;
                FleXdUDSServer& operator=(const FleXdUDSServer&) = delete;

            protected:
                virtual void onNewClient(int fd) {};

            private:
                virtual void readMessage(FleXdEpoll::Event e, std::array<uint8_t, 8192>&& array, int size) override;
                virtual bool onReConnect(int fd) override;
                void onAccept(FleXdEpoll::Event e);
                bool removeFdFromList(int fd);

            private:
                std::map<int, FleXdIPCBuffer> m_list;
            };

        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDUDSSERVER_H */

