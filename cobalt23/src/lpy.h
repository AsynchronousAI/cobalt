/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#ifndef PYTHONINLUA_H
#define PYTHONINLUA_H
#define POBJECT "Python Object"

#if PY_MAJOR_VERSION < 3
  #define PyBytes_Check           PyString_Check
  #define PyBytes_AsStringAndSize PyString_AsStringAndSize
#endif

int py_convert(lua_State *L, PyObject *o);

typedef struct
{
    PyObject *o;
    int asindx;
} py_object;

py_object*    luaPy_to_pobject(lua_State *L, int n);
#include "lualib.h"

#endif