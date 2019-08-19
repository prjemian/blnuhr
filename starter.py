#!/usr/bin/env python

"""
start the blnuhr program -- developer use only
"""


import os
import sys

# pull in some definitions from the package's __init__.py file
_path = os.path.dirname(__file__)
sys.path.insert(0, os.path.join(_path, 'src'))
from blnuhr import main

main.kopf()
