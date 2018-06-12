/*
Copyright (c) 2018, Globallogic s.r.o.
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
 * File:   Event.cpp
 *
 * Author: Martin Strenger
 *
 * Created on June 08, 2018, 10:22 AM
 */

#include "FleXdEpoll.h"
#include "FleXdEvent.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace flexd::icl::ipc;

void onEvent()
{
    std::cout << "[pid " << ::getpid() << "] Event triggered" << std::endl;
}

// void onSignal()
// {
//     std::cout << "Signal triggered" << std::endl;
//     exit(1);
// }

int main(int argc, char** argv)
{
    FleXdEpoll poller(10);
    FleXdEvent event(poller, onEvent);
    FleXdTermEvent termEvent(poller);

    if (termEvent.init()) {
        std::cout << "FleXdSigEvent.init() successful" << std::endl;
    } else {
        std::cout << "FleXdSigEvent.init() failed" << std::endl;
    }

    if (event.init())
    {
        std::cout << "FleXdEvent.init() successful" << std::endl;
        switch (::fork()) {
            case -1:
                std::cout << "fork() failed" << std::endl;
                break;
            case 0:
                std::cout << "[pid " << ::getpid() << "] sleep(2)" << std::endl;
                ::sleep(2);
                if (event.trigger()) {
                    std::cout << "[pid " << ::getpid() << "] Trigger event" << std::endl;
                } else {
                    std::cout << "[pid " << ::getpid() << "] Failed to trigger event" << std::endl;
                }
                ::exit(EXIT_SUCCESS);
            default:
                std::cout << "[pid " << ::getpid() << "] poller.loop()" << std::endl;
                poller.loop();
                break;
        }
    } else {
        std::cout << "FleXdEvent.init() failed" << std::endl;
    }
    return 0;
}
