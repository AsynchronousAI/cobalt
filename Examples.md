# luax/examples
Very useful examples to give you an idea of how to use luax.

All these examples are in pure luax with no external libraries or modules, and will work out of the box.
## Get system OS
```js
device.os // Returns the OS name, ex: OSX
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
import "ffi"

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
Color.rgb(255, 255, 255) // -> <color object>
Color.fromHex("#ffffff") // -> <color object>
Color.fromHex("#ffffff").R // -> 255
Color.fromHex("#ffffff").G // -> 255
Color.fromHex("#ffffff").B // -> 255
Color.hsv(50,25,23).R // -> 255 (Red)
/*
All color objects have 3 values, R, G, B. No matter how they are created internally they will be RGB.
*/
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
// if you are using luax in a game engine screen width and screen height should be autofilled by the engine
```

## Transform
```js
var t = Transform.new() // -> <transform object>
t.x = 1 // X axis is now 1

var t2 = Transform.new(Vector3.new(10,10,10), Vector3.new(11,11,11), Vector3.new(12,12,12), Vector3.new(13,13,13)) // -> <transform object> 
// t2 will be located at 10,10,10, rotated at 11,11,11, scaled at 12,12,12, and pivoted at 13,13,13
t2.py // -> 13
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