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
 * File:   FleXdIPCMsgTest.cpp
 *
 * Author: Matus Bodorik
 *
 * Created on April 06, 2018, 10:02 AM
 */

#include "FleXdIPCMsg.h"
#include <gtest/gtest.h>
#include <stdint.h>
#include "BitStream.h"
#include "vector"
#include "CRC.h"

namespace {
    
    using FleXdIPCAdtHdr = flexd::icl::epoll::FleXdIPCAdtHdr;
    using FleXdIPCMsg = flexd::icl::epoll::FleXdIPCMsg;

    std::vector<uint8_t> payload = { 9, 8, 7, 6 };
    FleXdIPCAdtHdr* ahtemp = new FleXdIPCAdtHdr(0x3F, 1, 2, FleXdIPCMsg::calcCRC16(&payload[0], payload.size()), 3, 4, 5);
    FleXdIPCMsg msg(ahtemp, std::move(payload));

    TEST(FleXdIPCMsg, Positive_Response_Funcion_getHeaderParamType)
    {
        EXPECT_EQ(msg.getHeaderParamType(), 1);
    }

    TEST(FleXdIPCMsg, Positive_Response_Funcion_getHeaderParam)
    {
        EXPECT_EQ(msg.getHeaderParam(), 0x3F);
    }

    TEST(FleXdIPCMsg, Positive_Response_Funcion_getMsgSize)
    {
        EXPECT_EQ(msg.getMsgSize(), 24);
    }

    TEST(FleXdIPCMsg, Positive_Response_Funcion_getAdditionalHeader)
    {
        FleXdIPCAdtHdr* ah = msg.getAdditionalHeader();
        EXPECT_NE(ah, nullptr);
    }

    TEST(FleXdIPCMsg, Positive_Response_Funcion_getPayload)
    {
        std::vector<uint8_t> vector = msg.getPayload();
        ASSERT_EQ(vector.size(), 4);
        EXPECT_EQ(vector[0], 9);
        EXPECT_EQ(vector[1], 8);
        EXPECT_EQ(vector[2], 7);
        EXPECT_EQ(vector[3], 6);
    }

    TEST(FleXdIPCMsg, Positive_Response_Funcion_isValid)
    {
        EXPECT_TRUE(msg.isValid());
    }

    TEST(FleXdIPCMsg, Positive_Response_Funcion_releaseMsg)
    {
        std::vector<uint8_t> vector = msg.releaseMsg();
        std::vector<uint8_t> vmsg = { 12, 252, 0, 98, 1, 2, 108, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 9, 8, 7, 6 };
        EXPECT_EQ(vector, vmsg);
        EXPECT_EQ(vector.size(), msg.getMsgSize());
    }

    TEST(FleXdIPCMsg, Calculate_CRC16)
    {
        uint16_t crc16 = msg.getAdditionalHeader()->getValue_2();
        std::vector<uint8_t> vector = msg.releaseMsg();

        EXPECT_EQ(crc16, 0x6C00);
        EXPECT_EQ(vector.size(), msg.getMsgSize());
    }

    TEST(FleXdIPCMsg, Calculate_CRC_From_Msg)
    {
        EXPECT_EQ(FleXdIPCMsg::calcCRC16(&payload[0], payload.size()), 0x6C00);
    }

}
