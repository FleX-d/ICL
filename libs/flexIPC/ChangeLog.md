# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]
## [0.1.0] - 2018-06-18
### Added
- Generic interface for IPCConnector [Martin Strenger]

### Changed
- Various small corrections [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-12
### Added
- CMakeLists option to enable examples [Martin Strenger]
- New functionalities FleXdTimer, FleXdEvent, FleXdTermEvent, FleXdSignalHandler [Martin Strenger]
- Added FleXdIPCCommon.h/.cpp to fleXdIPCCommon library [Martin Strenger]

### Changed
- Implemented findings from review [Martin Strenger]
- Moved common functions into fleXdIPCCommon library (FleXdIPCCommon.h/.cpp) [Martin Strenger]
- Updated UDS server/client examples [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-06
### Added
- None

### Changed
- Updated fleXdUDS examples to reflect changes in IPC [Martin Strenger]
- Updated CMakeLists.txt for FleXdIPCCommon unit tests to reflect name and code changes [Martin Strenger]
- Updated unit tests for FleXdIPCCommon [Martin Strenger]

### Removed
- None

## [0.1.1] - 2018-06-05
### Added
- None

### Changed
- Fix FleXdIPCConnector handshake and extend examples [Adrian Peniak]

### Removed
- None

## [0.1.0] - 2018-06-05
### Added
- None

### Changed
- Renamed proxy functions connectIPC() to connnect() and disconnectIPC() to disconnect() [Martin Strenger]
- Consolidation of server/client proxy callbacks [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-01
### Added
- Draft for IPCConnector [Adrian Peniak]
- Base class FleXdIPC [Adrian Peniak]
- Proxy FleXdIPCProxy and FleXdIPCProxyBuilder [Adrian Peniak]
- Simple example of handshake between 3 App's for FleXdIPCConnector [Adrian Peniak]

### Changed
- Consolidation of namespaces for whole project [Adrian Peniak]
- Internal structure of naming of functions for FleXdUDS [Adrian Peniak]
- Add fd to FleXdIPCBuffer [Adrian Peniak]

### Removed
- Move IPCBuffer to IPCCommon [Adrian Peniak]
- Comment example for FleXdIPCUDS -> should be fixed  according new implementation [Adrian Peniak]

## [0.0.1] - 2018-04-19
### Added
- None

### Changed
- Fix review findings [Matus Bodorik]

### Removed
- None

## [0.0.1] - 2018-04-12
### Added
- Implementation FleXdLogger [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-04-05
### Added
- UnitTests for FleXdFactory, FleXdMessage, FleXdBuffer [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-04-05
### Added
- Inheritance at FleXdUDS directory, between FleXdUDSClient, FleXdUDSServer and FleXdUDS [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-03-20
### Added
- Function to parse corrupted data and try to find next non-corrupted message at class FleXdFactory [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-02-01
### Added
- Initial version [Adrian Peniak]

### Changed
- None

### Removed
- None
