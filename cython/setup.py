"""setup.pyx: 

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

os.environ['CC'] = "g++"
os.environ['CXX'] = "g++"

extensions = [
        Extension("synpase", ["synapse.pyx"],
            include_dirs = [ '.' ],
            libraries = [],
            library_dirs = []
            ),
        ]

setup(
    name = "tantrika",
    ext_modules = cythonize(extensions
        , compiler_directives= {'language_level': 3}
        ),
    )

