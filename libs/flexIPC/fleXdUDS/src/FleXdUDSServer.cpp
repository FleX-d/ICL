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
#include "FleXdLogger.h"
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
    namespace ilc {
        namespace epoll {

            FleXdUDSServer::FleXdUDSServer(const std::string& socPath, FleXdEpoll& poller)
            : FleXdUDS(socPath, poller)
            {
                FLEX_LOG_INIT("FleXdUDSServer");
                FLEX_LOG_INFO("FleXdUDSServer -> Start");
            }

            FleXdUDSServer::~FleXdUDSServer()
            {
                FLEX_LOG_TRACE("FleXdUDSServer -> Destroyed");
            }

            bool FleXdUDSServer::initialization()
            {
                if(listenClient())
                {
                    m_poller.addEvent(getFd(), [this](FleXdEpoll::Event evn)
                    {
                        onAccept(evn);
                    });
                    FLEX_LOG_TRACE("FleXdUDSServer::initialization()  -> Initialization Success!");
                    return true;
                }
                FLEX_LOG_WARN("FleXdUDSServer::initialization() -> Initialization Fail!");
                return false;
            }

            void FleXdUDSServer::onAccept(FleXdEpoll::Event e)
            {
                if (e.type == EpollEvent::EpollOut || e.type == EpollEvent::EpollIn)
                {
                    int clientFd;
                    if ((clientFd = accept(getFd(), NULL, NULL)) == -1)
                    {
                        FLEX_LOG_ERROR("FleXdUDSServer::onAccept() -> Accept Error!");
                    }
                    FLEX_LOG_TRACE("FleXdUDSServer::onAccept()  -> Accept Success, fd: ", e.fd);
                    // TODO make a move operator for buffer in order to avoid pointer
                    auto buffer = std::make_unique<FleXdIPCBuffer>([this](pSharedFleXdIPCMsg msg){onMsg(msg);});
                    m_list.insert(std::pair<int, pUniqueFleXdIPCBuffer>(clientFd, std::move(buffer)));
                    m_poller.addEvent(clientFd, [this](FleXdEpoll::Event evn)
                    {
                        onRead(evn);
                    });
                }
            }

            void FleXdUDSServer::readMsg(FleXdEpoll::Event e, std::array<uint8_t, 8192>&& array, int size)
            {
                std::shared_ptr<std::array<uint8_t, 8192> > array_ptr = std::make_shared<std::array<uint8_t, 8192> >(array);
                auto search = m_list.find(e.fd);
                if(search != m_list.end()) 
                {
                    search->second->rcvMsg(array_ptr, size);
                } 
            }

            void FleXdUDSServer::onWrite(pSharedFleXdIPCMsg msg)
            {
                std::vector<uint8_t> data = msg->releaseMsg();
                unsigned sendData = 0;
                while(data.size() > sendData)
                {
                    sendData += write(m_list.begin()->first,  &data[sendData] , data.size());
                }
                FLEX_LOG_TRACE("FleXdUDSServer::onWrite() -> Write Success!");
            }
            
            void FleXdUDSServer::onMessage(pSharedFleXdIPCMsg msg)
            {
                if(msg)
                {
                    FLEX_LOG_DEBUG("FleXdUDSClient::onMessage() -> ", msg->releaseMsg().data());
                    std::vector<uint8_t> payload;
                    std::shared_ptr<FleXdIPCMsg> msg_ptr = std::make_shared<FleXdIPCMsg>(true, 0, 32, 1, 1, 1, 1, 1, 1, std::move(payload));
                    onWrite(std::move(msg_ptr));
                } else {
                    FLEX_LOG_DEBUG("FleXdUDSClient::onMessage() -> Massage Invalid!");
                    std::vector<uint8_t> payload;
                    std::shared_ptr<FleXdIPCMsg> msg_ptr = std::make_shared<FleXdIPCMsg>(false, 0, 32, 0, 0, 0, 0, 0, 0, std::move(payload));
                    onWrite(std::move(msg_ptr));
                } 
            }
            
            bool FleXdUDSServer::onReConnect(int fd) 
            {
                FLEX_LOG_TRACE("FleXdUDSServer::onReConnect()");
                return removeFdFromList(fd);
            }
            
            bool FleXdUDSServer::removeFdFromList(int fd)
            {
                auto it = m_list.find(fd);
                if (it != m_list.end()) {
                    std::ignore = m_list.erase(it);
                    FLEX_LOG_TRACE("FleXdUDSServer::removeFdFromList() -> Remove fd Success!");
                    return true;  
                }
                FLEX_LOG_WARN("FleXdUDSServer::removeFdFromList() -> Remove fd Fail!");
                return false;
            }
            
        } // namespace epoll
    } // namespace ilc
} // namespace flexd
