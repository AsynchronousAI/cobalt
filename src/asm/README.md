# luax/asm
Make standalone executable from Luax, this will return alot of C scripts and a Makefile.
```bash
$ luaxasm -h
Usage: luax asm [options] <file>
Options:
  -h, --help            show this help message and exit
  -o FILE, --output=FILE
                        output file
```
```bash
$ luaxasm myapp.luax -o
```
or if you have a folder of luax files
```bash
$ luaxasm myfolderofluax -o
```

***
Now you can run
```bash
$ make
```
to compile your application.

```bash
$ ./myapp
```