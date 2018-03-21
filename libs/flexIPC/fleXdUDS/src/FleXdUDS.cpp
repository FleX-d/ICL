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
 * 
 * Created on February 2, 2018, 1:51 PM
 */

#include "FleXdUDS.h"
#include <cstring>
#include <sys/un.h>
#include <sys/socket.h>
#include <iostream>

namespace flexd {
    namespace ilc {
        namespace epoll {

            struct FleXdUDS::Ctx {
                struct sockaddr_un addr;
            };
            
            FleXdUDS::FleXdUDS(const std::string& socPath)
            : m_fd(),
              m_socPath(socPath),
              m_ctx(std::make_unique<Ctx>()) {
            }

            FleXdUDS::~FleXdUDS() {
            }

            bool FleXdUDS::init() {
                if ((m_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
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
                return true;
            }
            
        } // namespace epoll
    } // namespace ilc
} // namespace flexd
