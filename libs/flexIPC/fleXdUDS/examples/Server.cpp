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


#include "FleXdUDSServer.h"
#include <iostream>

class myUDSServer : public flexd::icl::epoll::FleXdUDSServer
{
    using fleXdAdtHdr = flexd::icl::epoll::FleXdIPCAdtHdr;
    using fleXdMsg = flexd::icl::epoll::FleXdIPCMsg;

public:
    explicit myUDSServer(const std::string& socPath, flexd::icl::epoll::FleXdEpoll& poller)
    : FleXdUDSServer(socPath, poller) {}
    virtual ~myUDSServer() {}

private:
    virtual void onClientConnect(int fd) override
    {
        std::vector<uint8_t> payload {69};
        std::shared_ptr<fleXdMsg> msg_ptr = std::make_shared<fleXdMsg>(std::move(payload));
        fleXdAdtHdr* adtHdr= msg_ptr->getAdditionalHeader();
        adtHdr->setValue_0(99);

        std::cout << "Client connected " << fd << std::endl;
        sendMsg(msg_ptr, fd);
    }
    virtual void onClientDisconnect(int fd) override
    {
        std::cout << "Client disconnected " << fd << std::endl;
    }
};

int main(int argc, char** argv)
{
    flexd::icl::epoll::FleXdEpoll poller(10);
    myUDSServer server("/tmp/test", poller);
    if (server.init())
    {
        std::cout << "FleXdUDSServer.init() successful" << std::endl;
        poller.loop();
    } else {
        std::cout << "FleXdUDSServer.init() failed" << std::endl;
    }
    server.init();

    return 0;
}
