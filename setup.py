#!/usr/bin/python

import sys
import os

if sys.version > '3':
    PY3 = True
else:
    PY3 = False

if PY3:
    import subprocess as commands
else:
    import commands
from distutils.core import setup, Extension
from distutils.sysconfig import get_python_lib, get_python_version

if os.path.isfile("MANIFEST"):
    os.unlink("MANIFEST")

COBALTVERSION = "23"

PYTHONVERSION = get_python_version()
PYLIBS = ["python" + get_python_version(), "pthread", "util"]
PYLIBDIR = [get_python_lib(standard_lib=True) + "/config"]
COBALTLIBS = ["cobalt" + COBALTVERSION]
COBALTLIBDIR = []

def pkgconfig(*packages):
    # map pkg-config output to kwargs for distutils.core.Extension
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}

    combined_pcoutput = ''
    for package in packages:
        (pcstatus, pcoutput) = commands.getstatusoutput(
            "pkg-config --libs --cflags %s" % package)
        if pcstatus == 0:
            combined_pcoutput += ' ' + pcoutput
        else:
            sys.exit("pkg-config failed for %s; "
                     "most recent output was:\n%s" %
                     (", ".join(packages), pcoutput))

    kwargs = {}
    for token in combined_pcoutput.split():
        if token[:2] in flag_map:
            kwargs.setdefault(flag_map.get(token[:2]), []).append(token[2:])
        else:                           # throw others to extra_link_args
            kwargs.setdefault('extra_link_args', []).append(token)

    if PY3:
        items = kwargs.items()
    else:
        items = kwargs.iteritems()
    for k, v in items:     # remove duplicated
        kwargs[k] = list(set(v))

    return kwargs

cobalt_pkgconfig = pkgconfig('cobalt' + COBALTVERSION, 'python-' + PYTHONVERSION)
cobalt_pkgconfig['extra_compile_args'] = ['-I/usr/include/cobalt'+COBALTVERSION]

setup(name="cobalt",
      version="23.0",
      description="Cobalt Programming Language",
      author="Cobalt Team",
      url="http://github.com/cobalt-lang/cobalt",
      license="MIT",
      long_description="""Cobalt is a programming language that aims to be
Lua turned into a lowlevel language, like C.""",
      ext_modules=[
        Extension("cobalt-python",
                  ["cobalt23/src/pyl.c", "cobalt23/src/lpy.c"],
                  **cobalt_pkgconfig),
        Extension("cobalt",
                  ["cobalt23/src/pyl.c", "cobalt23/src/lpy.c"],
                  **cobalt_pkgconfig),
        ],
      )
