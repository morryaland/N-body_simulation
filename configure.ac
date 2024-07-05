AC_INIT([nbody],[0.1])
AC_CONFIG_SRCDIR(src/main.c)

AC_PROG_CC

AC_CHECK_HEADERS(
stdbool.h
stdlib.h
stdio.h
math.h
string.h
assert.h
stddef.h
)

LIB_CIMGUI_PATH="./lib/cimgui/Makefile"
LIB_IMGUI_PATH="./lib/cimgui/imgui/imgui.h"
LIB_RAYLIB_PATH="./lib/raylib/raylib/CMakeLists.txt"

AC_CHECK_FILE($LIB_CIMGUI_PATH,, AC_MSG_ERROR(cimgui not found. Please read INSTALL))
AC_CHECK_FILE($LIB_IMGUI_PATH,, AC_MSG_ERROR(imgui not found. Please read INSTALL))
AC_CHECK_FILE($LIB_RAYLIB_PATH,, AC_MSG_ERROR(raylib not found. Please read INSTALL))

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES(Makefile)

AC_OUTPUT
