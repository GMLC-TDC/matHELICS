# HELICS Matlab interface

This repository contains the tools and files required for using HELICS with Matlab.
For HELICS documentation see [HELICS.org](www.helics.org)

## Package Download

For systems running standard newer windows or mac(non arm based) and a newer Matlab (2021a+) there is a zip or tar.gz file
with the entire system that can be downloaded and unzipped. The zip file contains a helics folder that should be added to the matlab path(not with subfolders).
These archives can be found attached to the releases.
There is also a release that should work with ubuntu 20.04,  It is unclear if this mex file will work on other linux platforms.

## Installation and Requirements

For other platforms the installation is as simple as downloading this repository and inside matlab executing the `buildHelicsInterface` Matlab script.

This should download the required HELICS binary files and compile the platform specific mex file.  This does require a compiler be installed on the system and executing inside matlab

``` matlab
>> mex -setup
```

This setup is only once on any given Matlab installation and does not need to be repeated when upgrading HELICS 

```matlab
>> cd matHELICS
>> buildHelicsInterface('../helics');
```

this will generate the helics interface in a folder 1 level above the matHELICS directory.  If desired it can also be built in the matHELICS directory for example:

```matlab
>> cd matHELICS
>> buildHelicsInterface();
```

The `buildHelicsInterface` function also has an option to generate the package file.  This can be used to generate a package that can be transfered to a similar system.

This is a new process for building a Matlab interface, it is tested but not extensively and not on a large variety of platforms if you run into issues please let us know.

#### Building with Octave

The instructions for building with newer octave are identical.  This will work for octave 8.4 or newer.   Octave 9.1 on Windows does not work directly due to a bug with mingw 13.2 on windows.   This is not an issue with Octave, so if a helics version was build with mingw it could be made to work, but the scripts use the visual studio version, which works fine with older versions of the library and likely subsequent ones as well as the bug is reportedly fixed.   But this will likely require a new version of octave with a newer mingw release.  

### Requirements

Matlab 2018a or greater.  Octave 8.4 or greater.
intalled compatible compiler to build the mex file

### building with custom HELICS version

If you want to build with a custom HELICS version see [lib clang parser](./docs/libClangParser.md) for the code generation instructions

Note:  just using with an updated HELICS library only requires changing the helics.[dll|so|dylib] file and no other modification is necessary.
Updating a version with modified matlab files would require updating the matHelics repository via git and rerunning buildHelicsInterface.

## Using the library

On mac or linux it is necessary to run helicsStartup prior to use.  After that all the helics functions are available in the `helics` matlab package

```matlab
>> helics.helicsGetVersion()

```

## Building with older versions of Matlab

If it is necessary to run on an older version of Matlab prior to 2018a, some minor modifications to the helicsMex.cpp may be possible to allow things to work fine, please contact the developers for specific instructions.

Versions prior to Matlab 2015 will require more extensive modifications.  And may be better served by using the Swig based matlab build from Helics version 3.2 which should be compatible with other HELICS versions in the 3.X series.

### Older versions of octave

Older versions of octave can use the SWIG generate interface.  This version will be deprecated as of HELICS 3.6, but HELICS is generally backwards compatible for federates, that don't use the latest features.  

## Source Repo

The matHelics source code is hosted on GitHub: [https://github.com/GMLC-TDC/matHelics](https://github.com/GMLC-TDC/matHELICS)

## Citation
General citation for HELICS:
T. Hardy, B. Palmintier, P. Top, D. Krishnamurthy and J. Fuller, "HELICS: A Co-Simulation Framework for Scalable Multi-Domain Modeling and Analysis," in IEEE Access, doi: 10.1109/ACCESS.2024.3363615, available at [https://ieeexplore.ieee.org/document/10424422](https://ieeexplore.ieee.org/document/10424422/

## Release

HELICS, matHELICS and many associated repositories are distributed under the terms of the BSD-3 clause license. All new
contributions must be made under this license. [LICENSE](LICENSE)