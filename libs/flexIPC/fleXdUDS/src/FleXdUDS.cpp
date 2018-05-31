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
 * File:   FleXdUDS.cpp
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 *
 * Created on February 2, 2018, 1:51 PM
 */

#include "FleXdUDS.h"
#include "FleXdIPCBufferTypes.h"
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>


namespace flexd {
    namespace icl {
        namespace ipc {

            struct FleXdUDS::Ctx {
                struct sockaddr_un addr;
                int fd;
            };

            FleXdUDS::FleXdUDS(const std::string& socPath, FleXdEpoll& poller)
            : m_poller(poller),
              m_socPath(socPath),
              m_ctx(std::make_unique<Ctx>())
            {
            }

            FleXdUDS::~FleXdUDS()
            {
            }

            bool FleXdUDS::init()
            {
                if ((m_ctx->fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
                {
                    return false;
                }

                std::memset(&(m_ctx->addr), 0, sizeof (m_ctx->addr));
                m_ctx->addr.sun_family = AF_UNIX;
                if (m_socPath[0] == '\0')
                {
                    *(m_ctx->addr.sun_path) = '\0';
                    std::strncpy(m_ctx->addr.sun_path + 1, m_socPath.c_str() + 1, sizeof (m_ctx->addr.sun_path) - 2);
                } else
                {
                    std::strncpy(m_ctx->addr.sun_path, m_socPath.c_str(), sizeof (m_ctx->addr.sun_path) - 1);
                }
                return initialization();
            }

            void FleXdUDS::onEvent(FleXdEpoll::Event e)
            {
                if (e.type == EpollEvent::EpollIn)
                {
                    int rc;
                    byteArray8192 array;
                    while ((rc = read(e.fd, &array[0], sizeof (array))) > 0)
                    {
                        readMessage(e, std::move(array), rc);
                    }
                } else if (e.type == EpollEvent::EpollError){
                    onReConnect(e.fd);
                }
            }

            void FleXdUDS::onMessage(pSharedFleXdIPCMsg msg)
            {
                if (msg)
                {
                    std::vector<uint8_t> data = msg->releaseMsg();
                    // TMP print
                    for (auto it : data)
                    {
                        std::cout << (int) it << " ";
                    }
                    std::cout << std::endl;

                    onMsg(msg);
                }
            }

            int FleXdUDS::getFd() const
            {
                return m_ctx->fd;
            }

            bool FleXdUDS::connectUDS()
            {

                if (connect(m_ctx->fd, (struct sockaddr*) &(m_ctx->addr), sizeof (m_ctx->addr)) == -1)
                {
                   return false;
                }
                return true;
            }

            bool FleXdUDS::listenUDS()
            {
                if (m_socPath[0] != '\0')
                {
                    unlink(m_socPath.c_str());
                }
                if (bind(m_ctx->fd, (struct sockaddr*) &(m_ctx->addr), sizeof (m_ctx->addr)) == -1)
                {
                    return false;
                }

                if (listen(m_ctx->fd, 5) == -1)
                {
                    return false;
                }
                return true;
            }

        } // namespace epoll
    } // namespace icl
} // namespace flexd
