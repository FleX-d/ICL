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
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <map>

namespace flexd {
    namespace icl {
        namespace ipc {

            FleXdUDSServer::FleXdUDSServer(const std::string& socPath, FleXdEpoll& poller, FleXdIPC* proxy)
            : FleXdUDS(socPath, poller, proxy) {
            }

            FleXdUDSServer::~FleXdUDSServer() {
                for (const auto& ref : m_map) {
                    m_poller.rmEvent(ref.first);
                    ::close(ref.first);
                }
                m_poller.rmEvent(getFd());
                ::close(getFd());
                ::unlink(m_socPath.c_str());
                ::system("sync");
            }

            bool FleXdUDSServer::initUDS() {
                if (listenUDS()) {
                    m_poller.addEvent(getFd(), [this](FleXdEpoll::Event evn) {
                        m_proxy->rcvEvent(evn);
                    });
                    return true;
                }
                return false;
            }

            void FleXdUDSServer::rcvEvent(FleXdEpoll::Event e) {
                if (e.type == EpollEvent::EpollIn) {
                    // self fd is listen fd for incoming connections -> accept will perform
                    if(e.fd == getFd()) {
                        int clientFd = ::accept(getFd(), NULL, NULL);
                        if (m_map.count(clientFd) == 0) {
                            m_proxy->connectClient(clientFd);
                        }
                    // other fd's are client fd's -> read will perform
                    } else {
                        int rc = 1;
                        byteArray8192 array;
                        while ((rc = ::read(e.fd, &array[0], sizeof (array))) > 0) {
                            readMsg(e, std::move(array), rc);
                        }
                    }
                } else if (e.type == EpollEvent::EpollError) {
                    if(e.fd != getFd()) {
                        m_proxy->disconnectClient(e.fd);
                    }
                }
            }

            void FleXdUDSServer::connectClient(int fd) {
                ::fcntl(fd, F_SETFL, O_NONBLOCK);
                FleXdIPCBuffer buffer(fd, [this](pSharedFleXdIPCMsg msg, int fd) {
                    m_proxy->rcvMsg(msg, fd);
                });
                m_map.insert(std::make_pair(fd, std::move(buffer)));
                m_poller.addEvent(fd, [this](FleXdEpoll::Event evn) {
                    m_proxy->rcvEvent(evn);
                });
            }

            void FleXdUDSServer::disconnectClient(int fd) {
                m_poller.rmEvent(fd);
                ::close(fd);
                std::ignore = m_map.erase(fd);
            }

            void FleXdUDSServer::readMsg(FleXdEpoll::Event e, byteArray8192&& array, int size) {
                std::shared_ptr<byteArray8192> array_ptr = std::make_shared<byteArray8192>(array);
                auto search = m_map.find(e.fd);
                if (search != m_map.end()) {
                    search->second.rcvMsg(array_ptr, size);
                }
            }

            void FleXdUDSServer::sndMsg(pSharedFleXdIPCMsg msg, int fd) {
                if (msg) {
                    auto it = m_map.find(fd);
                    if (it != m_map.end()) {
                        std::vector<uint8_t> data = msg->releaseMsg();
                        unsigned sendData = 0;
                        while (data.size() > sendData) {
                            sendData += ::write(it->first, &data[sendData], data.size());
                        }
                    }
                }
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
