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
 * File:   FleXdUDS.h
 * Author: Adrian Peniak
 *
 * Created on February 2, 2018, 1:51 PM
 */

#ifndef FLEXDUDS_H
#define FLEXDUDS_H

#include "FleXdIPC.h"
#include "FleXdEpoll.h"
#include "FleXdIPCMsg.h"

#define FLEXDIPCUDSPATH "/tmp/FleXd/shared/ipc/uds/"

namespace flexd {
    namespace icl {
        namespace ipc {

            class FleXdUDS : public FleXdIPC {
            public:
                explicit FleXdUDS(const std::string& socPath, FleXdEpoll& poller, FleXdIPC* proxy = nullptr);
                FleXdUDS(const FleXdUDS&) = delete;
                FleXdUDS& operator=(const FleXdUDS&) = delete;
                virtual ~FleXdUDS();

                /**
                 * Function initialize Unix domain sockets for Client.
                 * @return true if initialization is done, false otherwise.
                 */
                virtual bool init() override;
                /**
                 * Function send message to Server.
                 * @param msg - is shared pointer which contains attributes of FleXdIPCMsg
                 */
                virtual void sndMsg(pSharedFleXdIPCMsg msg, int fd) = 0;


            protected:
                virtual bool initUDS() = 0;
                virtual void rcvMsg(pSharedFleXdIPCMsg msg, int fd) = 0;
                virtual void readMsg(FleXdEpoll::Event e, std::array<uint8_t, 8192>&& array, int size) = 0;
                virtual bool reconnect(int fd) = 0;
                virtual int getFd() const override;
                bool connectUDS();
                bool listenUDS();

            protected:
                FleXdEpoll& m_poller;
                FleXdIPC* m_proxy;
                const std::string m_socPath;

            private:
                struct Ctx;
                std::unique_ptr<Ctx> m_ctx;
            };

        } // namespace ipc
    } // namespace icl
} // namespace flexd

#endif /* FLEXDUDS_H */

