'''
Copyright (c) 2017-2022,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
'''
from argparse import ArgumentParser
from typing import List

from bindingGenerators.matlabBindingGenerator import MatlabBindingGenerator


def main(helicsHeaders: List[str]) -> None:
    """
    Main function for building MATLAB language bindings of the HELICS C library
    """
    generator = MatlabBindingGenerator(helicsHeaders)
    generator.generateSource()
            

if __name__ == '__main__':
    userInputParser = ArgumentParser()
    userInputParser.add_argument("headers", default=[], nargs="+",
        help="list of helics header files to parse")
    userArgs = userInputParser.parse_args()
    main(userArgs.headers)