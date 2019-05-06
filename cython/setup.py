"""setup.py: 

    This file is to be used by cmake.
"""
__author__           = "Dilawar Singh"
__copyright__        = "Copyright 2013, Dilawar Singh"
__license__          = "GPL"
__version__          = "1.0.0"
__maintainer__       = "Dilawar Singh"
__email__            = "dilawars@ncbs.res.in"
__status__           = "Development"

import sys
import os
from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

os.environ['CC']  = "/usr/bin/c++"
os.environ['CXX'] = "/usr/bin/c++"

extensions = [
        Extension("_tantrika", ["tantrika.pyx"],
            include_dirs = [ '/home1/dilawars/Work/GITHUB/DILAWAR/tantrika/external/install/include', '/usr/include/python3.7m' ],
            libraries = [ ],
            library_dirs = [ "/home1/dilawars/Work/GITHUB/DILAWAR/tantrika/_build" ],
            extra_compile_args = [ "-std=c++14" ],
            extra_objects=[ "/home1/dilawars/Work/GITHUB/DILAWAR/tantrika/_build/libtantrika.a" ]
            ),
        ]

setup(
        name = "tantrika",
        author= "Dilawar Singh <dilawars@ncbs.res.in>",
        ext_modules = cythonize(extensions
            , compiler_directives= {'language_level': 3}
            , build_dir="/home1/dilawars/Work/GITHUB/DILAWAR/tantrika/_build",
            ),
        )

