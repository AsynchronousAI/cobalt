# Building for other platforms
> NOTE: None of these are implemented to the Makefile, only a groundwork idea of how to build for these platforms and some scripts.
## Windows
If you have windows, we highly reccomend using cygwin to get GNU Make which is required to build Cobalt for 
all platforms.

You can use
```bash
$ cmake .
$ make msvc
$ make jit-msvc
```

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
