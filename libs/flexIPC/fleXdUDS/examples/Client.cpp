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
 * File:   Client.cpp
 *
 * Author: Matus Bodorik
 *
 * Created on March 15, 2018, 14:02 PM
 */

#include <iostream>
#include "FleXdEpoll.h"
#include "FleXdIPCProxyBuilder.h"
#include "FleXdUDSClient.h"
#include "FleXdIPCMsg.h"

using namespace flexd::icl::ipc;

class myUDSClient : public FleXdIPCProxyBuilder<FleXdUDSClient>
{
public:
    explicit myUDSClient(const std::string& socPath, flexd::icl::ipc::FleXdEpoll& poller)
    : FleXdIPCProxyBuilder<FleXdUDSClient>(socPath, poller) {
        this->setOnConnect([this](bool ret){ this->onConnect(ret); });
        this->setOnDisconnect([this](bool ret){ this->onDisconnect(ret); });
        this->setOnSndMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onSndMsg(msg, fd); });
        this->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
    }
    virtual ~myUDSClient() = default;

private:
    void onConnect(bool ret)
    {
        if (ret)
        {
            std::cout << "Connected to server" << std::endl;
        } else {
            std::cout << "Cannot connect to server" << std::endl;
        }
    }

    void onDisconnect(bool ret)
    {
        std::cout << "Disconnected from the server" << std::endl;
    }

    void onSndMsg(pSharedFleXdIPCMsg msg, int fd)
    {
        std::cout << "Sending:";
        for (auto it = msg->getPayload().begin() ; it != msg->getPayload().end(); ++it)
            std::cout << ' ' << (int) *it;
        std::cout << std::endl;
    }

    void onRcvMsg(pSharedFleXdIPCMsg msg, int fd)
    {
        std::cout << "Message received:";
        for (auto it = msg->getPayload().begin() ; it != msg->getPayload().end(); ++it)
            std::cout << ' ' << (int) *it;
        std::cout << std::endl;
    }
};

int main(int argc, char** argv)
{
    flexd::icl::ipc::FleXdEpoll poller(10);
    myUDSClient client("/tmp/test", poller);

    std::vector<uint8_t> payload {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40
                                    ,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76
                                    ,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,105};
    std::shared_ptr<FleXdIPCMsg> msg_ptr = std::make_shared<FleXdIPCMsg>(std::move(payload));
    FleXdIPCAdtHdr* adtHdr= msg_ptr->getAdditionalHeader();
    adtHdr->setValue_0(1);
    adtHdr->setValue_1(55);
    //adtHdr->setValue_2(); CRC already set through constructor
    adtHdr->setValue_3(444);
    adtHdr->setValue_4(9999);
    adtHdr->setValue_5(1111);

    std::vector<uint8_t> mpayload {7};
    std::shared_ptr<FleXdIPCMsg> mmsg_ptr = std::make_shared<FleXdIPCMsg>(std::move(mpayload));
    FleXdIPCAdtHdr* madtHdr= mmsg_ptr->getAdditionalHeader();
    madtHdr->setValue_0(1);
    madtHdr->setValue_1(2);
    //madtHdr->setValue_2(); CRC already set through constructor
    madtHdr->setValue_3(5);
    madtHdr->setValue_4(3);
    madtHdr->setValue_5(4);

    if (client.init())
    {
        client.sndMsg(msg_ptr);
        client.sndMsg(mmsg_ptr);
        client.sndMsg(mmsg_ptr);
        client.sndMsg(msg_ptr);

        poller.loop();
    }
    return 0;
}
