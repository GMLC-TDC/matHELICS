# HELICS Matlab interface
This repository contains the tools and files required for using HELICS with Matlab.
For HELICS documentation see [HELICS.org](www.helics.org)

## Package Download

For systems running standard newer windows or mac(non arm based) and a newer Matlab (2021a+) there is a zip or tar.gz file
with the entire system that can be downloaded and unziped. The zip file contains a helics folder that should be added to the matlab path(not with subfolders).
These archives can be found attached to the releases. 
There is also a release that should work with ubuntu 20.04,  It is unclear if this mex file will work on other linux platforms.

## Installation and Requirements
For other platforms the installation is as simple as downloading the repository and inside matlab executing the buildHelicsInterface Matlab script.

This should download the requried HELICS binary files and compile the platform specific mex file.  This does require a compiler be installed on the system and executing

``` matlab
>> mex -setup
```

inside matlab prior to running the script.  

```matlab
>> cd matHELICS
>> buildHelicsInterface('../helics');
```

this will generate the helics interface in a folder 1 level above the matHELICS directory.  If desired it can also be build in the matHELICS directory 

```matlab
>> cd matHELICS
>> buildHelicsInterface();
```

The `buildHelicsInterface` function also has an option to generate the package file.  This can be used to generate a package that can be transfered to a similar system.

### Requirements
Matlab 2018a or greater.  
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

## Building with older versions of Matlab.
If it is necessary to run on an older version of Matlab prior to 2018a, some minor modifications to the helicsMex.cpp may be possible to allow things to work fine.

Versions prior 2015 will require more extensive modifications.  And may be better served by using the Swig based matlab build from Helics version 3.2 which should be compatible with other HELICS versions in the 3.X series.


## Source Repo

The matHelics source code is hosted on GitHub: [https://github.com/GMLC-TDC/matHelics](https://github.com/GMLC-TDC/matHELICS)

## Release
HELICS, matHELICS and many associated repositories are distributed under the terms of the BSD-3 clause license. All new
contributions must be made under this license. [LICENSE](LICENSE)