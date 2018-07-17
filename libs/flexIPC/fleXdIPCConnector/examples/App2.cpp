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
 * File:   App2.cpp
 * Author: Adrian Peniak
 *
 * Created on May 31, 2018, 8:04 AM
 */

#include "App.h"

int main(int argc, char** argv) {
    flexd::icl::ipc::FleXdEpoll poller(10);
    flexd::icl::ipc::FleXdTermEvent event(poller);
    std::cout << "***Start app 101" << "\n";
    if(event.init()) {
        App a(101, poller, false, argc > 1);
        std::cout << "***addPeer 100 " << a.addPeer(100) << "\n";
        std::cout << "***addPeer 102 " << a.addPeer(102) << "\n";
        std::cout << "***addPeer 111 " << a.addPeer(111) << "\n";
        a.send(100, "Testing msg from 101 to 100");
        a.send(102, "Testing msg from 101 to 102");
        poller.loop();
    }
    return 0;
}
