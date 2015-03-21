#include "mars_define.h"
#include "MWordFilter.h"

using namespace Mars;

MVOID main( MVOID )
{
    MWordFilter Filter;
    MI32 nRst = Filter.Load( "D:\\marstrunk\\ut\\ut_utilib\\x64\\Debug\\words.txt" );
    if ( nRst != MarsFiniteStateMErrn_NoErr )
    {
        return;
    }
    string szRst = Filter.Filter( "k" );

    return;
}