# HELICS Matlab interface
This repository contains the tools and files required for using HELICS with Matlab.
For HELICS documentation see [HELICS.org](www.helics.org)

## Installation and Requirements
For most platforms the installation is as simple as downloading the repository and inside matlab executing the buildHelicsInterface Matlab script.

This should download the requried HELICS binary files and compile the platform specific mex file.  This does require a compiler be installed on the system and executing

``` matlab
>> mex -setup
``` 

inside matlab prior to running the script.  

We anticipate making available some prebuilt mex files for even easier install in the near future for specific platforms.  

## Requirements
Matlab 2018a or greater.  
intalled compatible compiler to build the mex file

### building with custom HELICS version
If you want to build with a custom HELICS version see [lib clang parser](./docs/libClangParser.md) for the code generation instructions

Note:  just using with an updated HELICS library only requires changing the helics.[dll|so|dylib] file and no other modification is necessary. 