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

#define IPC_MSG_START_MSG_FLAG 0
#define IPC_MSG_START_HEADER_FLAG 1
#define IPC_MSG_END_HEADER_FLAG 2
#define IPC_MSG_MIN_SIZE_OF_CACHE_TO_PARSE 4 // bytes

#define IPC_MSG_START_MSG_FLAG_BIT_COUNT 3
#define IPC_MSG_HEADER_START_FLAG_BIT_COUNT 2
#define IPC_MSG_HEADER_PARAMETER_TYPE_BIT_COUNT 1 // 0 = message type, 1 = with additional header
#define IPC_MSG_HEADER_PARAMETER_BIT_COUNT 8
#define IPC_MSG_MSG_SIZE_BIT_COUNT 16
#define IPC_MSG_HEADER_END_FLAG_BIT_COUNT 2
#define IPC_MSG_HEADER_SIZE 4 // bytes

#define IPC_MSG_ADDITIONAL_HEADER_VALUE_0_BIT_COUNT 8
#define IPC_MSG_ADDITIONAL_HEADER_VALUE_1_BIT_COUNT 8
#define IPC_MSG_ADDITIONAL_HEADER_VALUE_2_BIT_COUNT 16
#define IPC_MSG_ADDITIONAL_HEADER_VALUE_3_BIT_COUNT 32
#define IPC_MSG_ADDITIONAL_HEADER_VALUE_4_BIT_COUNT 32
#define IPC_MSG_ADDITIONAL_HEADER_VALUE_5_BIT_COUNT 32
#define IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_BIT_COUNT 8

#define IPC_MSG_HEADER_PARAM_TYPE_MSG_TYPE 0
#define IPC_MSG_HEADER_PARAM_TYPE_ADDITIONAL_HEADER 1

#define IPC_MSG_ADDITIONAL_HEADER_0_FLAG                0x01
#define IPC_MSG_ADDITIONAL_HEADER_1_FLAG                0x02
#define IPC_MSG_ADDITIONAL_HEADER_2_FLAG                0x04
#define IPC_MSG_ADDITIONAL_HEADER_3_FLAG                0x08
#define IPC_MSG_ADDITIONAL_HEADER_4_FLAG                0x10
#define IPC_MSG_ADDITIONAL_HEADER_5_FLAG                0x20
#define IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_FLAG 0x40
#define IPC_MSG_HEADER_IN_PAYLOAD_FLAG                  0xFF

namespace flexd {
    namespace icl {
        namespace ipc {

            namespace FleXdIPCMsgTypes {
                enum Enum {
                    Handshake = 0x00,
                    HandshakeAck,
                    HandshakeSuccess,
                    HandshakeSuccessGeneric,
                    HandshakeFail,
                    IPCMsg,
                    IPCMsgAck,
                    Undefined = 0xFF
                };
            } // namespace FleXdIPCMsgTypes

        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDIPCMSGTYPES_H */

