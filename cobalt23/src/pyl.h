#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#ifndef LUAINPYTHON_H
#define LUAINPYTHON_H

typedef struct
{
    PyObject_HEAD
    int ref;
    int refiter;
} LuaObject;

extern PyTypeObject LuaObject_Type;

#define LuaObject_Check(op) PyObject_TypeCheck(op, &LuaObject_Type)

PyObject* LuaConvert(lua_State *L, int n);

extern lua_State *LuaState;

#if PY_MAJOR_VERSION < 3
#  define PyInit_lua initlua
#endif
PyMODINIT_FUNC PyInit_lua(void);

#endif
#ifdef __cplusplus
}
#endif
