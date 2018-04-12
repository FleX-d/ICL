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
 * File:   JsonObj.h
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
//#include "CRC.h"

namespace {
    
    std::vector<uint8_t> payload = {7};
    flexd::ilc::epoll::FleXdIPCMsg msg (false, 0, 33, 1, 2, 3, 4, 5, 6, std::move(payload)); 
    
    TEST(fleXdMsg, Positive_Response_Funcion_getCRC16)
    {
        EXPECT_EQ(msg.getCRC16(), 0);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_getMsgSize)
    {
        EXPECT_EQ(msg.getMsgSize(), 33);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_getMsgType)
    {
        EXPECT_EQ(msg.getMsgType(), 1);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_getMsgID)
    {
        EXPECT_EQ(msg.getMsgID(), 2);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_getFrom)
    {
        EXPECT_EQ(msg.getFrom(), 3);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_getTo)
    {
        EXPECT_EQ(msg.getTo(), 4);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_getTimeStamp)
    {
        EXPECT_EQ(msg.getTimeStamp(), 5);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_getTtl)
    {
        EXPECT_EQ(msg.getTtl(), 6);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_getPayload)
    {
        std::vector<uint8_t> vector = msg.getPayload();
        EXPECT_EQ(vector.front(), 7);
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_isComplete)
    {
        EXPECT_FALSE(msg.isComplete());
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_setComplete)
    {
        msg.setComplete(true);
        EXPECT_TRUE(msg.isComplete());
    }
    
    TEST(fleXdMsg, Positive_Response_Funcion_releaseMsg)
    {
        std::vector<uint8_t> vector = msg.releaseMsg();
        std::vector<uint8_t> msg = {8, 216, 228, 0, 132,4, 0, 8, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 20, 0, 0, 0, 26, 7 }; 
        EXPECT_EQ(vector, msg);
        EXPECT_EQ(vector.size(), msg.size());
    }
    
    TEST(fleXdMsg, Calculate_CRC16)
    {
        uint16_t crc16 = 0;
        uint16_t msgSize = 32;
        uint8_t type = 1;
        uint16_t msgID = 2;
        uint64_t from = 3;
        uint64_t to = 4;
        uint32_t timeStamp = 5;
        uint32_t ttl = 6;
        std::vector<uint8_t> payload {7};

        crc16 = CRC::Calculate(&msgSize , sizeof(msgSize), CRC::CRC_16_ARC());
        crc16 = CRC::Calculate(&type , sizeof(type), CRC::CRC_16_ARC(), crc16);
        crc16 = CRC::Calculate(&msgID , sizeof(msgID), CRC::CRC_16_ARC(), crc16);
        crc16 = CRC::Calculate(&from , sizeof(from), CRC::CRC_16_ARC(), crc16);
        crc16 = CRC::Calculate(&to , sizeof(to), CRC::CRC_16_ARC(), crc16);
        crc16 = CRC::Calculate(&timeStamp , sizeof(timeStamp), CRC::CRC_16_ARC(), crc16);
        crc16 = CRC::Calculate(&ttl , sizeof(ttl), CRC::CRC_16_ARC(), crc16);
        crc16 = CRC::Calculate(&payload[0] , payload.size(), CRC::CRC_16_ARC(), crc16);
        
        EXPECT_EQ(crc16, 36238); 
        EXPECT_EQ(vector.size(), msg.size());
    }
    
    TEST(fleXdMsg, Calculate_CRC_From_Msg)
    {
        uint16_t crc16 = msg.calculateCRC();
        EXPECT_EQ(crc16, 36238); 
    }
    
    
}