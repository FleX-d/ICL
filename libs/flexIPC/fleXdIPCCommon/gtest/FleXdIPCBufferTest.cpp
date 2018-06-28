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
#include "FleXdIPCMsgTypes.h"
#include "BitStream.h"
#include <gtest/gtest.h>
#include <stdint.h>
#include <vector>

using namespace flexd::icl::ipc;

namespace {
    std::vector<uint8_t> createBigPayload()
    {
        std::vector<uint8_t> ret;
        for (uint32_t i = 0; i < UINT16_MAX - UINT8_MAX; i++)
        {
            ret.push_back(i % UINT8_MAX);
        }
        return ret;
    }

    std::vector<uint8_t> bigPayload(createBigPayload());

    TEST(FleXdIPCBuffer, Positive_Response_Funcion_rcvMsg_normal)
    {
        FleXdIPCBuffer buffer(0);
        pSharedArray8192 data(std::make_shared<std::array<uint8_t, 8192>>(std::array<uint8_t, 8192>{ 12, 252, 0, 98, 1, 2, 108, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 9, 8, 7, 6 }));

        buffer.rcvMsg(data, 24);

        pSharedFleXdIPCMsg msg(buffer.pop());

        std::vector<uint8_t> vector(msg->releaseMsg());
        std::vector<uint8_t> vmsg = { 12, 252, 0, 98, 1, 2, 108, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 9, 8, 7, 6 };
        EXPECT_EQ(vector, vmsg);
        EXPECT_EQ(vector.size(), msg->getMsgSize());
    }

    TEST(FleXdIPCBuffer, Positive_Response_Funcion_rcvMsg_normal_big)
    {
        FleXdIPCBuffer buffer(0);
        pSharedArray8192 data(std::make_shared<std::array<uint8_t, 8192>>(std::array<uint8_t, 8192>{ 12, 255, 252, 82, 1, 2, 146, 19, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5 }));
        uint32_t payloadIndex = 0;
        uint16_t i = 20;
        while (payloadIndex < UINT16_MAX - UINT8_MAX)
        {
            data->at(i) = payloadIndex % UINT8_MAX;
            i++;
            if (i >= 8192)
            {
                buffer.rcvMsg(data, 8192);
                i = 0;
            }
            payloadIndex++;
        }
        buffer.rcvMsg(data, i);

        pSharedFleXdIPCMsg msg(buffer.pop());

        std::vector<uint8_t> vector(msg->releaseMsg());
        std::vector<uint8_t> vmsg = { 12, 255, 252, 82, 1, 2, 146, 19, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5 };
        vmsg.insert(vmsg.end(), bigPayload.begin(), bigPayload.end());
        EXPECT_EQ(vector, vmsg);
        EXPECT_EQ(vector.size(), msg->getMsgSize());
    }

    TEST(FleXdIPCBuffer, Positive_Response_Funcion_rcvMsg_corrupted)
    {
        FleXdIPCBuffer buffer(0);
        pSharedArray8192 data(std::make_shared<std::array<uint8_t, 8192>>(std::array<uint8_t, 8192>{ 8, 0, 12, 252, 0, 98, 1, 2, 108, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 9, 8, 7, 6 }));

        buffer.rcvMsg(data, 26);

        pSharedFleXdIPCMsg msg(buffer.pop());

        std::vector<uint8_t> vector(msg->releaseMsg());
        std::vector<uint8_t> vmsg = { 12, 252, 0, 98, 1, 2, 108, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 9, 8, 7, 6 };
        EXPECT_EQ(vector, vmsg);
        EXPECT_EQ(vector.size(), msg->getMsgSize());
    }

    TEST(FleXdIPCBuffer, Positive_Response_Funcion_rcvMsg_corrupted_big)
    {
        FleXdIPCBuffer buffer(0);
        pSharedArray8192 data(std::make_shared<std::array<uint8_t, 8192>>(std::array<uint8_t, 8192>{ 8, 0, 12, 255, 252, 82, 1, 2, 146, 19, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5 }));
        uint32_t payloadIndex = 0;
        uint16_t i = 22;
        while (payloadIndex < UINT16_MAX - UINT8_MAX)
        {
            data->at(i) = payloadIndex % UINT8_MAX;
            i++;
            if (i >= 8192)
            {
                buffer.rcvMsg(data, 8192);
                i = 0;
            }
            payloadIndex++;
        }
        buffer.rcvMsg(data, i);

        pSharedFleXdIPCMsg msg(buffer.pop());

        std::vector<uint8_t> vector(msg->releaseMsg());
        std::vector<uint8_t> vmsg = { 12, 255, 252, 82, 1, 2, 146, 19, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5 };
        vmsg.insert(vmsg.end(), bigPayload.begin(), bigPayload.end());
        EXPECT_EQ(vector, vmsg);
        EXPECT_EQ(vector.size(), msg->getMsgSize());
    }

}
