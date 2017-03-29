#!/usr/bin/env python

# Copyright (c) 2009 - 2015, UChicago Argonne, LLC.
# See LICENSE file for details.


from setuptools import setup, find_packages
import os
import re
import sys
import versioneer

# pull in some definitions from the package's __init__.py file
sys.path.insert(0, os.path.join('src', ))
import blnuhr

long_description = open('README.rst', 'r').read()


setup (
    name             = blnuhr.__package_name__,        # blnuhr
    license          = blnuhr.__license__,
    version          = versioneer.get_version(),
    cmdclass         = versioneer.get_cmdclass(),
    description      = blnuhr.__description__,
    long_description = long_description,
    author           = blnuhr.__author_name__,
    author_email     = blnuhr.__author_email__,
    url              = blnuhr.__url__,
    download_url     = blnuhr.__download_url__,
    keywords         = blnuhr.__keywords__,
    install_requires = blnuhr.__requires__,
    platforms        = 'any',
    package_dir      = {'': 'src'},
    #packages         = find_packages(),
    packages         = [str(blnuhr.__package_name__), ],
    package_data     = dict(blnuhr=['resources/*', 'LICENSE', ]),
    classifiers      = blnuhr.__classifiers__,
    entry_points={
       # create & install console_scripts in <python>/bin
       #'console_scripts': ['blnuhr = blnuhr.main:main'],
       'gui_scripts': ['blnuhr = blnuhr:main'],
       },
      )
