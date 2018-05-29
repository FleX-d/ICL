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
#include "FleXdUDS.h"
#include "FleXdIPCMsg.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <thread>
#include <map>

namespace flexd {
    namespace icl {
        namespace epoll {

            FleXdUDSServer::FleXdUDSServer(const std::string& socPath, FleXdEpoll& poller)
            : FleXdUDS(socPath, poller)
            {
            }

            FleXdUDSServer::~FleXdUDSServer()
            {
            }

            bool FleXdUDSServer::initialization()
            {
                if(listenUDS())
                {
                    m_poller.addEvent(getFd(), [this](FleXdEpoll::Event evn)
                    {
                        onAccept(evn);
                    });
                    return true;
                }
                return false;
            }

            void FleXdUDSServer::onAccept(FleXdEpoll::Event e)
            {
                if (e.type == EpollEvent::EpollOut || e.type == EpollEvent::EpollIn)
                {
                    int clientFd;
                    clientFd = accept(getFd(), NULL, NULL);
                    auto it = m_list.find(clientFd);
                    if(it == m_list.end())
                    {
                        FleXdIPCBuffer buffer([this](pSharedFleXdIPCMsg msg){onMessage(msg);});
                        m_list[clientFd] = std::move(buffer);
                        m_poller.addEvent(clientFd, [this](FleXdEpoll::Event evn){onEvent(evn);});
                        onNewClient(clientFd);
                    }
                }
            }

            void FleXdUDSServer::readMessage(FleXdEpoll::Event e, std::array<uint8_t, 8192>&& array, int size)
            {
                std::shared_ptr<std::array<uint8_t, 8192> > array_ptr = std::make_shared<std::array<uint8_t, 8192> >(array);
                auto search = m_list.find(e.fd);
                if(search != m_list.end())
                {
                    search->second.rcvMsg(array_ptr, size);
                }
            }

            void FleXdUDSServer::sendMsg(pSharedFleXdIPCMsg msg, int fd)
            {
                if(msg)
                {
                    auto it = m_list.find(fd);
                    if (it != m_list.end()) {
                        std::vector<uint8_t> data = msg->releaseMsg();
                        unsigned sendData = 0;
                        while(data.size() > sendData)
                        {
                            sendData += write(it->first, &data[sendData], data.size());
                        }
                    }
                }
            }

            void FleXdUDSServer::onMsg(pSharedFleXdIPCMsg msg)
            {
                  // TODO This fcn will be overwritten
//                if(msg)
//                {
//                    std::vector<uint8_t> payload;
//                    std::shared_ptr<FleXdIPCMsg> msg_ptr = std::make_shared<FleXdIPCMsg>(true, 0, 32, 1, 1, 1, 1, 1, 1, std::move(payload));
//                    sendMsg(std::move(msg_ptr));
//                } else {
//                    std::vector<uint8_t> payload;
//                    std::shared_ptr<FleXdIPCMsg> msg_ptr = std::make_shared<FleXdIPCMsg>(false, 0, 32, 0, 0, 0, 0, 0, 0, std::move(payload));
//                    sendMsg(std::move(msg_ptr));
//                }
            }

            bool FleXdUDSServer::onReConnect(int fd)
            {
                return removeFdFromList(fd);
            }

            bool FleXdUDSServer::removeFdFromList(int fd)
            {
                auto it = m_list.find(fd);
                if (it != m_list.end()) {
                    std::ignore = m_list.erase(it);
                    return true;
                }
                return false;
            }

        } // namespace epoll
    } // namespace icl
} // namespace flexd
