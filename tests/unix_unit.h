/*
 * unix_unit.h - Copyright (C) 2021 Barthandelous01
 *
 *Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *  may be used to endorse or promote products derived from this software
 *  without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UNIX_UNIT_H_
#define UNIX_UNIT_H_

/*
 * Pull in headers for a few functions
 */
# include <stdio.h> 		/* for fprintf */
# include <string.h>		/* for strcmp */
# include <stdlib.h>		/* for EXIT_* */

/*
 * test and suite function macros. These are always necessary,
 * so they're not conditionally evaluated.
 */
# define uFAIL_VAL 1
# define uPASS_VAL 0

# define uMAKE_TEST(x) static int unix_unit_test_ ## x (void) {
# define uEND_TEST fprintf(stdout, "."); fflush(stdout); return uPASS_VAL; }

# define uMAKE_SUITE(x) static int unix_unit_suite_ ## x (void) {
# define uEND_SUITE fprintf(stdout, "*"); fflush(stdout); return uPASS_VAL; }

/*
 * Test runner harness; does some magic to allow unit tests
 * to live in the same file as the rest of the code.
 */
# ifdef RUN_UNIT
#  define uTEST_HARNESS int main (void) {
# else
#  define uTEST_HARNESS static int unix_unit_main_stubbed_______ () {
# endif	/* RUN_UNIT */
# define uEND_HARNESS fprintf(stdout, "\n"); return EXIT_SUCCESS; }

/*
 * Testing macros; assertions and other sundry things
 */

/* the STUB_* family */
# ifdef RUN_UNIT
#  define uSTUB_RETURN_FUNCTION(type, name, val, ...)	\
	type name (__VA_ARGS__) { return val; }
#  define uSTUB_VOID_FUNCTION(name, ...)	\
	void name (__VA_ARGS__) {}
#  define uSTUB_CUSTOM_FUNCTION(type, name, ...)	\
	type name (__VA_ARGS__) {
#  define uSTUB_CUSTOM_END }
#  define uSTUB_VOID_CUSTOM_FUNCTION(name, ...)	\
	void name (__VA_ARGS__) {
#  define uSTUB_VOID_CUSTOM_END }
# else	/* otherwise, stub out the stubs */
#  define uSTUB_RETURN_FUNCTION(type, name, val, ...)
#  define uSTUB_VOID_FUNCTION(type, name, ...)
#  define uSTUB_CUSTOM_FUNCTION(type, name, ...)
#  define uSTUB_CUSTOM_END
#  define uSTUB_VOID_CUSTOM_FUNCTION(name, ...)
#  define uSTUB_VOID_CUSTOM_END
# endif	 /* RUN_UNIT */

/* the uassert_* family */
# define uassert(val)							\
	if (!(val)) {							\
		fprintf(stderr, "\nerror in %s:%d: assertion failed\n",	\
			__func__, __LINE__);				\
	return uFAIL_VAL;						\
	}
# define uassert_eq(lh, rh)						\
	if (!(lh == rh)){						\
		fprintf(stderr, "\nerror in %s:%d: %d != %d\n",		\
			__func__, __LINE__, lh, rh);			\
		return uFAIL_VAL;					\
	}
# define uassert_neq(lh, rh)						\
	if (!(lh != rh)) {						\
		fprintf(stderr, "\nerror in %s:%d: %d == %d\n",		\
			__func__, __LINE__, lh, rh);			\
		return uFAIL_VAL;					\
	}
# define uassert_gt(lh, rh)						\
	if (!(lh > rh)) {						\
		fprintf(stderr, "\nerror in %s:%d: %d !> %d\n",		\
			__func__, __LINE__, lh, rh);			\
		return uFAIL_VAL;					\
	}
# define uassert_gte(lh, rh)						\
	if (!(lh >= rh)) {						\
		fprintf(stderr, "\nerror in %s:%d: %d !>= %d\n",	\
			__func__, __LINE__, lh, rh);			\
		return uFAIL_VAL;					\
	}
# define uassert_str_eq(lh, rh)						\
	if (!(strcmp(lh, rh) == 0)){					\
		fprintf(stderr, "\nerror in %s:%d: %s != %s\n",		\
			__func__, __LINE__, lh, rh);			\
		return uFAIL_VAL;					\
	}
# define uassert_str_neq(lh, rh)					\
	if (!(strcmp(lh, rh) != 0)) {					\
		fprintf(stderr, "\nerror in %s:%d: %s == %s\n",		\
			__func__, __LINE__, lh, rh);			\
		return uFAIL_VAL;					\
	}
# define uassert_null(val)						\
	if (!(val == NULL)) {						\
		fprintf(stderr, "\nerror in %s:%d: val != NULL\n",	\
			__func__, __LINE__);				\
		return uFAIL_VAL;					\
	}

/* explicit pass/fail macros */
#define upass return uFAIL_VAL
#define ufail return uPASS_VAL
#define uxfail return uPASS_VAL

/* skip macro */
#define uskip fprintf(stdout, "x"); fflush(stdout); return uPASS_VAL

/* suite and test management functions */
# define uadd_test(name)				\
	if ( unix_unit_test_ ## name () != uPASS_VAL)	\
		return EXIT_FAILURE;
# define uadd_suite(name)				\
	if ( unix_unit_suite_ ## name () != uPASS_VAL)	\
		return EXIT_FAILURE;

#endif	/* UNIX_UNIT_H_ */
