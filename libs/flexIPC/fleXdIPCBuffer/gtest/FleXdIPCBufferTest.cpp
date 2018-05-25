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
 * File:   FleXdIPCBufferTest.cpp
 *
 * Author: Matus Bodorik
 *
 * Created on April 06, 2018, 10:02 AM
 */

#include "FleXdIPCBuffer.h"
#include <gtest/gtest.h>
#include <stdint.h>
#include "BitStream.h"
#include "vector"

namespace {

    using FleXdIPCBuffer = flexd::icl::epoll::FleXdIPCBuffer;
    using FleXdIPCMsg = flexd::icl::epoll::FleXdIPCMsg;
    using pSharedArray8192 = flexd::icl::epoll::pSharedArray8192;
    using pSharedFleXdIPCMsg = flexd::icl::epoll::pSharedFleXdIPCMsg;

    TEST(FleXdIPCBuffer, Positive_Response_Funcion_rcvMsg)
    {
        /*FleXdIPCBuffer buffer;
        //TODO fill data with { 8, 0, 12, 252, 0, 98, 1, 2, 108, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 9, 8, 7, 6 };
        pSharedArray8192 data; //{ 8, 0, 12, 252, 0, 98, 1, 2, 108, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 9, 8, 7, 6 };

        buffer.rcvMsg(data, data->size());

        pSharedFleXdIPCMsg msg = std::make_shared<FleXdIPCMsg>(buffer.pop());

        std::vector<uint8_t> vector = msg->releaseMsg();
        std::vector<uint8_t> vmsg = { 12, 252, 0, 98, 1, 2, 108, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 9, 8, 7, 6 };
        EXPECT_EQ(vector, vmsg);
        EXPECT_EQ(vector.size(), msg->getMsgSize());*/
    }

}
