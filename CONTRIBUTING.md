# Contributing to HELICS

:+1: First off, thank you for taking the time to contribute! :+1:

The following is a set of guidelines for contributing to HELICS and associated projects. These are suggested guidelines. Use your best judgment.

## Licensing

HELICS is distributed under the terms of the BSD-3 clause license. All new
contributions must be made under this [LICENSE](LICENSE) in accordance with the GitHub [terms of service](https://help.github.com/en/articles/github-terms-of-service#6-contributions-under-repository-license), which uses inbound=outbound policy. By submitting a pull request you are acknowledging that you have the right to license your code under these terms.

## [Code of Conduct](.github/CODE_OF_CONDUCT.md)

If you just have a question check out [![Gitter chat](https://badges.gitter.im/GMLC-TDC/HELICS.png)](https://gitter.im/GMLC-TDC/HELICS)

and there are a few questions answered on the github [WIKI](https://github.com/GMLC-TDC/HELICS/wiki) page for HELICS.

## How Can I Contribute?

### Reporting Bugs

This section guides you through submitting a bug report for HELICS. Following these guidelines helps maintainers and the community understand your report, reproduce the behavior, and find related reports.

When you are creating a bug report, please include as many details as possible. Frequently, helpful information includes operating system, version, compiler used, API, interface, and some details about the function or operation being performed.

> **Note:** If you find a **Closed** issue that seems like it is the same thing that you're experiencing, open a new issue and include a link to the original issue in the body of your new one.

### Suggesting Enhancements

This section guides you through submitting a feature request, or enhancement for HELICS, including completely new features and minor improvements to existing functionality.

Please include as much detail as possible including the steps that you imagine you would take if the feature you're requesting existed, and the rational and reasoning of why this feature would be a good idea for a co-simulation framework.

A pull request including a bug fix or feature will always be welcome.

### Other repositories

There are a number of separate repositories included in the [GMLC-TDC organization](https://github.com/GMLC-TDC). Please feel free to explore those repositories for examples and additional tools, and to contribute.

### Your First Code Contribution

Unsure where to begin contributing to HELICS? You can start by looking for [`help wanted`](https://github.com/GMLC-TDC/HELICS/issues?utf8=%E2%9C%93&q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22) [`beginner`](https://github.com/GMLC-TDC/HELICS/issues?utf8=%E2%9C%93&q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22+label%3A%22beginner%22) issues:
Help with documentation and test cases is always welcome. Take a look at the [code coverage reports](https://codecov.io/gh/GMLC-TDC/HELICS) for some ideas on places that need more testing

### Submitting a pull request

Typically you would want to submit a pull request against the develop branch. That branch gets merged into master periodically but the develop branch is the active development branch where all the code is tested and merged before making a release. There is a [Pull request template](.github/PULL_REQUEST_TEMPLATE.md) that will guide you through the needed information. The pull requests are run through several automated checks in Travis and AppVeyor and for the most part must pass these tests before merging. The Codacy check is evaluated but not required as the checks are sometimes a bit aggressive.

## Styleguides

Code formatting is controlled via clang-format, a somewhat higher level style guide can be found [here](https://helics.readthedocs.io/en/latest/developer-guide/style.html). The code is not fully conformant to this yet but we are slowly working on it.

## Releasing a new HELICS version

Updating to a new HELICS release is manual. Each step below needs review, so it is not scripted.

1. Set `HelicsVersion` in `buildHelicsInterface.m` to the new version.
2. Regenerate the bindings from the new release's `include/helics/helics.h`:
   - `pip install cheader2json`
   - `python src/main.py <output-dir> <path-to-helics.h>`
   - Copy `matlabBindings/+helics` and `helicsMex.cpp` from the output into the repo.
   - Convert the copied files to LF line endings. The generator writes CRLF on Windows and the repo stores LF.
   - Review the diff. Check added or removed functions, constants, and property enums against the HELICS CHANGELOG and `docs/Public_API.md`.
3. Add a CHANGELOG entry and its link reference.
4. Add or update tests in `tests/` for any new API.
5. Build and test locally with `buildHelicsInterface('helics',true)` and `runAllTests`.
6. Open a pull request. CI builds and tests on Windows, macOS, and Linux. It also checks that the built library reports the version in `buildHelicsInterface.m`.
7. After merging, publish a GitHub release with a `v<version>` tag. The release job attaches the packages built by CI.
