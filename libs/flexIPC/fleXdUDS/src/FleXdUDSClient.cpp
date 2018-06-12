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
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace flexd {
    namespace icl {
        namespace ipc {


            FleXdUDSClient::FleXdUDSClient(const std::string& socPath, FleXdEpoll& poller, FleXdIPC* proxy)
            : FleXdUDS(socPath, poller, proxy),
              m_buffer(getFd(), [this](pSharedFleXdIPCMsg msg, int fd){ m_proxy->rcvMsg(msg, fd);}) {
            }

            FleXdUDSClient::~FleXdUDSClient() {
            }

            bool FleXdUDSClient::connect() {
                if(connectUDS())
                {
                    m_poller.addEvent(getFd(), [this](FleXdEpoll::Event e)
                    {
                        m_proxy->rcvEvent(e);
                    });
                    m_buffer.setFd(getFd());
                    return true;
                }
                return false;
            }

            bool FleXdUDSClient::disconnect() {
                return reconnect(0);
            }

            bool FleXdUDSClient::initUDS() {
                return m_proxy->connect();
            }

            void FleXdUDSClient::rcvEvent(FleXdEpoll::Event e) {
                if (e.type == EpollEvent::EpollIn) {
                    int rc = 1;
                    byteArray8192 array;
                    while ((rc = ::read(e.fd, &array[0], sizeof (array))) > 0) {
                        readMsg(e, std::move(array), rc);
                    }
                } else if (e.type == EpollEvent::EpollError) {
                    m_proxy->disconnect();
                }
            }

            void FleXdUDSClient::readMsg(FleXdEpoll::Event e, byteArray8192&& array, int size) {
                m_array = std::make_shared<byteArray8192>(array);
                m_buffer.rcvMsg(m_array, size);
            }

            void FleXdUDSClient::sndMsg(pSharedFleXdIPCMsg msg, int) {
                std::vector<uint8_t> data = msg->releaseMsg();
                size_t sendData = 0;
                while(data.size() > sendData)
                {
                    sendData += ::write(getFd(),  &data[sendData] , data.size());
                }
            }

            bool FleXdUDSClient::reconnect(int fd) {
                return m_proxy->connect();
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
