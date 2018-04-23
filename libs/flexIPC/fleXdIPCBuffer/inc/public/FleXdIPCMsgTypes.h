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
 * File:   FleXdIPCMsgTypes.h
 * Author: Adrian Peniak
 *
 * Created on April 18, 2018, 11:00 AM
 */

#ifndef FLEXDIPCMSGTYPES_H
#define FLEXDIPCMSGTYPES_H

#define HEADER_SIZE 32
#define START_MSG_FLAG 0
#define START_HEADER_FLAG 1
#define END_HEADER_FLAG 2
#define MIN_SIZE_OF_CACHE_TO_PARSE 5

#define IPC_MSG_START_BIT_COUNT 4
#define IPC_MSG_CRC_BIT_COUNT 16
#define IPC_HEADER_FLAG_BIT_COUNT 2
#define IPC_MSG_SIZE_BIT_COUNT 16
#define IPC_MSG_TYPE_BIT_COUNT 8
#define IPC_MSG_ID_BIT_COUNT 16
#define IPC_MSG_APP_ID_BIT_COUNT 64
#define IPC_MSG_TIMESTAMP_BIT_COUNT 32
#define IPC_MSG_TTL_BIT_COUNT 32

namespace flexd {
    namespace icl {
        namespace epoll {
            
            namespace FleXdIPCMsgTypes {
                enum Enum {
                    Handshake = 0x00,
                    IPCMsg,
                    IPCMsgAck = 0xFC,
                    HandshakeSuccess = 0xFD,
                    HandshakeFail = 0xFE,
                    Undefined = 0xFF
                };
            } // namespace FleXdIPCMsgTypes
 
        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDIPCMSGTYPES_H */

