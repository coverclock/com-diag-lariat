#ifndef COM_DIAG_LARIAT_LARIAT_H_
#define COM_DIAG_LARIAT_LARIAT_H_

/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA
 * Licensed under the terms in README.h
 * Chip Overclock (coverclock@diag.com)
 * http://www.diag.com/navigation/downloads/Lariat.html
 * Implements the Lariat surrogate main program com::diag::lariat::main.
 */

#include "gtest/gtest.h"

namespace com { namespace diag { namespace lariat {

/**
 * Convert the string into a binary number suitable for a resource limit value.
 * @param string points to the string.
 * @param valuep if non-null points to where the value is returned.
 * @return a pointer past the last numeric character of the string.
 */
extern const char * number(const char * string, unsigned long * valuep);

/**
 * This value indicates that the resource limit is unlimited. For unprivileged
 * processes this really implies the pre-defined maximum hard limit value, and
 * even then the limit must be forced.
 */
extern const unsigned long UNLIMITED;

/**
 * Set the limit of the specified resource to the specified value. The resource
 * limit is set to the value or to the pre-defined maximum hard limit value,
 * whichever is less.
 * @param resource identifies the resource.
 * @param value is the new limit value or ~0 to indicate unlimited.
 * @param force if true overrides the pre-defined maximum hard limit value.
 * @return 0 for success, <0 otherwise.
 */
extern int limit(int resource, unsigned long value = UNLIMITED, bool force = false);

/**
 * Handle a signal. If the signal is SIGALRM, the entire thread group exits.
 * @param signum is the signal number.
 */
extern void handler(int signum);

/**
 * Install the signal handler for the specified signal.
 * @param signum is the signal for which the handler is installed.
 * @param handler points to the signal handler function or NULL to remove.
 * @param restart if true causes interrupted system calls to be restarted.
 * @return 0 for success, <0 otherwise.
 */
extern int install(int signum, void (* handler)(int) = 0, bool restart = false);

/**
 * Set a non-periodic interval timer for real clock time in seconds or clear an
 * existing timer.
 * @param which indicates the timer type: ITIMER_REAL, ITIMER_VIRTUAL, or ITIMER_PROF.
 * @param seconds is the timer interval in seconds or zero to clear.
 * @return 0 for success, <0 otherwise.
 */
extern int timer(int which, unsigned long seconds = 0);

/**
 * This is a surrogate main program that can be called by the real main program
 * to do all the housekeeping necessary to run all unit tests. It implements
 * a bunch of command line options to place or relax limits on the unit test
 * process. This is useful when running the process as part of a larger, and
 * perhaps automated, unit test suite.
 * @param argc is the number of command line arguments including the program name.
 * @param argv is a vector of the command line argument strings.
 * @param envp is a vector of the environmental variables.
 * @return the number of unit tests that failed.
 */
extern int main(int argc, char ** argv, char ** envp = 0);

} } }

#endif /* COM_DIAG_LARIAT_LARIAT_H_ */
