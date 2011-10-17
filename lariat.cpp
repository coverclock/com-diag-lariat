/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA
 * Licensed under the terms in README.h
 * Chip Overclock (coverclock@diag.com)
 * http://www.diag.com/navigation/downloads/Lariat.html
 * Implements the Lariat surrogate main program com::diag::lariat::main.
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "lariat.h"

namespace com { namespace diag { namespace lariat {

using namespace ::std;

const char * number(const char * string, unsigned long * valuep)
{
	unsigned long value;
	char * end;

	value = strtoull(string, &end, 0);

	if (valuep != 0) { *valuep = value; }
	if (*end != '\0') { errno = EINVAL; perror(string); }

	return end;
}

const unsigned long UNLIMITED = ~(unsigned long)0;

int limit(int resource, unsigned long value, bool force)
{
	int rc = -1;
	struct rlimit limit;

    do {

    	if (getrlimit(RLIMIT_CORE, &limit) < 0) {
    		perror("getrlimit");
            break;
        }

		if (force) {
    		limit.rlim_cur = value;
		} else if (value == UNLIMITED) {
    		limit.rlim_cur = limit.rlim_max;
    	} else if (value < limit.rlim_max) {
    		limit.rlim_cur = value;
    	} else {
    		limit.rlim_cur = limit.rlim_max;
    	}

		if (setrlimit(RLIMIT_CORE, &limit) < 0) {
			perror("setrlimit");
            break;
		}

        rc = 0;

    } while (0);

	return rc;
}

void handler(int signum)
{
	if (signum == SIGALRM) {
		errno = ETIMEDOUT;
		perror("SIGALRM");
		exit(2);
	}
}

int install(int signum, void (* handler)(int), bool restart)
{
    struct sigaction action;
    int rc;

    memset(&action, 0, sizeof(action));
    action.sa_handler = (handler != 0) ? handler : SIG_DFL;
    action.sa_flags = restart ? SA_RESTART : 0;

    if ((rc = sigaction(signum, &action, (struct sigaction *)0)) < 0) {
        perror("sigaction");
    }

    return rc;
}

int timer(int which, unsigned long seconds)
{
    struct itimerval timer;
    int rc;

    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    if ((rc = setitimer(which, &timer, (struct itimerval *)0)) < 0) {
    	perror("setitimer");
    }

    return rc;
}

/**
 * Print a usage menu.
 * @param program points to the program name.
 * @param stream points to an output stream to which the menu is printed.
 */
static void usage(const char * program, FILE * stream)
{
    fprintf(stream, "usage: %s [ -c SECONDS | -C ] [ -d BYTES | -D ] [ -e BYTES | -E ] [ -f BYTES | -F ] [ -o OPENED | -O ] [ -r SECONDS | -R ] [ -s BYTES | -S ] [ -t THREADS | -T ] [ -v BYTES | -V ] [ -? ]\n", program);
    fprintf(stream, "       -c SECONDS    Set the CPU time limit to SECONDS\n");
    fprintf(stream, "       -C            Set the CPU time limit to unlimited\n");
    fprintf(stream, "       -d BYTES      Set the data segment size limit to BYTES\n");
    fprintf(stream, "       -D            Set the data segment size limit to unlimited\n");
    fprintf(stream, "       -e BYTES      Set the core file limit to BYTES\n");
    fprintf(stream, "       -E            Set the core file limit to unlimited\n");
    fprintf(stream, "       -f BYTES      Set the file size limit to BYTES\n");
    fprintf(stream, "       -F            Set the file size limit to unlimited\n");
    fprintf(stream, "       -o OPENED     Set the open file descriptor limit to OPENED\n");
    fprintf(stream, "       -O            Set the open file descriptor to unlimited\n");
    fprintf(stream, "       -r SECONDS    Set the real time limit to SECONDS\n");
    fprintf(stream, "       -R            Set the real time limit to unlimited\n");
    fprintf(stream, "       -s BYTES      Set the process stack size limit to BYTES\n");
    fprintf(stream, "       -S            Set the process stack size limit to unlimited\n");
    fprintf(stream, "       -t THREADS    Set the process and thread limit to THREADS\n");
    fprintf(stream, "       -T            Set the process and thread limit to unlimited\n");
    fprintf(stream, "       -v BYTES      Set the virtual memory limit to BYTES\n");
    fprintf(stream, "       -V            Set the virtual memory limit to unlimited\n");
    fprintf(stderr, "       -!            Enable debug output\n");
    fprintf(stderr, "       -?            Print menu\n");
}

int main(int argc, char ** argv, char ** envp)
{
	const char * program;
    int opt;
    extern char * optarg;
    bool debug = false;
    bool done = false;
    bool error = false;
    unsigned long value;

    program = strrchr(argv[0], '/');
    program = (program == (char *)0) ? argv[0] : program + 1;

    ::testing::InitGoogleTest(&argc, argv);

    while ((opt = getopt(argc, argv, "c:Cd:De:Ef:Fo:Os:St:Tv:V!?")) >= 0) {

        switch (opt) {

        case 'c':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (limit(RLIMIT_CPU, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'C':
            if ((limit(RLIMIT_CPU) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case 'd':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (limit(RLIMIT_DATA, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'D':
            if ((limit(RLIMIT_DATA) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case 'e':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (limit(RLIMIT_CORE, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'E':
            if ((limit(RLIMIT_CORE) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case 'f':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (limit(RLIMIT_FSIZE, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'F':
            if ((limit(RLIMIT_FSIZE) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case 'o':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (limit(RLIMIT_NOFILE, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'O':
            if ((limit(RLIMIT_NOFILE) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case 'r':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (install(SIGALRM, handler) == 0) && (timer(ITIMER_REAL, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'R':
            if ((install(SIGALRM) == 0) && (timer(ITIMER_REAL) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case 's':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (limit(RLIMIT_STACK, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'S':
            if ((limit(RLIMIT_STACK) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case 't':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (limit(RLIMIT_NPROC, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'T':
            if ((limit(RLIMIT_NPROC) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case 'v':
            if ((!(error = (*number(optarg, &value) != '\0'))) && (limit(RLIMIT_AS, value) == 0) && debug) {
            	fprintf(stderr, "%s: -%c %lu\n", program, opt, value);
            }
            break;

        case 'V':
            if ((limit(RLIMIT_AS) == 0) && debug) {
            	fprintf(stderr, "%s: -%c\n", program, opt);
            }
            break;

        case '!':
            debug = true;
            break;

        case '?':
            usage(program, stderr);
            done = true;
            break;

        default:
        	usage(program, stderr);
            error = true;
            break;

        }

        if (error) {
            break;
        }

        if (done) {
            break;
        }

    }

    if (error) {
    	exit(1);
    }

    if (done) {
    	exit(0);
    }

    if (debug) {
    	if (envp != 0) {
			for (; (*envp !=  0); ++envp) {
				fprintf(stderr, "%s: %s\n", program, *envp);
			}
    	}
    }

    return RUN_ALL_TESTS();
}


} } }
