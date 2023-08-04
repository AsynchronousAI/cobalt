# lxx/asm
Make standalone executable from lxx, this will return alot of C scripts and a Makefile.
```bash
$ lxxasm -h
Usage: lxx asm [options] <file>
Options:
  -h, --help            show this help message and exit
  -o FILE, --output=FILE
                        output file
```
```bash
$ lxxasm myapp.lxx -o
```
or if you have a folder of lxx files
```bash
$ lxxasm myfolderoflxx -o
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