# Merge a list of header files, so you can get all of them in a single include.
# Basically we concatenate them, except that we remove the #include directives
# referring to the files that we are merging. Note that we require that the
# list be already sorted in the order of the dependencies.

function is_core_include() {
    return \
        match($0, /^ *#include/) &&
        match($0, /".*"/) &&
        is_core[substr($0, RSTART+1, RLENGTH-2)]
}

BEGIN {
    for (i = 1; i < ARGC; i++) {
        is_core[ARGV[i]] = 1
    }

    print "#ifndef luacore_h"
    print "#define luacore_h"
}

END {
    print "#endif"
}

!is_core_include() { print }