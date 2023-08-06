# lxx/examples
Very useful examples to give you an idea of how to use lxx.

All these examples are in pure lxx with no external libraries or modules, and will work out of the box.

These items are in no particular order, But I will try to keep easier examples at the top.

## Get system specs
You could use the built in library `device` to get the system specs for all platforms, it provides a abstraction layer for all platforms.

```js
device.info().generalos // Returns "windows" or "unix". Some features on windows are not supported
device.info().sysname // Returns the system name, ex: Darwin. If this is being run on windows it will only return "Windows"
device.os // Like sysname
device.info().nodename // Returns the node name, ex: MacBook-Pro
```
## Get device specs
```js

var deviceinfo = device.info() // Returns a table with all the info

deviceinfo.sysname // Returns the system name, ex: Darwin
deviceinfo.nodename // Returns the node name, ex: MacBook-Pro.local
deviceinfo.generalos // Returns "windows" or "unix". Some features on windows are not supported
deviceinfo.release // Returns the release, ex: 20.6.0
deviceinfo.version // Returns the version, ex: Darwin Kernel Version 20.6.0: Mon Aug 30 06:12:21 PDT 2021; root:xnu-7195.141.6~3/RELEASE_X86_64
deviceinfo.machine // Returns the machine, ex: x86_64
```
## Count to 10
```js
for (i = 1, 10) {
    print(i)
}
```
## Print "Hello, World!"
```js
print("Hello, World!")
```
## Print "Hello, World!" with a variable
```js
var a = "Hello, World!"
print(a)
```
## FFI
```js
import("ffi")

ffi.cdef("int printf(const char *format, ...);")
ffi.C.printf("Hello %s!\n", "world")

ffi.os // Returns the OS name, ex: OSX
ffi.arch // Returns the architecture, ex: x86_64
```

## Memory
```js
// Allocate a block of memory
var address = core.memory.alloc(1024)

// Write data to the block of memory
core.memory.set(address, 1024, "Hello, world!")

// Read data from the block of memory
var data = core.memory.get(address, 1024)
print(data) // prints "Hello, world!", but may also print garbage data
print(data->sub(0, 12)) // print "Hello, world!" (the first 12 bytes) with no garbage data

// Free the block of memory
core.memory.free(address)

/***************************************************************************/
/* WARNING: Do not try the next line it will crash your program with a     /
"Segmentation fault: 11" error                                           */
core.memory.get(address, 1024)                                          //
/* since the memory is already freed, it is locked and cannot be        /
accessed leading to a crash */                                        //
/*********************************************************************/
```

## Color
```js
Color.new(255, 255, 255) // -> <color object>
Color.hex("#ffffff") // -> <color object>

/*
All color objects have 3 values, R, G, B. No matter how they are created internally they will be RGB.
*/
```
## Get memory usage
```js
// All of the following techniques are valid
// it is your prefrence on which one you use

// device.info()
device.info().scriptmemory // -> 117.74 MB // formats from kb to yb (crazy if you use that much memory)
device.info().scriptmemoryint // -> 31 (as an integer) // only returns kb

// collectgarbage()
collectgarbage("count") // -> 31 (as an integer) // only returns kb
```
## Access macros
```js
// Macros should only be fetched in the start of the program or be initialized in the start of the program
// Otherwise macros may take a big performance hit.
core.macros() // Returns a table of all macros; if this is the first time accessing macros, it will initialize them
core.macros() // Since you did this again it will happen instantly since it is initialized

core.macros("_WIN32") // This is a real C macro, it will return "1" if the OS is windows

// For example:
if (core.macros("_WIN32") == "1") {
    print("You are on windows!")
}else if(core.macros("__APPLE__") == "1") {
    print("You are on an Apple device!")
}else if(core.macros("__linux__") == "1") {
    print("You are on linux!")
}else {
    print("You are on an unknown device!")
}

core.macros()["__linux__"] // This WILL NOT work, you must use core.macros("__linux__") for speed and because
// of how the macros are structured internally this will not work

// All of these macros shown are real C macros that are used in the C preprocessor
// We do not reccomend using macros for fetching the OS or Device, use device.info(), device.os, or ffi.os instead
// There are hundreds of macros and all are fully supported in lxx
```

## Vector3
```js
var v = Vector3.new(1, 2, 3) // -> <vector3 object>
v.X // -> 1
v.Y // -> 2
v.Z // -> 3
```

## Vector2
```js
var v = Vector2.new(1, 2) // -> <vector2 object>
v.X // -> 1
v.Y // -> 2

var rv = Vector2.fromScreen(.5, 5, .5, 5, 100, 100) // -> <vector2 object>. Format like xscale, xoffset, yscale, yoffset, screen width, screen height
// if you are using lxx in a game engine screen width and screen height should be autofilled by the engine
```

## Transform
```js
var t = Transform.new() // -> <transform object>
t.X = 1 // X axis is now 1

var t2 = Transform.new(Vector3.new(10,10,10), Vector3.new(11,11,11), Vector3.new(12,12,12), Vector3.new(13,13,13)) // -> <transform object> 
// t2 will be located at 10,10,10, rotated at 11,11,11, scaled at 12,12,12, and pivoted at 13,13,13
t2.PY // -> 13 | PY means Pivot Y (Pivot Y axis)
```
## File control
lxx provides a File Control library buildin based on lfs.
```js
file.chdir("path") // Changes the current working directory to path
file.mkdir("path") // Creates a directory at path
file.rmdir("path") // Removes a directory at path
// .. and more
```
## Wait
```js
wait(1) // Allias for swait

swait(1) // Waits 1 second
mwait(1) // Waits 1 millisecond (1/1000 of a second)
uwait(1) // Waits 1 microsecond (1/1000000 of a second)

// These are very accurate and will not drift like Lua's wait function
// Do not do:
uwait(1000000) // This DOES NOT improve performance, it just makes it unreadable

/*swait/wait, mwait, uwait are interchangable because internally they all get waited by the microsecond*/

// Do:
swait(1) // Better!
/////

// Do not do:
for (i = 1, 60) {
    swait(1)
}

// Do:
swait(60) // Better!
```
## Enum
```js
// LXX enums are kindof like C enums
enum("myenum", {
    "a": 0,
    "b": 1,
    "c": 2
})

myenum.a // -> 0
myenum.b // -> 1
myenum.c // -> 2

// Example usage
enum("NPC_State", {
    "Idle": 0,
    "Walking": 1,
    "Running": 2,
    "Attacking": 3,
    "Dead": 4
})

var npc = NPC_State.Idle
if (npc == NPC_State.Idle) {
    print("NPC is idle")
}

// You could do this:
NPC_State = {
    "Idle": 0,
    "Walking": 1,
    "Running": 2,
    "Attacking": 3,
    "Dead": 4
}
// but that would be modifyable and is a table and the garbage collector will treat it like one.
```
## Arrays and Tables
```js
// Unlike in Lua, arrays and tables are different in lxx
// Tables have a key value, arrays dont
// Tables would be looped through using pairs, arrays would be looped through using ipairs
// They are different just for performance reasons and cleaner code
var myTable = {
    "a": 1, // Unlike in lua how you would define using
    // ["a"] = 1, // This is not valid in lxx and will create parser issues
}

var myArray = [1, 2, 3, 4, 5] // Arrays are defined using square brackets

for (i, v in pairs(myTable)) {
    print(i, v)
}

// You could use pairs on an array but it is a good practice to use ipairs
// pairs gives the key as the first value and the value as the second value
// ipairs gives the index as the first value and the value as the second value

for (i, v in ipairs(myArray)) {
    print(i, v)
}

// You can also use the # operator to get the length of an array
#myArray // -> 5
// But with tables
#myTable // -> 0, Since the index is a string it doesnt register that.
// But you can loop through tables.

// Tables and Arrays are stores in "similar" fashions but are different.
```
## new and types/typedef
```js
// new is kindof like Javascript's new keyword
// It creates a new instance of a class or object. 

// For example:
var v = Vector3.new(1, 2, 3) // Creates a new Vector3 object with the values 1, 2, 3

typedef("myType", {
    // typedef doesnt do anything right now, it just makes it easier to create a new type
    "x": 10,
    "myFunc": function() {
        print(this.x)
    }

    "new": function(z) {
        var new = this
        new.z = z
        return new // Duplicates itself and returns it
    }
})

var t = myType.new(50) // Creates a new instance of myType
t->myFunc() // -> 10
// Using -> is like using : in Lua, it passes the `this` variable (self in Lua) to the function
t.x = 20
t.z = 50 // Since the second argument to new was 50, z is now 50 because of the __new method

// All these changes will not affect the original type or any others made from it
myType.x // -> 10 // When making a class, and using itself it will be read-only
```
# try and xtry
```js
// try is like pcall in lua
// xtry is like xpcall in lua

try(function() {
    print("Hello, World!")
})

xtry(function() { // function to try
    print("Hello, World!")
}, function(err) { // error handler
    print("Error: " + err)
})
```
## Async
```js
// Async is a library that runs a function in a new thread
// These are real threads and not coroutines like in Lua.
// they use pthreads on unix and windows threads on windows 

var thread = async.new("swait(7); print('Hello, World from Thread 2!')") // Creates a new thread and runs the code in it
thread->start() // Starts the thread
// So we can have out own code here while the thread is running
var index = 0;
while (swait(1)) {
    print("Hello, World!")
    if index == 10 {
        break
    }
}

/*  OUTPUT
Hello, World!
Hello, World!
Hello, World!
Hello, World!
Hello, World!
Hello, World!
Hello, World!
Hello, World from Thread 2!
Hello, World!
Hello, World!
Hello, World!
*/
```

## Signals
```js
// Signals are a way to communicate between threads
// They are very useful for running multiple things at once
signal.new("test", function(){
    print("Hello, World!")
}) 
// Creates a new signal and when raised/called will print "Hello, World!"
signal.call("test") // Calls the signal "test" and will print "Hello, World!"
// Signals can be called from any thread
```
## Structs
```js
// Structs are a way to pack structures data into a single variable/string
// they are not like C structs, they are more like Python's struct module

struct.pack("ii", 1, 2) // Packs 1 and 2 into a struct
// -> �
// This is a binary string, it is not readable. That is not also the real result, it is just an example
struct.unpack("ii", "�") // Unpacks the binary string
// -> 1, 2
```
