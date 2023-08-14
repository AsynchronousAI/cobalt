# Building for other platforms
> **Known issues:**
> - Unsupported platforms
> - Interpreter is not available for some platforms
> - Consoles will have issues when being compiled on posix systems 
> - Raptor is ready to be supported but will need some work
# JIT Consoles
## Building for console
### XBOX One
XBOX One has the JIT available, but you will need to build it yourself. Currently the normal
interpreter is not available for XBOX One. 
#### Building
```bash
$ cmake .
$ make jit-xbox
```

### XBOX X
> NOTE: XBOX Series X support is not yet available, but it will be soon.
XBOX Series X has the JIT available, but you will need to build it yourself. Currently the normal
interpreter is not available for XBOX Series X. 
#### Building
```bash
$ cmake .
$ make jit-xbox-x
```

### Nintendo Switch
Nintendo Switch has the JIT available, but you will need to build it yourself. Currently the normal
interpreter is not available for Nintendo Switch.
#### Building
```bash
$ cmake .
$ make jit-switch
```

### Playstation 4
Playstation 4 have the JIT available, but you will need to build it yourself. Currently the normal
interpreter is not available for Playstation 4.
#### Building
```bash
$ cmake .
$ make jit-ps4
```

### Playstation 5
Playstation 5 have the JIT available, but you will need to build it yourself. Currently the normal
interpreter is not available for Playstation 5.
#### Building
```bash
$ cmake .
$ make jit-ps5
```

### Playstation Vita
Playstation Vita have the JIT available, but you will need to build it yourself. Currently the normal
interpreter is not available for Playstation Vita.
#### Building
```bash
$ cmake .
$ make jit-vita
```

### Xbox 360
Xbox 360 have the JIT available, but you will need to build it yourself. Currently the normal
interpreter is not available for Xbox 360.
#### Building
```bash
$ cmake .
$ make jit-xbox360
```
## Finding build files for console
- JIT: `cobalt23/jit/src/cobaltjit(.exe)`
- Compiler: `cobalt23/cobaltc(.exe)`
- Interpreter: `cobalt23/cobalt(.exe)`
# Apple Platforms
## iOS, iPadOS, tvOS, watchOS, xrOS
Use the Swift - Cobalt bridge to use Cobalt on these plartforms. [Swift - Cobalt](https://github.com/cobalt-lang/swift-cobalt)
## macOS
Read [build.md](build.md) for more information on how to build for macOS.
***
# Android
Android support is not yet available, but it will be soon.
***
# Web
Cobalt is not available for web yet, you can try using
lua-cobalt to compile a Lua Web Framework to work with Cobalt.
***
# Other