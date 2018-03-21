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
 * File:   FleXdUDSClient.cpp
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 * 
 * Created on February 1, 2018, 2:45 PM
 */

#include "FleXdUDSClient.h"
#include "FleXdIPCBuffer.h"
#include <cstring>
#include <iostream>

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace flexd {
    namespace ilc {
        namespace epoll {

            struct FleXdUDSClient::Ctx {
                struct sockaddr_un addr;
                int fd;
            };

            FleXdUDSClient::FleXdUDSClient(const std::string& socPath, FleXdEpoll& poller)
            : m_ctx(std::make_unique<Ctx>()),
            m_socPath(socPath),
            m_poller(poller)
            {
            }

            FleXdUDSClient::~FleXdUDSClient() {
            }

            bool FleXdUDSClient::init() {
                if ((m_ctx->fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
                    std::cerr << "socket error...\n";
                    return false;
                }

                std::memset(&(m_ctx->addr), 0, sizeof (m_ctx->addr));
                m_ctx->addr.sun_family = AF_UNIX;
                if (m_socPath[0] == '\0') {
                    *(m_ctx->addr.sun_path) = '\0';
                    std::strncpy(m_ctx->addr.sun_path + 1, m_socPath.c_str() + 1, sizeof (m_ctx->addr.sun_path) - 2);
                } else {
                    std::strncpy(m_ctx->addr.sun_path, m_socPath.c_str(), sizeof (m_ctx->addr.sun_path) - 1);
                    //unlink(m_socPath.c_str());
                }
               
                
                m_buffer = std::make_unique<FleXdIPCBuffer>([this](pSharedFleXdIPCMsg msg){onMsg(msg);});
                m_poller.addEvent(m_ctx->fd, [this](FleXdEpoll::Event e)
                {
                    onRead(e);
                });
                
                return true;         
            }
            
            
            void FleXdUDSClient::onConnect()
            {
                if (connect(m_ctx->fd, (struct sockaddr*) &(m_ctx->addr), sizeof (m_ctx->addr)) == -1) 
                {
                   std::cerr << "connect error...\n";
                }
            }
            
            void FleXdUDSClient::onReConnect()
            {
                
            }
            
            void FleXdUDSClient::onRead(FleXdEpoll::Event e)
            {
                std::cout << "FleXdUDSClient::onRead() " << e.fd << std::endl;
                if (e.type == EpollEvent::EpollOut)
                {

                } else if (e.type == EpollEvent::EpollIn)
                {
                    int rc;
                    std::array<uint8_t, 8192> array;
                    while ((rc=read(m_ctx->fd, &array[0], sizeof(array))) > 0) 
                    {
                        m_array = std::make_shared<std::array<uint8_t, 8192> >(array);
                        m_buffer->rcvMsg(m_array, rc);
                    }
                }
            }
            
            void FleXdUDSClient::onWrite(pSharedFleXdIPCMsg msg)
            {
                std::vector<uint8_t> data = msg->releaseMsg();
                unsigned sendData = 0;
                while(data.size() > sendData)
                {
                    sendData += write(m_ctx->fd,  &data[sendData] , data.size());
                }
            }

            void FleXdUDSClient::onMsg(pSharedFleXdIPCMsg msg)
            {
                std::cout << "FleXdUDSClient::onMsg() " << std::endl;
                if(msg)
                {
                    std::vector<uint8_t> data = msg->releaseMsg();
                    for(auto it : data)
                    {
                        std::cout << (int)it << " "; 
                    }
                    std::cout << std::endl;         
                } else {
                    std::cout << "Message is Invalid!" << std::endl;
                }
            }
        } // namespace epoll
    } // namespace ilc
} // namespace flexd
