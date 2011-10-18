################################################################################
# Copyright 2011 by the Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in README.h
# Chip Overclock <coverclock@diag.com>
# http://www.diag.com/navigation/downloads/Biscuit
################################################################################

PROJECT=lariat
MAJOR=0
MINOR=0
FIX=0

SVN_URL=svn://graphite/biscuit/trunk/Lariat
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

PHONY+=core

core:	unittest
	./unittest --gtest_filter=LariatDeathTest.Core -E

PHONY+=memory

memory:	unittest
	./unittest --gtest_filter=LariatTest.Memory -r 1 -m 10485760

PHONY+=opened

opened:	unittest
	./unittest --gtest_filter=LariatTest.Opened -r 1 -o 10

PHONY+=real

real:	unittest
	./unittest --gtest_filter=LariatDeathTest.Real -r 5

PHONY+=stack

stack:	unittest
	./unittest --gtest_filter=LariatDeathTest.Stack -s 10

PHONY+=thread

thread:	unittest
	./unittest --gtest_filter=LariatTest.Thread -r 1 -t 10

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
