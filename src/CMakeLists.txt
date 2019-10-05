project(fsa C)
set(CMAKE_C_FLAGS "-Werror=all -Werror=extra \
    -fPIE -fstack-check -fstack-protector-all -ftrapv -D_FORTIFY_SOURCE=2 \
    -Werror=float-equal -Wformat-security -Werror=format-overflow=2 -Wformat-truncation=2\
    -Winline -Winvalid-pch -Wjump-misses-init -Wlogical-op -Wmissing-declarations \
    -Wmissing-prototypes -Wnested-externs -Wold-style-definition \
    -Woverlength-strings -Werror=redundant-decls -Werror=shadow -Werror=strict-overflow=5 \
    -Wsuggest-attribute=const -Werror=switch-default -Wtrampolines \
    -Werror=undef -Wunsuffixed-float-constants\
    -Werror=unused -Werror=stringop-overflow=4")
add_library(fsa SHARED fsa.c)