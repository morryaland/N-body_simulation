AC_INIT([nbody],[0.1])
AC_CONFIG_SRCDIR(src/main.c)

AC_PROG_CC
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

win_raylib_path="./lib/win/libraylib.lib ./lib/win/libraylibdll.lib"
linux_raylib_path="./lib/linux/libraylib.a"
mac_raylib_path="./lib/macos/libraylib.a"

case "${host_os}" in
    linux*)
        AC_CHECK_FILE($linux_raylib_path, LIBS=$linux_raylib_path)
        ;;
    cygwin*|mingw*)
        AC_CHECK_FILES($win_raylib_path, LIBS=$win_raylib_path)
        ;;
    darwin*)
        AC_CHECK_FILE($mac_raylib_path, LIBS=$mac_raylib_path)
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac 

AC_SUBST([LIBS])

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES(Makefile)

AC_OUTPUT
