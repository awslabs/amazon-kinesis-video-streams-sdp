#ifndef CATCH_ASSERT_H_
#define CATCH_ASSERT_H_

#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

#ifndef CATCH_JMPBUF
    #define CATCH_JMPBUF    waypoint_
#endif

static jmp_buf CATCH_JMPBUF;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void catchHandler_( int signal )
{
    longjmp( CATCH_JMPBUF, signal );
}
#pragma GCC diagnostic pop

#define catch_assert( x )                    \
    do {                                     \
        int ltry = 0, lcatch = 0;            \
        int saveFd = dup( 2 );               \
        struct sigaction sa = { 0 }, saveSa; \
        sa.sa_handler = catchHandler_;       \
        sigaction( SIGABRT, &sa, &saveSa );  \
        close( 2 );                          \
        if( setjmp( CATCH_JMPBUF ) == 0 )    \
        {                                    \
            ltry++;                          \
            x;                               \
        }                                    \
        else                                 \
        {                                    \
            lcatch++;                        \
        }                                    \
        sigaction( SIGABRT, &saveSa, NULL ); \
        dup2( saveFd, 2 );                   \
        close( saveFd );                     \
        TEST_ASSERT_EQUAL( ltry, lcatch );   \
    } while( 0 )

#endif /* ifndef CATCH_ASSERT_H_ */
