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
 * File:   FleXdIPC.h
 * Author: Adrian Peniak
 *
 * Created on May 31, 2018, 7:49 PM
 */

#ifndef FLEXDIPC_H
#define FLEXDIPC_H

#include "FleXdIPCMsg.h"
#include "FleXdEpoll.h"
#include <memory>

namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdIPC {
            public:
                FleXdIPC() = default;
                FleXdIPC(const FleXdIPC&) = delete;
                FleXdIPC& operator=(const FleXdIPC&) = delete;
                virtual ~FleXdIPC() {}

                virtual int getFd() const = 0;
                virtual bool init() = 0;
                virtual bool connect() { return false; }
                virtual bool disconnect() { return false; }
                virtual void sndMsg(pSharedFleXdIPCMsg msg, int fd) = 0;
                virtual void connectClient(int fd) {}
                virtual void disconnectClient(int fd) {}
                virtual void rcvMsg(pSharedFleXdIPCMsg msg, int fd) = 0;
                virtual void rcvEvent(FleXdEpoll::Event e) = 0;

                virtual bool isServer() = 0;
                virtual bool isClient() = 0;

                virtual void onInit(bool ret) {}
                virtual void onConnectClient(int fd) {}
                virtual void onDisconnectClient(int fd) {}
                virtual void onConnect(bool ret) {}
                virtual void onDisconnect(int fd) {}
                virtual void onSndMsg(pSharedFleXdIPCMsg msg, int fd) {}
                virtual void onRcvMsg(pSharedFleXdIPCMsg msg, int fd) {}
                virtual void onRcvEvent(FleXdEpoll::Event e) {}
            };
            typedef std::shared_ptr<FleXdIPC> pSharedFleXdIPC;

        } // namespace epoll
    } // namespace icl
} // namespace flexd
#endif /* FLEXDIPC_H */

