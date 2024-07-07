AC_INIT([nbody],[0.1])
AC_CONFIG_SRCDIR(src/main.c)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_CANONICAL_HOST

AC_CHECK_HEADERS(
stdbool.h
stdlib.h
stdio.h
math.h
string.h
assert.h
stddef.h
)

win_raylib_path="./lib/win/libraylib.a -lgdi32 -lwinmm"
linux_raylib_path="./lib/linux/libraylib.a"
mac_raylib_path="./lib/macos/libraylib.a"

case "${host_os}" in
    linux*)
        LIBS=$linux_raylib_path
        OS=Linux
        ;;
    cygwin*|mingw*)
        LIBS=$win_raylib_path
        OS=Winodows_NT
        ;;
    darwin*)
        LIBS=$mac_raylib_path
        OS=Darwin
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac 

AC_SUBST([LIBS])
AC_SUBST([OS])

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES(Makefile)

AC_OUTPUT
