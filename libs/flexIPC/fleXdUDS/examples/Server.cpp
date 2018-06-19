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
 * File:   Server.cpp
 *
 * Author: Matus Bodorik
 *
 * Created on March 15, 2018, 14:32 PM
 */

#include <iostream>
#include "FleXdEpoll.h"
#include "FleXdIPCProxyBuilder.h"
#include "FleXdUDSServer.h"
#include "FleXdIPCMsg.h"

using namespace flexd::icl::ipc;

class myUDSServer : public FleXdIPCProxyBuilder<FleXdUDSServer>
{
public:
    explicit myUDSServer(const std::string& socPath, flexd::icl::ipc::FleXdEpoll& poller)
    : FleXdIPCProxyBuilder<FleXdUDSServer>(socPath, poller) {
        this->setOnConnectClient([this](int fd){ this->onConnectClient(fd); });
        this->setOnDisconnectClient([this](int fd){ this->onDisconnectClient(fd); });
        this->setOnSndMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onSndMsg(msg, fd); });
        this->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
    }
    virtual ~myUDSServer() = default;

private:
    void onConnectClient(int fd)
    {
        std::vector<uint8_t> payload {69};
        std::shared_ptr<FleXdIPCMsg> msg_ptr = std::make_shared<FleXdIPCMsg>(std::move(payload));
        FleXdIPCAdtHdr* adtHdr= msg_ptr->getAdditionalHeader();
        adtHdr->setValue_0(99);

        std::cout << "Client " << fd << " connected" << std::endl;
        sndMsg(msg_ptr, fd);
    }

    void onDisconnectClient(int fd)
    {
        std::cout << "Client " << fd << " disconnected" << std::endl;
    }

    void onSndMsg(pSharedFleXdIPCMsg msg, int fd)
    {
        std::cout << "Sending to client " << fd << ":";
        for (auto it = msg->getPayload().begin() ; it != msg->getPayload().end(); ++it)
            std::cout << ' ' << (int) *it;
        std::cout << std::endl;
    }

    void onRcvMsg(pSharedFleXdIPCMsg msg, int fd)
    {
        std::vector<uint8_t> payload {11,22,33,44,55,66,77,88,99};
        std::shared_ptr<FleXdIPCMsg> msg_ptr = std::make_shared<FleXdIPCMsg>(std::move(payload));
        FleXdIPCAdtHdr* adtHdr= msg_ptr->getAdditionalHeader();
        adtHdr->setValue_0(1);
        adtHdr->setValue_1(55);
        //adtHdr->setValue_2(); CRC already set through constructor
        adtHdr->setValue_3(444);
        adtHdr->setValue_4(9999);
        adtHdr->setValue_5(1111);

        std::cout << "Message from client " << fd << " received:";
        for (auto it = msg->getPayload().begin() ; it != msg->getPayload().end(); ++it)
            std::cout << ' ' << (int) *it;
        std::cout << std::endl;

        sndMsg(msg_ptr, fd);
    }
};

int main(int argc, char** argv)
{
    flexd::icl::ipc::FleXdEpoll poller(10);
    myUDSServer server("/tmp/test", poller);

    if (server.init())
    {
        std::cout << "Server is running" << std::endl;
        poller.loop();
    } else {
        std::cout << "Initialization failed" << std::endl;
    }
    return 0;
}
