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
 * File:   FleXdIPCTypes.h
 * Author: Adrian Peniak
 *
 * Created on February 1, 2018, 9:19 AM
 */

#ifndef FLEXDIPCTYPES_H
#define FLEXDIPCTYPES_H

#include <cstdint>
#include <memory>
#include <array>

namespace flexd {
    namespace icl {
        namespace ipc {

            namespace EpollEvent {

                enum Enum {
                    None = 0x00,
                    EpollOut = 0x01,
                    EpollIn = 0x02,
                    EpollError = 0x03,
                    InvalidEpollFd = 0x04,
                    InvalidFd = 0x05,
                    UndefinedError = 0xFF
                };

            } // namespace EpollEvent

            namespace IPCMsgType{
                enum Enum {
                    SyncMsg,
                    DefMsg,
                    ContMsg,
                    AckMsg,
                };
            } //namespace IPCMsgType

        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDIPCTYPES_H */
