################################################################################
# Copyright 2011 by the Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in README.h
# Chip Overclock <coverclock@diag.com>
# http://www.diag.com/navigation/downloads/Lariat
# Lariat is a thin layer around Google Test that simplifies integrating unit
# test suites into an automated build and test system for continuous
# integration. It allows the tester to place resource constraints (real clock
# time is the most useful one) on a particular unit test suite so that a
# particular misbehaving suite can't bog down the entire automated test sweep.
# For example, you can require that each sweep completes in less than a minute
# of clock time. In addition, each suite is executed within its own process
# group. This makes it easier to kill not just a particular unit test suite
# but all subprocesses and threads it may otherwise leave behind.
################################################################################

PROJECT=lariat
TITLE=Lariat
MAJOR=1
MINOR=4
BUILD=3

HTTP_URL=http://www.diag.com/navigation/downloads/$(TITLE).html
GIT_URL=https://github.com/coverclock/com-diag-$(PROJECT).git

THERE=$(shell realpath ../..)

################################################################################
# PREREQUISITES
################################################################################

GTEST_ROOT=$(THERE)/googletest

GMOCK_DIR=$(GTEST_ROOT)/googlemock
GMOCK_LIB=$(GMOCK_DIR)/out/libgmock.a
GMOCK_INC=$(GMOCK_DIR)/include

GTEST_DIR=$(GTEST_ROOT)/googletest
GTEST_LIB=$(GTEST_DIR)/out/libgtest.a
GTEST_INC=$(GTEST_DIR)/include

ifdef 0
GTEST_INCS=-I$(GMOCK_INC) -I$(GTEST_INC)
GTEST_LIBS=$(GMOCK_LIB) $(GTEST_LIB)
GTEST_DEFS=-DCOM_DIAG_LARIAT_GMOCK=1
else
GTEST_INCS=-I$(GTEST_INC)
GTEST_LIBS=$(GTEST_LIB)
GTEST_DEFS=
endif

################################################################################
# PROJECT
################################################################################

CWD:=$(shell pwd)
PROJECT_DIR=$(CWD)

LARIAT_DIR=$(PROJECT_DIR)
LARIAT_LIB=$(LARIAT_DIR)/lib$(PROJECT).a
LARIAT_INC=$(LARIAT_DIR)/include

CC=gcc
CXX=g++
AR=ar
RANLIB=ranlib
STRIP=strip

CPPFLAGS=-I$(LARIAT_INC) $(GTEST_INCS) $(GTEST_DEFS)
CFLAGS=-g
CXXFLAGS=-g
ARFLAGS=rcv
LDFLAGS=-g $(LARIAT_LIB) $(GTEST_LIBS) -lpthread -lrt -lm

################################################################################
# LISTS
################################################################################

PHONY=# Make these targets every time regardless of dependencies.

TARGETS=# Make these targets for make all.

ARTIFACTS=# Remove these targets for make clean.

DELIVERABLES=# Remove these targets for make clobber.

ARCHIVABLE=# Include these targets in the archive or shared library.

################################################################################
# DEFAULT
################################################################################

PHONY+=default

default:	all

################################################################################
# RULES
################################################################################

TARGETS+=$(PROJECT).o

ARTIFACTS+=$(PROJECT).o

ARCHIVABLE+=$(PROJECT).o

TARGETS+=lib$(PROJECT).a

ARTIFACTS+=lib$(PROJECT).a

lib$(PROJECT).a:	$(ARCHIVABLE)
	$(AR) $(ARFLAGS) lib$(PROJECT).a $(ARCHIVABLE)
	$(RANLIB) lib$(PROJECT).a

TARGETS+=unittest

ARTIFACTS+=unittest

unittest:	unittest.o $(LARIAT_LIB) $(GTEST_LIB)
	$(CXX) -o unittest unittest.o $(LDFLAGS)

################################################################################
# UNIT TESTS
################################################################################

PHONY+=cpu

cpu:	unittest
	./unittest --gtest_filter=LariatDeathTest.Cpu -c 1
	echo "PASSED cpu"

PHONY+=core

core:	unittest
	./unittest --gtest_filter=LariatDeathTest.Core -E
	echo "PASSED core"

PHONY+=data

data:	unittest
	./unittest --gtest_filter=LariatTest.Data -d 10485760
	echo "PASSED data"

PHONY+=memory

memory:	unittest
	./unittest --gtest_filter=LariatTest.Memory -m 10485760
	echo "PASSED memory"

PHONY+=opened

opened:	unittest
	./unittest --gtest_filter=LariatTest.Opened -r 1 -o 10
	echo "PASSED opened"

PHONY+=real

real:	unittest
	./unittest --gtest_filter=LariatDeathTest.Real
	echo "PASSED real"

PHONY+=stack

stack:	unittest
	./unittest --gtest_filter=LariatDeathTest.Stack -s 10
	echo "PASSED stack"

PHONY+=thread

thread:	unittest
	./unittest --gtest_filter=LariatTest.Thread -r 1 -t 100
	echo "PASSED thread"

# Limit the entire google test suite to a fixed real time interval and verify
# that it is killed by SIGALRM.

PHONY+=limit

limit:	unittest
	./unittest --gtest_filter=LariatDeathTest.Limit -r 5 && false || test `expr $$? % 128` -eq 14
	echo "PASSED limit"

# Verify that all of the forked processes run in the same process group, that
# it is the process group of the parent process, and that we can kill the
# entire process group.

PHONY+=group

group:	unittest
	./unittest --gtest_filter=LariatTest.Group & PGID=$$!; echo $$PGID; sleep 5; ps -eo pgid,ppid,pid,comm | grep "^$$PGID "; /bin/kill -TERM -$$PGID; wait $$PGID; test `expr $$? % 128` -eq 15; ps -eo pgid,ppid,pid,comm | grep "^$$PGID " && false || true
	echo "PASSED group"

PHONY+=test

test:	cpu core data memory opened real stack thread limit group
	echo "PASSED all"

################################################################################
# PATTERNS
################################################################################

%.txt:	%.cpp
	$(CXX) -E $(CPPFLAGS) -c $< > $*.txt

%.o:	%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

%.txt:	%.c
	$(CC) -E $(CPPFLAGS) -c $< > $*.txt

%.o:	%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

%:	%_unstripped
	$(STRIP) -o $@ $<

################################################################################
# DEPENCENDIES
################################################################################

PHONY+=depend

ARTIFACTS+=dependencies.mk

CFILES=$(wildcard *.c)
CXXFILES=$(wildcard *.cpp)

depend:
	$(CXX) $(CPPFLAGS) -M -MG $(CFILES) $(CXXFILES) > dependencies.mk

-include dependencies.mk

################################################################################
# DOCUMENTATION
################################################################################

DOC_DIR=doc
BROWSER=firefox

PHONY+=documentation browse refman manpages
DELIVERABLES+=$(DOC_DIR)

documentation:
	mkdir -p $(DOC_DIR)/latex $(DOC_DIR)/man $(DOC_DIR)/pdf
	sed -e "s/\\\$$Name.*\\\$$/$(MAJOR).$(MINOR).$(BUILD)/" < doxygen.cf > doxygen-local.cf
	doxygen doxygen-local.cf
	( cd $(DOC_DIR)/latex; $(MAKE) refman.pdf; cp refman.pdf ../pdf )
	cat $(DOC_DIR)/man/man3/*.3 | groff -man -Tps - > $(DOC_DIR)/pdf/manpages.ps
	ps2pdf $(DOC_DIR)/pdf/manpages.ps $(DOC_DIR)/pdf/manpages.pdf

browse:
	$(BROWSER) file:doc/html/index.html

refman:
	$(BROWSER) file:doc/pdf/refman.pdf

manpages:
	$(BROWSER) file:doc/pdf/manpages.pdf

################################################################################
# ENTRY POINTS
################################################################################

PHONY+=all clean clobber pristine

all:	$(TARGETS)

clean:
	rm -f *.o
	rm -f $(ARTIFACTS)
	
clobber:	clean
	rm -f $(DELIVERABLES)

################################################################################
# END
################################################################################

.PHONY:	$(PHONY)
