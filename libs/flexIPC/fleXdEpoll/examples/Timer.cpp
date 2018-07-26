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
 * File:   Timer.cpp
 *
 * Author: Martin Strenger
 *
 * Created on June 07, 2018, 15:19 PM
 */

#include "FleXdEpoll.h"
#include "FleXdEvent.h"
#include "FleXdTimer.h"
#include <iostream>

using namespace flexd::icl::ipc;

int main(int argc, char** argv)
{
    FleXdEpoll poller(10);
    FleXdTermEvent termEvent(poller);
    FleXdTimer timer1(poller, 0, 900 FLEXDTIMER_MSEC, true, []() { std::cout << "Periodic timer 1 expired" << std::endl; });
    FleXdTimer timer2(poller, 2, 0, true,  []() { std::cout << "Periodic timer 2 expired" << std::endl; });
    FleXdTimer timer3(poller, 2, 400 FLEXDTIMER_MSEC, false,  []() { std::cout << "Single shot timer expired" << std::endl; });
    FleXdTimer timer4(poller, 5, 0, false, [&]() { std::cout << "Program will be terminated" << std::endl; termEvent.trigger(); });
    if (termEvent.init())
    {
        std::cout << "Periodic timer 1 start " << (timer1.start() ? "successful" : "failed") << std::endl;
        std::cout << "Periodic timer 2 start " << (timer2.start() ? "successful" : "failed") << std::endl;
        std::cout << "Single shot timer start " << (timer3.start() ? "successful" : "failed") << std::endl;
        std::cout << "Termination timer start " << (timer4.start() ? "successful" : "failed") << std::endl;
        poller.loop();
    } else {
        std::cout << "FleXdTermEvent::init() failed" << std::endl;
    }
    return 0;
}
