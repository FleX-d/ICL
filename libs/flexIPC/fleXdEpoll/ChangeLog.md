# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]
## [0.1.0] - 2018-07-27
### Added
- None

### Changed
- Changed FleXdEvent to be able to trigger event with custom values [Martin Strenger]
- Updated examples for FleXdEvent [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-07-26
### Added
- None

### Changed
- Updated examples for FleXdTimer [Martin Strenger]

### Removed
- Removed FleXdSignalHandler class (singleton) [Martin Strenger]

## [0.1.0] - 2018-07-17
### Added
- New setOn... functions for FleXdEvent and FleXdTimer [Martin Strenger]
- Added FleXdEvent::uninit() function [Martin Strenger]

### Changed
- Fixed missing file descriptor closing in FleXdTimer::stop() function [Martin Strenger]
- Fixed behaviour for multiple calling of FleXdEvent::init() function [Martin Strenger]
- Fixed behaviour for multiple calling of FleXdTimer::start() and FleXdTimer::stop() functions [Martin Strenger]
- Fixed epoll_wait deadlock [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-12
### Added
- New functionalities FleXdTimer, FleXdEvent, FleXdTermEvent, FleXdSignalHandler [Martin Strenger]

### Changed
- Implemented findings from review [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-01
### Added
- None

### Changed
- Consolidation of namespaces for whole project [Adrian Peniak]

### Removed
- None

## [0.0.1] - 2018-04-12
### Added
- Implementation FleXdLogger [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.1.0] - 2018-06-04
### Added
- None

### Changed
- Consolidation of namespaces for whole project [Adrian Peniak]

### Removed
- FleXdLogger [Adrian Peniak]
