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
 * File:   FleXdIPCFactory.h
 * Author: Matus Bodorik
 *
 * Created on February 21, 2018, 8:32 AM
 */

#ifndef FLEXDIPCFACTORY_H
#define FLEXDIPCFACTORY_H

#include "FleXdIPCBufferTypes.h"
#include "FleXdIPCMsg.h"
#include "BitStream.h"
#include <vector>
#include <queue>


namespace flexd {
    namespace ilc {
        namespace epoll {

            class FleXdIPCFactory {
            public:
                FleXdIPCFactory(std::function<void(pSharedFleXdIPCMsg msg)> releaseMsg);
                virtual ~FleXdIPCFactory();
                
                void parseData(const pSharedArray8192& data,const size_t size);
                void releaseMsg(bool complete, uint16_t crc16, uint16_t msgSize, uint8_t type, uint16_t msgID, uint64_t from, uint64_t to, uint32_t timeStamp, uint32_t ttl,const std::vector<uint8_t>& payload);

                FleXdIPCFactory(const FleXdIPCFactory& orig) = delete;
                FleXdIPCFactory& operator=(const FleXdIPCFactory& orig) = delete;
                
            private:
                BiteStream m_data;
                std::function<void(pSharedFleXdIPCMsg) > m_releaseMsg;
            };
        }
    }
}
#endif /* FLEXDIPCFACTORY_H */

