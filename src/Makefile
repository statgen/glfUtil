EXE=glfUtil
TOOLBASE = GlfExecutable Dump Split
SRCONLY = Main.cpp
HDRONLY = 

DATE=$(shell date)
USER=$(shell whoami)

USER_COMPILE_VARS = -DDATE="\"${DATE}\"" -DVERSION="\"${VERSION}\"" -DUSER="\"${USER}\""
COMPILE_ANY_CHANGE = GlfExecutable

PARENT_MAKE = Makefile.src
include ../Makefile.inc
