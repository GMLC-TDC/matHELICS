# Changelog

All notable changes to this project will be documented in this file

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/).
This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [3.7.1][] ~ 2026-11-22

- update to HELICS [3.7.1](https://github.com/GMLC-TDC/HELICS/releases/tag/v3.7.1) release
- add `helicsBrokerIsRoot`, `helicsBrokerIsOpenToNewFederates`, and `helicsCoreIsOpenToNewFederates`
- add `HELICS_PROPERTY_INT_VALUE_BUFFER_WARNING` to `HelicsProperties`
- add `HELICS_MAX_TIME_VALUE`, `HELICS_TIME_BIGTIME`, `HELICS_TIME_TERMINATION`, and the `cHelicsMaxTimeValue` / `cHelicsTerminateTime` constants
- `HELICS_BIG_NUMBER` now returns 9223372000.0 (was 9223372036.854774); `HELICS_TIME_MAXTIME` is now defined from `HELICS_MAX_TIME_VALUE` and keeps its numeric value
- `helicsFederateRegisterGlobalInput` and `helicsFederateRegisterTypeInput` documentation now describes the returned input object

## [3.5.2][] ~ 2023-05-09

- update to HELICS [3.5.2](https://github.com/GMLC-TDC/HELICS/releases/tag/v3.5.2) release
- add some code for Octave support, see [Readme](https://github.com/GMLC-TDC/matHELICS/blob/main/README.md#building-with-octave)

## [3.4.0][] ~ 2023-01-24

- update to HELICS [3.4.0](https://github.com/GMLC-TDC/HELICS/releases/tag/v3.4.0) release
- update matlab docstrings on extra matlab codes

## [3.3.2][] ~ 2022-12-16

- update to HELICS [3.3.2](https://github.com/GMLC-TDC/HELICS/releases/tag/v3.3.2) release

## [3.2.1][] ~ 2022-06-03

Initial version to match [HELICS](www.github.com/GMLC-TDC/HELICS) versioning.

[3.2.1]: https://github.com/GMLC-TDC/matHELICS/releases/tag/v3.2.1
[3.3.2]: https://github.com/GMLC-TDC/matHELICS/releases/tag/v3.3.2
[3.4.0]: https://github.com/GMLC-TDC/matHELICS/releases/tag/v3.4.0
[3.5.2]: https://github.com/GMLC-TDC/matHELICS/releases/tag/v3.5.2
[3.7.1]: https://github.com/GMLC-TDC/matHELICS/releases/tag/v3.7.1