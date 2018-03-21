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

#include <string>
#include <memory>
#include "FleXdIPCBufferTypes.h"
#include "FleXdIPCMsg.h"
#include "FleXdEpoll.h"
#include "FleXdIPCBuffer.h"

namespace flexd {
    namespace ilc {
        namespace epoll {

            class FleXdUDSClient {
            public:
                FleXdUDSClient(const std::string& socPath, FleXdEpoll& poller);
                virtual ~FleXdUDSClient();
                
                bool init();
                
                FleXdUDSClient(const FleXdUDSClient&) = delete;
                FleXdUDSClient& operator=(const FleXdUDSClient&) = delete;
                
                void onWrite(pSharedFleXdIPCMsg msg);
                void onRead(FleXdEpoll::Event e);
                void onConnect();
                void onReConnect();
                void onMsg(pSharedFleXdIPCMsg msg);
                
                
            private:
                class Ctx;
                std::unique_ptr<Ctx> m_ctx;
                const std::string m_socPath;
                pSharedArray8192 m_array;
                pUniqueFleXdIPCBuffer m_buffer;
                FleXdEpoll& m_poller;
            };

        } // namespace epoll
    } // namespace ilc
} // namespace flexd

#endif /* FLEXDUDSCLIENT_H */
