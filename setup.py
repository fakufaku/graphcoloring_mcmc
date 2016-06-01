#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#----------------------------------------------------------------------------
#"THE BEER-WARE LICENSE" (Revision 42):
#Robin, Gilles, Elie wrote this file.  As long as you retain this notice you
#can do whatever you want with this stuff. If we meet some day, and you think
#this stuff is worth it, you can buy me a beer in return.   Robin, Gilles, Elie
#----------------------------------------------------------------------------

from distutils.core import setup, Extension
import os
import platform
import numpy as np

if platform.system() == 'Darwin':
    CCFLAGS = ['-std=c++14','-stdlib=libc++','-mmacosx-version-min=10.7']
    LDFLAGS = ['-stdlib=libc++','-mmacosx-version-min=10.7']
else:
    CCFLAGS = ['-std=c++1y']
    LDFLAGS = []
#os.environ["CC"] = "clang++"

ext_modules = [ Extension('pymcmc', 
    sources = ['pymcmc.cpp','mcmc.cpp','graph.cpp'], 
    extra_compile_args=CCFLAGS,
    extra_link_args=LDFLAGS)]

setup(
        name = 'pymcmc',
        version = '1.0',
        include_dirs = [np.get_include()], #Add Include path of numpy
        ext_modules = ext_modules
      )
