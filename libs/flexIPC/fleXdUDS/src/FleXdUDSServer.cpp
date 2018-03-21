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
 * File:   FleXdUDSServer.cpp
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 * 
 * Created on February 1, 2018, 2:45 PM
 */

#include "FleXdUDSServer.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <thread>
#include <map>

namespace flexd {
    namespace ilc {
        namespace epoll {

            struct FleXdUDSServer::Ctx {
                struct sockaddr_un addr;
                int fd;
            };

            FleXdUDSServer::FleXdUDSServer(const std::string& socPath, FleXdEpoll& poller)
            : m_ctx(std::make_unique<Ctx>()),
            m_socPath(socPath),
            m_poller(poller)
            {
            }

            FleXdUDSServer::~FleXdUDSServer()
            {
            }

            bool FleXdUDSServer::init()
            {
                if ((m_ctx->fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
                {
                    std::cerr << "socket error\n";
                    return false;
                }

                memset(&(m_ctx->addr), 0, sizeof (m_ctx->addr));
                m_ctx->addr.sun_family = AF_UNIX;
                if (m_socPath[0] == '\0')
                {
                    *(m_ctx->addr.sun_path) = '\0';
                    strncpy(m_ctx->addr.sun_path + 1, m_socPath.c_str() + 1, sizeof (m_ctx->addr.sun_path) - 2);
                } else
                {
                    strncpy(m_ctx->addr.sun_path, m_socPath.c_str(), sizeof (m_ctx->addr.sun_path) - 1);
                    unlink(m_socPath.c_str());
                }

                if (bind(m_ctx->fd, (struct sockaddr*) &(m_ctx->addr), sizeof (m_ctx->addr)) == -1)
                {
                    std::cerr << "bind error\n";
                    return false;
                }

                if (listen(m_ctx->fd, 5) == -1)
                {
                    std::cerr << "listen error\n";
                    return false;
                }

                m_poller.addEvent(m_ctx->fd, [this](FleXdEpoll::Event evn)
                {
                    onAccept(evn);
                });

                return true;
            }

            void FleXdUDSServer::onAccept(FleXdEpoll::Event e)
            {
                std::cout << "FleXdUDSServer::onAccept()\n";
                if (e.type == EpollEvent::EpollOut || e.type == EpollEvent::EpollIn)
                {
                    int clientFd;
                    if ((clientFd = accept(m_ctx->fd, NULL, NULL)) == -1)
                    {
                        std::cerr << "accept error\n";
                    }
                    std::cout << " OnAccept() -> " << e.fd << std::endl;
                    m_clientList.insert(std::pair<int, std::string>(clientFd, ""));
                    // TODO make a move operator for buffer in order to avoid pointer
                    auto buffer = std::make_unique<FleXdIPCBuffer>([this](pSharedFleXdIPCMsg msg){onMsg(msg);});
                    m_list.insert(std::pair<int, pUniqueFleXdIPCBuffer>(clientFd, std::move(buffer)));
                    m_poller.addEvent(clientFd, [this](FleXdEpoll::Event evn)
                    {
                        onMsg(evn);
                    });
                }
            }

            void FleXdUDSServer::onMsg(FleXdEpoll::Event e)
            {
                std::cout << "FleXdUDSServer::onMsg() " << e.fd << std::endl;
                if (e.type == EpollEvent::EpollOut)
                {

                } else if (e.type == EpollEvent::EpollIn)
                {
                    int rc;
                    std::array<uint8_t, 8192> array;
                    while ((rc = read(e.fd, &array[0], sizeof(array))) > 0)
                    {          
                        std::shared_ptr<std::array<uint8_t, 8192> > array_ptr = std::make_shared<std::array<uint8_t, 8192> >(array);
                        auto search = m_list.find(e.fd);
                        if(search != m_list.end()) 
                        {
                            search->second->rcvMsg(array_ptr, rc);
                        }
                    }
                }
            }

            void FleXdUDSServer::onMsg(pSharedFleXdIPCMsg msg)
            {
                if(msg)
                {
                    std::vector<uint8_t> data = msg->releaseMsg();
                    for(auto it : data)
                    {
                        std::cout << (int)it << " "; 
                    }
                    std::cout << std::endl;         
                    FleXdIPCMsg msg(true, 0, 33, 1, 1,  1, 1, 1, 1, {1});
                    std::shared_ptr<FleXdIPCMsg> msg_ptr = std::make_shared<FleXdIPCMsg>(msg);
                    sendMsg(msg_ptr);
                } else {
                    std::cout << "Message is Invalid!" << std::endl;
                }
            }

            void FleXdUDSServer::sendMsg(pSharedFleXdIPCMsg msg)
            {
                std::vector<uint8_t> data = msg->releaseMsg();
                unsigned sendData = 0;
                while(data.size() > sendData)
                {
                    sendData += write(m_clientList.begin()->first,  &data[sendData] , data.size());
                }
            }

        } // namespace epoll
    } // namespace ilc
} // namespace flexd
