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
 * Created on March 15, 2018, 14:02 PM
 */
#include "FleXdUDSClient.h"
#include "FleXdIPCMsg.h"
#include <iostream>

typedef flexd::ilc::epoll::FleXdIPCMsg  fleXdMsg;

int main(int argc, char** argv)
{
    flexd::ilc::epoll::FleXdEpoll poller(10);
    flexd::ilc::epoll::FleXdUDSClient client("/tmp/test", poller);
        
    uint16_t crc16 = 0;
    uint16_t msgSize = 134;
    uint8_t type = 1;
    uint16_t msgID = 555;
    uint64_t from = 9999;
    uint64_t to = 1111;
    uint32_t timeStamp = 444;
    uint32_t ttl = 600;
    std::vector<uint8_t> payload {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40
                                    ,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76
                                    ,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,105};
    
    crc16 = fleXdMsg::calkCRC16(&msgSize , sizeof(msgSize));
    crc16 = fleXdMsg::calkCRC16(&type , sizeof(type), crc16);
    crc16 = fleXdMsg::calkCRC16(&msgID , sizeof(msgID), crc16);
    crc16 = fleXdMsg::calkCRC16(&from , sizeof(from), crc16);
    crc16 = fleXdMsg::calkCRC16(&to , sizeof(to), crc16);
    crc16 = fleXdMsg::calkCRC16(&timeStamp , sizeof(timeStamp), crc16);
    crc16 = fleXdMsg::calkCRC16(&ttl , sizeof(ttl), crc16);
    crc16 = fleXdMsg::calkCRC16(&payload[0] , payload.size(), crc16);
    
    fleXdMsg msg(true, 0, msgSize, type, msgID,  from, to, timeStamp, ttl, std::move(payload));
    std::shared_ptr<fleXdMsg> msg_ptr = std::make_shared<fleXdMsg>(msg);
    
    
    uint16_t mcrc16 = 0;
    uint16_t mmsgSize = 33;
    uint8_t mtype = 1;
    uint16_t mmsgID = 2;
    uint64_t mfrom = 3;
    uint64_t mto = 4;
    uint32_t mtimeStamp = 5;
    uint32_t mttl = 6;
    std::vector<uint8_t> mpayload {7};/*,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40
                                    ,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76
                                    ,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102};*/
    
    mcrc16 = fleXdMsg::calkCRC16(&mmsgSize , sizeof(mmsgSize));
    mcrc16 = fleXdMsg::calkCRC16(&mtype , sizeof(mtype), mcrc16);
    mcrc16 = fleXdMsg::calkCRC16(&mmsgID , sizeof(mmsgID), mcrc16);
    mcrc16 = fleXdMsg::calkCRC16(&mfrom , sizeof(mfrom), mcrc16);
    mcrc16 = fleXdMsg::calkCRC16(&mto , sizeof(mto), mcrc16);
    mcrc16 = fleXdMsg::calkCRC16(&mtimeStamp , sizeof(mtimeStamp), mcrc16);
    mcrc16 = fleXdMsg::calkCRC16(&mttl , sizeof(mttl), mcrc16);
    mcrc16 = fleXdMsg::calkCRC16(&mpayload[0] , mpayload.size(), mcrc16);
    
    fleXdMsg mmsg(true, 0, mmsgSize, mtype, mmsgID,  mfrom, mto, mtimeStamp, mttl, std::move(mpayload));
    std::shared_ptr<fleXdMsg> mmsg_ptr = std::make_shared<fleXdMsg>(mmsg);
    client.init();

    client.sendMsg(msg_ptr);
    client.sendMsg(mmsg_ptr);
    client.sendMsg(mmsg_ptr);
    client.sendMsg(msg_ptr);
    
    poller.loop();
    while(true){}
    
    return 0;
}