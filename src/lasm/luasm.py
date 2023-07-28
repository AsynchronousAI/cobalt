'''
    lasm.py - Lua Bytecode -> Assembly
    
    lasm is built into luax, and converts Lua bytecode to a format
    that is has a baked in borrow checker (used in rust) rather than 
    a garbage collector. This is done by converting the bytecode to
    a format that is more similar to assembly, and then using a 
    system that can bake in a borrow checker. This will give extremely
    good performance, and can be run on all platforms that support
    ANSI C and Python 3.9+.
'''

""" PERKS OF LASM:
- way faster than luac
- cannot be decompiled
- can be run on any platform that supports ANSI C and Python 3.9+
- shorter code
"""