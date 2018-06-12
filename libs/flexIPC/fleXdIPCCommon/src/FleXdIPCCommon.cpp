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
 * File:   FleXdIPCCommon.cpp
 * Author: Martin Strenger
 *
 * Created on June 08, 2018, 12:44 PM
 */

#include "FleXdIPCCommon.h"
#include <sys/stat.h>

namespace flexd {
    namespace icl {
        namespace ipc {

            bool checkIfFileExist(const std::string& filePath) {
                struct stat info;
                return (stat(filePath.c_str(), &info) == 0);
            }

            bool makeDir(const std::string& dirPath) {
                if (dirPath.empty()) return false;
                return (::mkdir(dirPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO | S_IXOTH) == 0);
            }

            std::string cutLastPathElement(const std::string& dirPath) {
                return dirPath.substr(0, dirPath.find_last_of("\\/"));
            }

            bool makeParentDir(const std::string& filePath) {
                std::string dir = cutLastPathElement(filePath);
                if(checkIfFileExist(dir)) return true;
                if(!checkIfFileExist(cutLastPathElement(dir))) makeParentDir(dir);
                return makeDir(dir);
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
