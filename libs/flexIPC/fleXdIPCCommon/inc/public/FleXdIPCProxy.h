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
 * File:   FleXdIPCProxy.h
 * Author: Adrian Peniak
 *
 * Created on May 31, 2018, 7:49 PM
 */

#ifndef FLEXDIPCPROXY_H
#define FLEXDIPCPROXY_H

#include "FleXdIPC.h"
#include "FleXdIPCMsg.h"
#include <functional>

namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdIPCProxy : public FleXdIPC {
            public:
                FleXdIPCProxy(pSharedFleXdIPC ipc);
                FleXdIPCProxy(const FleXdIPCProxy&) = delete;
                FleXdIPCProxy& operator=(const FleXdIPCProxy&) = delete;
                virtual ~FleXdIPCProxy();
                
                pSharedFleXdIPC getIPC();
                int getFd() const;
                bool init();
                bool connectIPC();
                bool disconnectIPC();
                void sndMsg(pSharedFleXdIPCMsg msg, int fd = -1);
                
                void setOnInit(std::function<void(bool)> fcn);
                void setOnConnect(std::function<void(bool)> fcn);
                void setOnDisconnect(std::function<void(bool)> fcn);
                void setOnConnectClient(std::function<void(int)> fcn);
                void setOnDisconnectClient(std::function<void(int)> fcn);
                void setOnSndMsg(std::function<void(pSharedFleXdIPCMsg, int)> fcn);
                void setOnRcvMsg(std::function<void(pSharedFleXdIPCMsg, int)> fcn);
                void setOnRcvEvent(std::function<void(FleXdEpoll::Event)> fcn);
                
            protected:
                void connectClient(int fd);
                void disconnectClient(int fd);
                void rcvMsg(pSharedFleXdIPCMsg msg, int);
                void rcvEvent(FleXdEpoll::Event e);
            
            protected:
                pSharedFleXdIPC m_ipc;
            
            private:
                std::function<void(bool)> m_onInit;
                std::function<void(int)> m_onConnectClient;
                std::function<void(int)> m_onDisconnectClient;
                std::function<void(bool)> m_onConnect;
                std::function<void(bool)> m_onDisconnect;
                std::function<void(pSharedFleXdIPCMsg, int)> m_onSndMsg;
                std::function<void(pSharedFleXdIPCMsg, int)> m_onRcvMsg;
                std::function<void(FleXdEpoll::Event)> m_onRcvEvent;
            };
            typedef std::shared_ptr<FleXdIPCProxy> pSharedFleXdIPCProxy;

        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDIPCPROXY_H */

