/*
          Copyright Oliver Kowalke 2010.
 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/

#include <cstdio>
#include <cstdlib>

#include <boost/cstdint.hpp>

#if defined(__mips__)
// on MIPS we assume 64bit regsiters - even for 32bit ABIs
typedef uint64_t greg_t;
typedef uint64_t freg_t;
#elif defined(__powerpc__)
#if defined(__powerpc64__)
typedef uint64_t greg_t;
#else
typedef uint32_t greg_t;
#endif
typedef uint64_t freg_t;
#elif defined(__x86_64__)
typedef uint64_t greg_t;
typedef uint32_t freg_t;
#else
typedef uint32_t greg_t;
typedef uint32_t freg_t;
#endif

/* number of preserved registers */
#if defined(_MSC_VER)

# if defined(_M_IX86)
#define BOOST_CONTEXT_NGREG	6
#define BOOST_CONTEXT_NFREG	1
#define BOOST_CONTEXT_CALLDECL __cdecl
# endif

#elif defined(__GNUC__)

# if defined(__i386__)
#define BOOST_CONTEXT_NGREG	6
#define BOOST_CONTEXT_NFREG	1
#define BOOST_CONTEXT_CALLDECL __attribute__((cdecl))

# elif defined(__x86_64__)
#define BOOST_CONTEXT_NGREG	9
#define BOOST_CONTEXT_NFREG	2
#define BOOST_CONTEXT_CALLDECL

# elif defined(__arm__)
#define BOOST_CONTEXT_NGREG	12
#define BOOST_CONTEXT_NFREG	16
#define BOOST_CONTEXT_CALLDECL

# elif defined(__mips__)
#  if _MIPS_SIM == _ABI64
#define BOOST_CONTEXT_NGREG	14
#define BOOST_CONTEXT_NFREG	8
#  else
#define BOOST_CONTEXT_NGREG	14
#define BOOST_CONTEXT_NFREG	6
#  endif
#define BOOST_CONTEXT_CALLDECL

# elif defined(__powerpc__)
#define BOOST_CONTEXT_NGREG	24
#define BOOST_CONTEXT_NFREG	19
#define BOOST_CONTEXT_CALLDECL

# endif
#endif

/* container for all preserved registers */
typedef greg_t gregset_t[BOOST_CONTEXT_NGREG];
typedef freg_t fregset_t[BOOST_CONTEXT_NFREG];

typedef struct stack	stack_t;
struct stack
{
	void	*	ss_base;
	void	*	ss_limit;
};

typedef struct fcontext	fcontext_t;
struct fcontext
{
	gregset_t		fc_greg;
	fregset_t		fc_freg;
	stack_t			fc_stack;
	fcontext_t	*	fc_link;
};

extern "C" int get_fcontext( fcontext_t * fc);
extern "C" int set_fcontext( fcontext_t const* fc);
extern "C" int swap_fcontext( fcontext_t * ofc, fcontext_t const* nfc);
extern "C" int make_fcontext( fcontext_t const* fc, void(*fn)(void*), void*p);

int x = 0;
int k = 10;
double y = 2.;

fcontext_t gfc1 = {}, gfc2 = {};

void f( void * p)
{
	int i = *(int*)p;
	x += i;
	fprintf(stderr, "x == %d\n", x);
}

void g( void * p)
{
	int i = *(int *) p;
	y += i;
	swap_fcontext( & gfc2, & gfc1);
}

void u( void * p)
{
	int i = * ( int *)p;
	x += i;
	fprintf(stderr, "u(): i == %d, x == %d\n", i, x);
}

fcontext_t gfc = {};

void test_setcontext()
{
	fcontext_t fc = {};

	posix_memalign( ( void **) & fc.fc_stack.ss_limit, 8, 655360);
	fc.fc_stack.ss_base = (char *)fc.fc_stack.ss_limit + 655360;

	get_fcontext( & fc);
	int i = 3;
	make_fcontext( & fc, f, & i);

	set_fcontext( & fc);
}

void test_link()
{
	fcontext_t fc1 = {}, fc2 = {};

	posix_memalign( ( void **) & fc1.fc_stack.ss_limit, 8, 655360);
	fc1.fc_stack.ss_base = (char *)fc1.fc_stack.ss_limit + 655360;

	posix_memalign( ( void **) & fc2.fc_stack.ss_limit, 8, 655360);
	fc2.fc_stack.ss_base = (char *)fc2.fc_stack.ss_limit + 655360;

	fc1.fc_link = & fc2;

	get_fcontext( & fc1);
	get_fcontext( & fc2);
	int i = 3;
	make_fcontext( & fc1, f, & i);
	make_fcontext( & fc2, u, ( void *) ( & k) );

	set_fcontext( & fc1);
}

void test_swapcontext()
{
	fcontext_t fc1 = {}, fc2 = {};

	posix_memalign( ( void **) & fc2.fc_stack.ss_limit, 8, 655360);
	fc2.fc_stack.ss_base = (char *)fc2.fc_stack.ss_limit + 655360;

	int i = 3;
	get_fcontext( & fc2);
	make_fcontext( & fc2, f, & i);

	swap_fcontext( & fc1, & fc2);
}

void test_global_swapcontext()
{
	posix_memalign( ( void **) & gfc2.fc_stack.ss_limit, 8, 655360);
	gfc2.fc_stack.ss_base = (char *)gfc2.fc_stack.ss_limit + 655360;

	int u = 3;
	get_fcontext( & gfc1);
	get_fcontext( & gfc2);
	make_fcontext( & gfc2, g, & u);

	y += 0.1;

	swap_fcontext( & gfc1, & gfc2);

	y += 0.5;

	fprintf(stderr, "x == %d\n", x);
	fprintf(stderr, "y == %f\n", y);
}

void test_jump()
{
	fcontext_t fc = {};
	get_fcontext( & fc );
	fprintf(stderr,"abc\n");
	set_fcontext( & fc);
}

int main(int argc, char *argv[])
{
	test_swapcontext();	
/*
	test_jump();	
	test_setcontext();
	test_link();
	test_swapcontext();
	test_global_swapcontext();
*/
	return EXIT_SUCCESS;
}
