'''
Copyright (c) 2017-2022,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
'''
import os
from argparse import ArgumentParser
from typing import List

from bindingGenerators.matlabBindingGenerator import MatlabBindingGenerator


def main(bindingsRootDir: str, helicsHeaders: List[str]) -> None:
    """
    Main function for building MATLAB language bindings of the HELICS C library
    """
    generator = MatlabBindingGenerator(bindingsRootDir, helicsHeaders)
    generator.generateSource()
            

if __name__ == '__main__':
    userInputParser = ArgumentParser()
    userInputParser.add_argument("bindingsRootDir", nargs="?", default=os.getcwd(),
        help="location to generate matlab language bindings.")
    userInputParser.add_argument("headers", default=[], nargs="+",
        help="list of helics header files to parse")
    userArgs = userInputParser.parse_args()
    print(userArgs.bindingsRootDir)
    print(userArgs.headers)
    main(userArgs.bindingsRootDir, userArgs.headers)