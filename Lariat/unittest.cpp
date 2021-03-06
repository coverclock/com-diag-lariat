/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 * Lariat Unit Test Suite
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA
 * Licensed under the terms in README.h
 * Chip Overclock (coverclock@diag.com)
 * http://www.diag.com/navigation/downloads/Lariat.html
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include "gtest/gtest.h"
#include "com/diag/lariat/lariat.h"

using namespace std;

namespace com { namespace diag { namespace lariat { namespace test {

static void stack(char was[1024]) {
	char now[1024];
	return stack(now);
}

TEST(LariatDeathTest, Stack)  {
	char array[1024];
	EXPECT_EXIT(stack(array), ::testing::KilledBySignal(SIGSEGV), ".*");
}

static void * memory() {
	void * ptr;
	try {
		if ((ptr = malloc(10485760 * 10)) == 0) {
			perror("malloc");
		} else {
			free(ptr);
		}
	} catch (...) {
		perror("malloc");
		ptr = 0;
	}
	return ptr;
}

TEST(LariatTest, Memory) {
	EXPECT_EQ(memory(), (void *)0);
}

static void * data() {
	void * ptr;
	if ((ptr = sbrk(10485760 * 10)) == (void *)-1) {
		perror("sbrk");
	}
	return ptr;
}

TEST(LariatTest, Data) {
	EXPECT_EQ(data(), (void *)-1);
}

static void cpu() {
	while (true) {
		;
	}
}

TEST(LariatDeathTest, Cpu) {
	EXPECT_EXIT(cpu(), ::testing::KilledBySignal(SIGXCPU), ".*");
}

static void real() {
	if (::com::diag::lariat::timer(5) == 0) {
		sleep(10);
	}
}

TEST(LariatDeathTest, Real) {
	EXPECT_EXIT(real(), ::testing::KilledBySignal(SIGALRM), ".*");
}

static int core() {
	int rc;
	pid_t pid = getpid();
	if (pid < 0) {
		perror("getpid");
	} else {
		rc = kill(getpid(), SIGABRT);
		if (rc < 0) {
			perror("kill");
		}
	}
	return rc;
}

TEST(LariatDeathTest, Core) {
	EXPECT_EXIT(core(), ::testing::KilledBySignal(SIGABRT), ".*");
}

static int thread() {
	int nn = 0;
	pid_t pid;
	while ((pid = fork()) >= 0) {
		if (pid == 0) {
			sleep(1);
			exit(0);
		} else {
			++nn;
		}
	}
	perror("fork");
	fprintf(stderr, "forks=%d\n", nn);
	return pid;
}

TEST(LariatTest, Thread) {
	EXPECT_EQ(thread(), -1);
}

static int opened() {
	int nn = 0;
	int fd;
	while ((fd = open("/dev/null", O_RDONLY)) >= 0) {
		++nn;
	}
	perror("open");
	fprintf(stderr, "opens=%d\n", nn);
	return fd;
}

TEST(LariatTest, Opened) {
	EXPECT_EQ(opened(), -1);
}

static void limit() {
	sleep(10);
}

TEST(LariatDeathTest, Limit) {
	EXPECT_EXIT(real(), ::testing::KilledBySignal(SIGALRM), ".*");
}

static int group() {
	pid_t pid;
	for (int ii = 0; ii < 10; ++ii) {
		if ((pid = fork()) < 0) {
			perror("fork");
			return pid;
		} else if (pid == 0) {
			continue;
		} else {
			for (int jj = 0; jj < 20; jj++) {
				sleep(1);
			}
			exit(0);
		}
	}
	for (int kk = 0; kk < 20; kk++) {
		sleep(1);
	}
	return 0;
}

TEST(LariatTest, Group) {
	EXPECT_EQ(group(), 0);
}

} } } }

int main(int argc, char ** argv, char ** envp)
{
	return ::com::diag::lariat::main(argc, argv, envp);
}
