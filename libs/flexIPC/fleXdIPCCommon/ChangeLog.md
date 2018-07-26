# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]
## [0.1.0] - 2018-07-26
### Added
- New virtual functions isServer() and isClient() for FleXdIPC class [Martin Strenger]

### Changed
- None

### Removed
- None

## [0.1.0] - 2018-07-17
### Added
- None

### Changed
- Fixed segmentation fault when proxy was deleted on the run (example: switching from client to server) [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-28
### Added
- Unit tests for messages bigger than 8192 bytes [Martin Strenger]

### Changed
- Fixed finding non-corrupted message for messages bigger than 8192 bytes [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-18
### Added
- None

### Changed
- Small corrections [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-12
### Added
- Added FleXdIPCCommon.h/.cpp [Martin Strenger]

### Changed
- Implemented findings from review [Martin Strenger]
- Moved common functions into fleXdIPCCommon library (FleXdIPCCommon.h/.cpp) [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-06
### Added
- None

### Changed
- Updated CMakeLists.txt for unit tests to reflect name and code changes [Martin Strenger]
- Updated unit tests [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-05
### Added
- None

### Changed
- Renamed proxy functions connectIPC() to connnect() and disconnectIPC() to disconnect() [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-01
### Added
- Base class FleXdIPC [Adrian Peniak]
- Proxy FleXdIPCProxy and FleXdIPCProxyBuilder [Adrian Peniak]
- FleXdIPCConnector [Adrian Peniak]

### Changed
- Consolidation of namespaces for whole project [Adrian Peniak]

### Removed
- Move IPCBuffer to IPCCommon [Adrian Peniak]

## [0.0.1] - 2018-05-30
### Added
- New function releasePayload() for FleXdIPCMsg [Martin Strenger]

### Changed
- none

### Removed
- None

## [0.0.1] - 2018-05-29
### Added
- Unit tests for FleXdIPCBuffer

### Changed
- Rebuild FleXdIPCMsg and FleXdIPCBuffer from scratch [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-05-15
### Added
- Add function to calculate IPCMsg size [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-04-12
### Added
- Implementation FleXdLogger [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-03-20
### Added
- Implementation findNonCoruptedMessage() function at FleXdIPCFactory - function parse corrupted data and try to find next non corrupted message [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-02-29
### Added
- Initial version of FleXdIPCFactory [Matus Bodorik]
- Implementation of parseData() and releaseMsg() functions at FleXdIPCFactory - function parse received data and create FleXdIPCMsg [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-02-26
### Added
- Implementation of releaseMsg() function at FleXdIPCBuffer - lambda function called by FleXdIPCFactory when IPC message is read [Matus Bodorik]

### Changed
- function rcvMsg() - send data to FleXdIPCFactory [Matus Bodorik]

### Removed
- None

## [0.0.1] - 2018-02-21
### Added
- Implementation of calkCRC16() function at FleXdIPCMsg - static function for calculating CRC16 from data [Matus Bodorik]
- Implementation of releaseMsg() function at FleXdIPCMsg - Function create vector of bytes
  from atributes FleXdIPCMsg which is ready to send [Matus Bodorik]

### Changed
- Constructor of FleXdIPCMsg  [Matus Bodorik]

### Removed
- None
## [0.0.1] - 2018-02-02
### Added
- Initial version [Adrian Peniak]

### Changed
- None

### Removed
- None
