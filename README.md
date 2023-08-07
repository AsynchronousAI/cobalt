# cobalt
A programming language made in C and C++ made to be the *portable, simple, and modern* successor of C.

## Example code:
The following code is a snippet that shows off some of the genera features of cobalt.

Read [examples.md](/Examples.md) for more short examples that work. 
Check out [/cobalt-tests](/cobalt-1.0.0-tests/) for 100+ longer test examples.

```js
import("notareallibrary") // Not a real library, just an example

if (core.macros("__APPLE__") == "1"){ // Real C macros
    print("Seems that you are on an apple device");
}

print(device.os); // "windows" or "linux" or "darwin"
// This is a comment
var a = 1; // Type declarations are now optional
var b: number = 2; // This is a number, and will always be a number

for (i in range(0, 10)) {
    print(i);
}

var memoryaddress = core.memory.alloc(10); // Allocates 10 bytes of memory
var memoryaddress = core.memory.realloc(memoryaddress, 20); // Reallocates 20 bytes of memory
core.memory.free(memoryaddress); // Frees the memory

// Enums!
enum("myenum", {
    a = 1,
    b = 2,
    c = 3
})

print(myenum.a) // 1

var myarray = [1,2,3,4]
var mytable = {
    "a": 1,
    "b": 2,
    "c": 3
}

async.new("print('I am in thread 2')")->start(); // Real threads, using pthreads and windows threads
```

## Why?
C is a great language same for C++, but they dont appeal to the modern programmer. Cobalt is a language that is made from the groundwork of Lua & LuaJIT which are some of the fastest languages of all-time and we improved it
to have C like features like enums, memory, macros, and used ljs syntax to look like C rather than Lua's quirky
syntax.

## Speed?
Cobalt is as fast as Lua(JIT) which it is based upon. LuaJIT is considered to be as fast as C and C++ in some cases, and Rust in most cases. Cobalt has its own performance improvements over LuaJIT, so can be considered to
match C, C++, and Rust in nearly all cases.

## Features
- Direct memory access
- Type annotations and checking
- Real threads
- Ultra portable
- Real macros
- Enums
- Ultra fast
- Manual memory management
- Better syntax

## Why should I switch from what I am using?
Its not my choice to make but I believe regardless on what you are using cobalt should have something that applys to you, Cobalt is ultrafast and portable
since it is based on Lua, and LuaJIT source code. Not only that but it comes packaged with all the essentials
like memory access, typechecker, threads, and more. If you are using C/++ or Rust switching will give you
an overall better programming experience regardless if you are a beginner, a professional, or a hobbyist,
doing this as a job, or just for fun. Cobalt is a language that is made for everyone to give them
everything they need to make a great program with a great experience.

## [How to build and install](/build.md)
## [Credits](/COPYRIGHTS.md)
***

## TODO:
- FFI (far future) (alongside library installer)
- Batchfiles (far future)
- Make a better linter (far future)
- library installer

### LuaJIT compatibility
- Port libraries to JIT
- Overall sync JIT

#### Changes to add to JIT
- New libraries
- New functions:
```c
static int luaB_warn (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  lua_getglobal(L, "tostring");
  lua_getglobal(L, "print");
  for (i=1; i<=n; i++) {
    const char *s;
    size_t l;
    lua_pushvalue(L, -2);  /* function to be called */
    lua_pushvalue(L, i);   /* value to print */
    lua_call(L, 1, 1);
    lua_pushstring(L, yellow("warning: ", (const char *[]){"bold"}, 1));
    lua_insert(L, -2);
    lua_concat(L, 2);
    s = lua_tolstring(L, -1, &l);  /* get result */
    if (s == NULL)
      return luaL_error(L, "'tostring' must return a string to 'warn'");
    lua_call(L, 1, 0);
  }
  return 0;
}

static int luaB_info (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  lua_getglobal(L, "tostring");
  lua_getglobal(L, "print");
  for (i=1; i<=n; i++) {
    const char *s;
    size_t l;
    lua_pushvalue(L, -2);  /* function to be called */
    lua_pushvalue(L, i);   /* value to print */
    lua_call(L, 1, 1);
    lua_pushstring(L, blue("info: ", (const char *[]){"bold"}, 1));
    lua_insert(L, -2);
    lua_concat(L, 2);
    s = lua_tolstring(L, -1, &l);  /* get result */
    if (s == NULL)
      return luaL_error(L, "'tostring' must return a string to 'info'");
    lua_call(L, 1, 0);
  }
  return 0;
}

static int luaB_uwait(lua_State *L) {
  float m = luaL_optnumber(L, 1, 1);
  usleep(m);
  // returns a function that waits m without needing any arguments
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, m);
  lua_pushcclosure(L, luaB_uwait, 1);
  return 1;
}

static int luaB_wait(lua_State *L) {
  float seconds = luaL_optnumber(L, 1, 1);
  usleep(seconds * 1000000);
  // returns a function that waits m without needing any arguments
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, seconds * 1000000);
  lua_pushcclosure(L, luaB_uwait, 1);
  return 1;
}

static int luaB_mwait(lua_State *L) {
  float ms = luaL_optnumber(L, 1, 1);
  usleep(ms * 1000);
  // returns a function that waits m without needing any arguments
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushnumber(L, ms * 1000);
  lua_pushcclosure(L, luaB_uwait, 1);
  return 1;
}


static int luaB_slice(lua_State *L) {
  int nargs = lua_gettop(L);
  luaL_argcheck(L, nargs >= 2, 2, "expected at least 2 arguments");

  int start = luaL_checkinteger(L, 1);
  int stop = luaL_checkinteger(L, 2);
  int step = 1;

  if (nargs >= 3) {
    step = luaL_checkinteger(L, 3);
    luaL_argcheck(L, step != 0, 3, "step cannot be zero");
  }

  luaL_checktype(L, -1, LUA_TTABLE);
  int len = lua_rawlen(L, -1);

  if (start < 0) {
    start = len + start + 1;
  }

  if (stop < 0) {
    stop = len + stop + 1;
  }

  luaL_argcheck(L, start >= 1 && start <= len, 1, "start index out of range");
  luaL_argcheck(L, stop >= 1 && stop <= len, 2, "stop index out of range");

  if (step > 0) {
    luaL_argcheck(L, start <= stop, 2, "stop index must be greater than start index");
  } else {
    luaL_argcheck(L, start >= stop, 2, "stop index must be less than start index");
  }

  lua_newtable(L);
  int j = 1;

  for (int i = start; step > 0 ? i <= stop : i >= stop; i += step) {
    lua_rawgeti(L, -2, i);
    lua_rawseti(L, -2, j);
    j++;
  }

  return 1;
}


static int luaB_inputf(lua_State *L) {
  const char *prompt = luaL_optstring(L, 1, " ");
  const char *fmt = luaL_optstring(L, 2, "*l");
  int nargs = lua_gettop(L);
  luaL_argcheck(L, nargs <= 2, 2, "expected at most 2 arguments");
  luaL_argcheck(L, nargs >= 1, 1, "expected at least 1 argument");

  printf("%s", prompt);
  char buf[1024];
  fgets(buf, sizeof(buf), stdin);
  buf[strlen(buf) - 1] = '\0';

  if (strcmp(fmt, "*n") == 0) {
    lua_pushnumber(L, atof(buf));
  } else if (strcmp(fmt, "*a") == 0) {
    lua_pushstring(L, buf);
  } else if (strcmp(fmt, "*l") == 0) {
    lua_pushstring(L, buf);
  } else {
    return luaL_error(L, "invalid format");
  }

  return 1;
}

static int luaB_typedef(lua_State *L) {
  // typedef(name, table)
  // adds a new method to the table if it doesnt exist
  // saves the table in the global namespace
  // the new method returns an unlocked version of the table
  // otherwise the table will be read-only
  const char *name = luaL_checkstring(L, 1);
  luaL_checktype(L, 2, LUA_TTABLE);

  // Check that the name isnt already taken
  lua_getglobal(L, name);
  if (!lua_isnil(L, -1)) {
    return luaL_error(L, "type name '%s' is already taken", name);
  }
  lua_pop(L, 1);
  // Duplicate the inputted table and save it globally
  lua_pushvalue(L, 2);
  lua_setglobal(L, name);

  return 1;
}
static int luaB_range(lua_State *L) {
  // range(start, stop, step)
  // returns a table of numbers from start to stop (inclusive) with step
  // if step is not provided, it defaults to 1
  int nargs = lua_gettop(L);
  luaL_argcheck(L, nargs >= 2, 2, "expected at least 2 arguments");
  luaL_argcheck(L, nargs <= 3, 3, "expected at most 3 arguments");

  int start = luaL_checkinteger(L, 1);
  int stop = luaL_checkinteger(L, 2);
  int step = 1;

  if (nargs >= 3) {
    step = luaL_checkinteger(L, 3);
    luaL_argcheck(L, step != 0, 3, "step cannot be zero");
  }

  if (start < 0) {
    start = stop + start + 1;
  }

  if (stop < 0) {
    stop = stop + stop + 1;
  }

  luaL_argcheck(L, start >= 1 && start <= stop, 1, "start index out of range");
  luaL_argcheck(L, stop >= 1 && stop <= stop, 2, "stop index out of range");

  lua_newtable(L);
  int j = 1;

  for (int i = start; i <= stop; i += step) {
    lua_pushinteger(L, i);
    lua_rawseti(L, -2, j);
    j++;
  }

  return 1;
}
static int newenum(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);

    // Check that the name isnt already taken
    lua_getglobal(L, name);
    if (!lua_isnil(L, -1)) {
        return luaL_error(L, "enum name '%s' is already taken", name);
    }
    lua_pop(L, 1);
    // Create the enum table
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, name);
    // Add the enum values
    lua_pushnil(L);
    while (lua_next(L, 2) != 0) {
        lua_pushvalue(L, -2);
        lua_insert(L, -2);
        lua_settable(L, -4);
    }
    return 1;
}
```
