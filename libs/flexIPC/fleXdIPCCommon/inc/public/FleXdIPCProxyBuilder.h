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
 * File:   FleXdIPCProxyBuilder.h
 * Author: Adrian Peniak
 *
 * Created on May 31, 2018, 7:49 PM
 */

#ifndef FLEXDIPCPROXYBUILDER_H
#define FLEXDIPCPROXYBUILDER_H

#include "FleXdIPCProxy.h"

namespace flexd {
    namespace icl {
        namespace ipc {

            template<typename T>
            class FleXdIPCProxyBuilder : public FleXdIPCProxy {
            public:
                template<typename... Args>
                FleXdIPCProxyBuilder(Args&&... args)
                : FleXdIPCProxy(std::make_shared<T>(std::forward<Args>(args)..., this)) {}
                FleXdIPCProxyBuilder(const FleXdIPCProxyBuilder&) = delete;
                FleXdIPCProxyBuilder& operator=(const FleXdIPCProxyBuilder&) = delete;
                virtual ~FleXdIPCProxyBuilder() {}
            };

        } // namespace epoll
    } // namespace icl
} // namespace flexd
#endif /* FLEXDIPCPROXYBUILDER_H */

