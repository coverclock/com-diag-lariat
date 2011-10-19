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
MAJOR=0
MINOR=1
FIX=0

SVN_URL=svn://graphite/$(PROJECT)/trunk/Lariat
HTTP_URL=http://www.diag.com/navigation/downloads/Lariat.html

################################################################################
# PREREQUISITES
################################################################################

GTEST_SRC=$(HOME)/src/gtest-1.6.0#http://googletest.googlecode.com/files/gtest-1.6.0.zip
GTEST_LIB=$(GTEST_SRC)/libgtest.a
GTEST_INC=$(GTEST_SRC)/include

################################################################################
# PROJECT
################################################################################

CWD:=$(shell pwd)
PROJECT_DIR=$(CWD)

LARIAT_SRC=$(PROJECT_DIR)
LARIAT_LIB=$(PROJECT_DIR)/lib$(PROJECT).a
LARIAT_INC=$(PROJECT_DIR)/include

CC=gcc
CXX=g++
AR=ar
RANLIB=ranlib
STRIP=strip

CPPFLAGS=-I$(LARIAT_INC) -I$(GTEST_INC)
CFLAGS=-g
CXXFLAGS=-g
ARFLAGS=rcv
LDFLAGS=-g $(LARIAT_LIB) $(GTEST_LIB) -lpthread -lrt -lm

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

unittest:	main.o $(LARIAT_LIB) $(GTEST_LIB)
	$(CXX) -o unittest main.o $(LDFLAGS)

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
# DISTRIBUTION
################################################################################

PHONY+=dist

ARTIFACTS+=$(PROJECT)-$(MAJOR).$(MINOR).$(FIX).tgz

dist $(PROJECT)-$(MAJOR).$(MINOR).$(FIX).tgz:
	TARDIR=$(shell mktemp -d /tmp/$(PROJECT).XXXXXXXXXX); \
	svn export $(SVN_URL) $$TARDIR/$(PROJECT)-$(MAJOR).$(MINOR).$(FIX); \
	tar -C $$TARDIR -cvzf - $(PROJECT)-$(MAJOR).$(MINOR).$(FIX) > $(PROJECT)-$(MAJOR).$(MINOR).$(FIX).tgz; \
	rm -rf $$TARDIR

################################################################################
# END
################################################################################

.PHONY:	$(PHONY)
