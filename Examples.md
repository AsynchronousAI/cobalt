# luax/examples
Very useful examples to give you an idea of how to use luax.

All these examples are in pure luax with no external libraries or modules.
## Get system OS
```js
ffi.os // Returns the OS name, ex: OSX
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

## USB
```js
var usb = device.usb
var ctx = usb.init()
var devices = ctx:get_device_list()

for (i, dev in ipairs(devices)) {
   var descr = dev->get_device_descriptor()
   var devhandle = dev->open()
   print(string.format("USB %s - bus:%d port:%d %.4x:%.4x %s %s (%s)",
      descr.usb_version, dev->get_bus_number(), dev->get_port_number(),
      descr.vendor_id, descr.product_id,
      devhandle->get_string_descriptor(descr.manufacturer_index) || "???",
      devhandle->get_string_descriptor(descr.product_index) || "???",
      descr.class))
   devhandle:close()
   dev:free()
}
```

## Memory
```js
core.alloc(1024) // Allocates 1024 bytes of memory
core.get({}) // 0x0, gets the address of the object
core.set({}, "0x0") // Sets the address of the object to 0x0
core.free(core.get({})) // Frees the memory of the object
core.fetch("0x0") // Fetches the object at 0x0
```

## FFI
Like LuaJIT's FFI.
```js
ffi.cdef("int printf(const char *format, ...);")
ffi.C.printf("Hello %s!\n", "world") //-> Hello world!, Return value: 13
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

## Audio
```js
var a = Audio.new("path/to/file.wav") // -> <audio object>
a->play() // Plays the audio
a->pause() // Pauses the audio
a->stop() // Stops the audio

a.Volume = 1 // Sets the volume to 1
a.Loop = true

a.Position = Vector3.new(1, 2, 3) // Sets the position of the audio to 1, 2, 3
// If the user has 3d audio enabled the audio will be heard as if it was being played from 1, 2, 3
```

## Wait
```js
wait(1) // Allias for swait

swait(1) // Waits 1 second
mwait(1) // Waits 1 millisecond (1/1000 of a second)
uwait(1) // Waits 1 microsecond (1/1000000 of a second)

// These are very accurate and will not drift like Lua's wait function
// Do not do:
uwait(1000000) // This DOES NOT improve performance, it just makes it slower and unreadable

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