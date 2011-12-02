/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef COM_DIAG_LARIAT_LARIAT_H_
#define COM_DIAG_LARIAT_LARIAT_H_

/**
 * @file
 * Lariat Surrogate Main Program com::diag::lariat::main Declaration
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA
 * Licensed under the terms in README.h
 * Chip Overclock (coverclock@diag.com)
 * http://www.diag.com/navigation/downloads/Lariat.html
 */

namespace com { namespace diag { namespace lariat {

/**
 * Convert the string into a binary number suitable for a resource limit value.
 * @param string points to the string.
 *
 * @param valuep if non-null points to where the value is returned.
 * @return a pointer past the last numeric character of the string.
 */
extern const char * number(const char * string, unsigned long * valuep = 0);

/**
 * Print a stack trace to the specified file descriptor using a buffer of
 * the specified size.
 *
 * @param buffer points to a buffer into which the stack trace data is stored.
 * @param size is the cardinality of the buffer.
 * @param fd is the file descriptor.
 * @return the number of positions used in the buffer.
 */
extern int stacktrace(void ** buffer, unsigned int size, int fd = 2);

/**
 * Print a stack trace to standard error using a default buffer.
 *
 * @return the number of positions used in the default buffer.
 */
extern int stacktrace();

/**
 * This value indicates that the resource limit is unlimited. For unprivileged
 * processes this really implies the pre-defined maximum hard limit value, and
 * even then the limit must be forced.
 */
static const unsigned long UNLIMITED = ~(unsigned long)0;

/**
 * Set the limit of the specified resource to the specified value. The resource
 * limit is set to the value. If UNLIMITED is specified, the resource limit is
 * set to the pre-defined hard maximum, or to INFINITY is force is applied.
 * Only privileged processes may apply force.
 *
 * @param resource identifies the resource.
 * @param value is the new limit value or UNLIMITED.
 * @param force if true overrides the pre-defined maximum hard limit value.
 * @return 0 for success, <0 otherwise.
 */
extern int limit(int resource, unsigned long value = UNLIMITED, bool force = false);

/**
 * Install the signal handler for the specified signal.
 *
 * @param signum is the signal for which the handler is installed.
 * @param handler points to the signal handler function or NULL to remove.
 * @param restart if true causes interrupted system calls to be restarted.
 * @param handlerp points to where a pointer to the prior handler is returned.
 * @return 0 for success, <0 otherwise.
 */
extern int install(int signum, void (* handler)(int) = 0, bool restart = false, void (** handlerp)(int) = 0);

/**
 * Set a non-periodic interval timer for real clock time in seconds or clear an
 * existing timer.
 *
 * @param seconds is the timer interval in seconds or zero to clear.
 * @return 0 for success, <0 otherwise.
 */
extern int timer(unsigned long seconds = 0);

/**
 * This is a surrogate main program that can be called by the real main program
 * to do all the housekeeping necessary to run all unit tests. It implements
 * a bunch of command line options to place or relax limits on the unit test
 * process. This is useful when running the process as part of a larger, and
 * perhaps automated, unit test suite.
 *
 * @param argc is the number of command line arguments including the program name.
 * @param argv is a vector of the command line argument strings.
 * @param envp is a vector of the environmental variables.
 * @return the number of unit tests that failed.
 */
extern int main(int argc, char ** argv, char ** envp = 0);

} } }

#endif /* COM_DIAG_LARIAT_LARIAT_H_ */
