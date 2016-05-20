#!/usr/bin/env python
# -*- coding: UTF-8 -*-

from distutils.core import setup, Extension
import os
import numpy as np

#os.environ["CC"] = "clang++"

ext_modules = [ Extension('pymcmc', 
    sources = ['pymcmc.cpp','mcmc.cpp','graph.cpp'], 
    extra_compile_args=['-std=c++14','-stdlib=libc++','-mmacosx-version-min=10.7'],
    extra_link_args=['-stdlib=libc++','-mmacosx-version-min=10.7'])]

setup(
        name = 'pymcmc',
        version = '1.0',
        include_dirs = [np.get_include()], #Add Include path of numpy
        ext_modules = ext_modules
      )
