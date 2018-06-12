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

#define MAXBUFFERSIZE 65536

#include "FleXdIPCMsg.h"
#include "FleXdIPCBufferTypes.h"
#include "BitStream.h"
#include <functional>
#include <cstdint>
#include <queue>
#include <array>

namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdIPCBuffer {
            public:
                explicit FleXdIPCBuffer(int fd, size_t maxBufferSize = MAXBUFFERSIZE);
                explicit FleXdIPCBuffer(int fd, std::function<void(pSharedFleXdIPCMsg, int)> onMsg, size_t maxBufferSize = MAXBUFFERSIZE);
                FleXdIPCBuffer(const FleXdIPCBuffer&) = delete;
                FleXdIPCBuffer& operator=(const FleXdIPCBuffer&) = delete;
                virtual ~FleXdIPCBuffer();
                FleXdIPCBuffer(FleXdIPCBuffer&&);
                FleXdIPCBuffer& operator=(FleXdIPCBuffer&&);

                int getFd() const;
                void setFd(int fd);
                /**
                 * Function receive data, push them to end of cache and call function rcvMsg()
                 * @param data - shared pointer to Array of data which will be parsed
                 * @param size - size of data
                 */
                void rcvMsg(pSharedArray8192& data, size_t size);
                /**
                 * Function return FleXdIPCMsg from queue, but not remove it
                 * @return shared pointer to FleXdIPCMsg from front of queue
                 */
                pSharedFleXdIPCMsg front() const;
                /**
                 * Function return FleXdIPCMsg from queue, but not remove it
                 * @return shared pointer to FleXdIPCMsg from back of queue
                 */
                pSharedFleXdIPCMsg back() const;
                /**
                 * Function return FleXdIPCMsg from queue and remove it
                 * @return shared pointer to FleXdIPCMsg from front of queue
                 */
                pSharedFleXdIPCMsg pop();


            private:
                /**
                 * Function parse data from cache, when find non-corrupted message call function releaseMsg()
                 * when find corrupted message, call function findNonCoruptedMessage()
                 */
                void rcvMsg();
                /**
                 * Function call lambda method with complete message if exist, otherwise push complete message to buffer
                 * @param msg - shared pointer to FleXdIPCMsg
                 */
                void releaseMsg(pSharedFleXdIPCMsg&& msg);
                /**
                 * Function try to find next non-corrupted message at cache and remove corrupted message
                 * @param coruptedMsgSize - Size of corrupted message, otherwise 0
                 */
                void findNonCoruptedMessage(uint16_t coruptedMsgSize);

            private:
                int m_fd;
                size_t m_maxBufferSize;
                size_t m_bufferSize;
                BitStream m_cache;
                std::queue<pSharedFleXdIPCMsg> m_queue;
                std::function<void(pSharedFleXdIPCMsg, int)> m_onMsg;
            };

        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDIPCBUFFER_H */
