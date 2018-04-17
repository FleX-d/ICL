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
#include "FleXdUDS.h"
#include "FleXdLogger.h"
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

            
            FleXdUDSClient::FleXdUDSClient(const std::string& socPath, FleXdEpoll& poller) 
            : FleXdUDS(socPath, poller),
              m_buffer([this](pSharedFleXdIPCMsg msg){onMessage(msg);})
            {
                FLEX_LOG_INIT("FleXdUDSClient");
                FLEX_LOG_INFO("FleXdUDSClient -> Start");
            }

            FleXdUDSClient::~FleXdUDSClient() 
            {
                FLEX_LOG_TRACE("FleXdUDSClient -> Destroyed");
            }

            bool FleXdUDSClient::initialization() 
            {
                if(connectUDS())
                {
                    m_poller.addEvent(getFd(), [this](FleXdEpoll::Event e)
                    {
                        onEvent(e);
                    });
                    FLEX_LOG_TRACE("FleXdUDSClient::initialization()  -> Initialization Success!");
                    return true;         
                }
                FLEX_LOG_WARN("FleXdUDSClient::initialization() -> Initialization Fail!");
                return false;             
            }          
            
            void FleXdUDSClient::readMessage(FleXdEpoll::Event e, std::array<uint8_t, 8192>&& array, int size)
            {
                m_array = std::make_shared<std::array<uint8_t, 8192> >(array);
                m_buffer.rcvMsg(m_array, size);
            }
            
            void FleXdUDSClient::sendMsg(pSharedFleXdIPCMsg msg)
            {
                std::vector<uint8_t> data = msg->releaseMsg();
                size_t sendData = 0;
                while(data.size() > sendData)
                {
                    sendData += write(getFd(),  &data[sendData] , data.size());
                }
                FLEX_LOG_TRACE("FleXdUDSClient::onWrite() -> Write Success!");
            }

            void FleXdUDSClient::onMsg(pSharedFleXdIPCMsg msg)
            {
                FLEX_LOG_WARN("FleXdUDSClient::onMessage() -> function shall be overwritten.");
                // TODO This fcn will be overwritten 
            }
            
            bool FleXdUDSClient::onReConnect(int fd)
            {
                FLEX_LOG_INFO("FleXdUDSClient::onReConnect() -> Try to Reconnect!");
                return connectUDS();
            }
            
        } // namespace epoll
    } // namespace ilc
} // namespace flexd
