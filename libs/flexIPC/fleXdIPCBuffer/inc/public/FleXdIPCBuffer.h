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
 * File:   FleXdIPCBuffer.h
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 * 
 * Created on January 31, 2018, 8:32 PM
 */

#ifndef FLEXDIPCBUFFER_H
#define FLEXDIPCBUFFER_H

#include "FleXdIPCMsg.h"
#include "FleXdIPCBufferTypes.h"
#include <functional>
#include <cstdint>
#include <array>
#include <queue>
#include <atomic>
#include <memory>

namespace flexd {
    namespace ilc {
        namespace epoll {

	    class FleXdIPCFactory;
            class FleXdIPCBuffer {
            public:
                explicit FleXdIPCBuffer(size_t maxBufferSize = 65536);
                explicit FleXdIPCBuffer(std::function<void(pSharedFleXdIPCMsg msg)> onMsg, size_t maxBufferSize = 65536);
                ~FleXdIPCBuffer();
                
                void rcvMsg(pSharedArray8192& data, size_t size);
                pSharedFleXdIPCMsg front() const;
                pSharedFleXdIPCMsg back() const;
                pSharedFleXdIPCMsg pop();

                FleXdIPCBuffer(const FleXdIPCBuffer&) = delete;
                FleXdIPCBuffer& operator=(const FleXdIPCBuffer&) = delete;
                
            private:
                void releaseMsg(pSharedFleXdIPCMsg msg);
                
            private:
                const size_t m_maxBufferSize;
                size_t m_bufferSize;
                std::queue<pSharedFleXdIPCMsg> m_queue;
                std::function<void(pSharedFleXdIPCMsg)> m_onMsg;
                std::unique_ptr<FleXdIPCFactory> m_factory;
            };
            typedef std::shared_ptr<FleXdIPCBuffer> pUniqueFleXdIPCBuffer;
            
        } // namespace epoll
    } // namespace ilc
} // namespace flexd

#endif /* FLEXDIPCBUFFER_H */
