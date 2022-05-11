# HELICS Libclang Parser
This repository consists of a single python program, cheader_reader.py. 
This script is designed to parse the HELICS C API header files which currently
consist of helics.h and is designed to generate language specific bindings.
The current supported language conversions are Python and MATLAB.

## Installation and Requirements
### Linux
1. the libclang-11-dev package.
	`sudo apt install libclang-11-dev`
2. Please refer to the requirements.txt file for requred python packages.

### Mac
TODO

### Windows
TODO

## Usage
`python cheader_reader.py -p -m <full path to HELICS installation locaction>/include/helics/helics.h`

-p tells the script to generate the HELICS Python language bindings.

-m tells the script to generate the HELICS MATLAB bindings

### Output
- HELICSHeaderClangParseLog.log: a log file for troubleshooting and debugging purposes.
- capi.py: the HELICS Python language binding file to be used in pyhelics. This file is only generated if the -p option is provided.
- +helics: the folder containing all the .m functions for the MATLAB bindings. This folder is only generated if the -m option is provided.
- helicsMex.cpp: the mex file for the HELICS MATLAB bindings. This file is only generated if the -m option is provided.