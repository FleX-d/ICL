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
 * File:   FleXdUDSClient.h
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 *
 * Created on February 1, 2018, 2:45 PM
 */

#ifndef FLEXDUDSCLIENT_H
#define FLEXDUDSCLIENT_H

#include "FleXdIPCMsg.h"
#include "FleXdIPCBuffer.h"
#include "FleXdUDS.h"


namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdUDSClient : public FleXdUDS {
            public:
                FleXdUDSClient(const std::string& socPath, FleXdEpoll& poller, FleXdIPC* proxy = nullptr);
                FleXdUDSClient(const FleXdUDSClient&) = delete;
                FleXdUDSClient& operator=(const FleXdUDSClient&) = delete;
                virtual ~FleXdUDSClient();

                virtual void sndMsg(pSharedFleXdIPCMsg msg, int fd = 0) override;
                virtual void rcvMsg(pSharedFleXdIPCMsg msg, int fd) override {}

                virtual bool connect() override;
                virtual bool disconnect() override;

                virtual bool isServer() override { return false; }
                virtual bool isClient() override { return true; }

            protected:
                virtual bool initUDS() override;

            private:
                virtual void rcvEvent(FleXdEpoll::Event e) override;
                virtual void readMsg(FleXdEpoll::Event e, byteArray8192&& array, int size) override;
                virtual bool reconnect() override;

            private:
                pSharedArray8192 m_array;
                FleXdIPCBuffer m_buffer;
            };

        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDUDSCLIENT_H */
