#include "MWordFilter.h"

using namespace Mars;

static MBOOL MarsIsChinese
( 
    MARS_IN MCONST MBYTE cLeft, 
    MARS_IN MCONST MBYTE cRight 
)
{
    if ( cLeft >= 0xa1 && cLeft <= 0xfe &&
        cRight >= 0xa1 && cRight <= 0xfe )
    {
        return MTRUE;
    }
    return MFALSE;
}

static MCHAR MarsToLowercase( MARS_IN MCHAR cInput )
{
    if ( cInput >= 'A' && cInput <= 'Z' )
    {
        cInput += 32;
    }

    return cInput;
};

namespace Mars
{
    // 状态机的状态
    class MarsFSMState
    {
    public:
        MarsFSMState( MarsFSMState* pPrev, MCHAR cInput );
        ~MarsFSMState( MVOID );

    public:
        MBOOL IsEnd( MVOID )
        { return m_IsEnd; }

        std::string GetMatch( MVOID )
        { return m_Matching; }

        MBOOL HasPrev( MVOID )
        { return !m_Prev; }

    private:
        friend class MarsFiniteStateM;

        map<MCHAR, MarsFSMState*>       m_Nexts;
        MarsFSMState*                   m_Prev;
        MCHAR                           m_Input;
        MarsFSMState*                   m_BackTraceState;
        MBOOL                           m_IsEnd;
        string                          m_Matching;
    };

    // 有限状态机类
    class MarsFiniteStateM
    {
    public:
        MarsFiniteStateM( MVOID );
        ~MarsFiniteStateM( MVOID );

    public:
        MVOID Init( MARS_IN MCONST vector<string>& vecKeywords );
        MarsFSMState* Front( MVOID )
        { return m_FSMState; }
        MarsFSMState* NextState( MARS_IN MarsFSMState* pState, MARS_IN MCHAR cInput );

    private:
        MarsFSMState* InsertState( MARS_IN MarsFSMState* pSate, MARS_IN MCHAR cInput );

    private:
        MarsFSMState* m_FSMState;

    };
}

MarsFSMState::MarsFSMState( MarsFSMState* pPrev, MCHAR cInput )
:m_Input(cInput),m_IsEnd(MFALSE),m_Matching(""),m_Prev(pPrev)
{/* */}

MarsFSMState::~MarsFSMState( MVOID )
{/* */}
MarsFiniteStateM::MarsFiniteStateM( MVOID ):m_FSMState( MNULL )
{
    m_FSMState = new MarsFSMState( MNULL, 0 );
}

MarsFiniteStateM::~MarsFiniteStateM( MVOID )
{/* */}

MVOID MarsFiniteStateM::Init( MARS_IN MCONST vector<string>& vecKeywords )
{
    MarsFSMState* pFSMState = MNULL;

    vector<string>::const_iterator iterKeywords = vecKeywords.begin( );

    // 遍历所有keyword把它们加到状态树
    for ( ; iterKeywords != vecKeywords.end( ); ++iterKeywords )
    {
        pFSMState = m_FSMState;
        MSIZE_T sizetSize = iterKeywords->length( );

        for ( MI32 nCnt = 0; nCnt < sizetSize; ++ nCnt )
        {
            // 判断是否是汉字
            if  ( sizetSize - nCnt >= 2 && MarsIsChinese( (*iterKeywords)[nCnt], (*iterKeywords)[nCnt+1] ) )
            {
                pFSMState = InsertState( pFSMState, (*iterKeywords)[nCnt++] );
                pFSMState = InsertState( pFSMState, (*iterKeywords)[nCnt] );
            }
            else
            {
                pFSMState = InsertState( pFSMState, MarsToLowercase( (*iterKeywords)[nCnt] ) );
            }
        }

        pFSMState->m_IsEnd = MTRUE;
        pFSMState->m_Matching = *iterKeywords;
    }

}

MarsFSMState* MarsFiniteStateM::InsertState( MARS_IN MarsFSMState* pSate, MARS_IN MCHAR cInput )
{
    MarsFSMState* pNext = MNULL;

    // 已经有了，就取下一个
    if ( pSate->m_Nexts.count( cInput ) > 0 )
    {
        pNext = pSate->m_Nexts.find( cInput )->second;
    }
    else
    {
        // 否则添加一个
        pNext = new MarsFSMState( pSate, cInput );
        pSate->m_Nexts.insert( make_pair<MCHAR,MarsFSMState*>( cInput, pNext ) );
    }

    return pNext;
}

MarsFSMState* MarsFiniteStateM::NextState( MARS_IN MarsFSMState* pState, MARS_IN MCHAR cInput )
{
    if ( pState->m_Nexts.count( cInput ) )
    {
        return pState->m_Nexts.find( cInput )->second;
    }
    else if( pState == m_FSMState )
    {
        return m_FSMState;
    }
    else
    {
        return NextState( m_FSMState, cInput );
    }
}

//---------------------------------------------------------------------------------------------------

MWordFilter::MWordFilter(MVOID):m_IsInit(MFALSE),m_Fsm(MNULL)
{
    // 类使用者自己去Catch bad_alloc
    m_Fsm = new MarsFiniteStateM;
    // 临界区初始化
    MarsInitCriticalSection( &m_CritialSection );
}

MWordFilter::~MWordFilter(MVOID)
{
    // 临界区反初始化
    MarsDelCriticalSection( &m_CritialSection );
}

MarsFiniteStateMErrn MWordFilter::Load( MARS_IN MLPCSTR szPath )
{
    // 参数合法性检查
    if ( !szPath || !szPath[0] )
    {
        return MarsFiniteStateMErrn_Illegal_Param;
    }

    // 不支持二次Load
    if ( m_IsInit )
    {
        return MarsFiniteStateMErrn_Already_Init;
    }

    // 加载文件
    fstream fsLoad(szPath);

    if ( fsLoad.fail( ) )
    {
        return MarsFiniteStateMErrn_Loadfile_Failed;
    }

    vector<string> vecKeywords;
    string szLine;

    while( getline( fsLoad, szLine ) )
    {
        if ( fsLoad.fail( ) )
        {
            return MarsFiniteStateMErrn_Loadfile_Failed;
        }

        if ( szLine.empty( ) )
        {
            continue;
        }

        vecKeywords.push_back( szLine );
    }

    // 初始化有限状态机
    m_Fsm->Init( vecKeywords );

    m_IsInit = MTRUE;

    return MarsFiniteStateMErrn_NoErr;
}

std::string MWordFilter::Filter( MARS_IN MCONST string& szSrc )
{
    std::string szRst = szSrc;

    // 保证线程安全
    MarsEnterCriticalSection( &m_CritialSection );

    MarsFSMState* pSate = m_Fsm->Front( );
    MSIZE_T sizetSize = szSrc.length( );

    for ( MI32 nSize = 0; nSize < sizetSize; )
    {
        // 判断是否是汉字
        if ( ( sizetSize - nSize ) >= 0 && MarsIsChinese( szRst[nSize], szRst[nSize+1] ) )
        {
            pSate = m_Fsm->NextState( pSate, szRst[nSize++] );
            
            // 第一个字符不是就不判断第二个了
            if ( !pSate->HasPrev() )
            {
                pSate = m_Fsm->NextState( pSate, szRst[nSize] );    
            }
        }
        else
        {
            pSate = m_Fsm->NextState( pSate, MarsToLowercase(szRst[nSize]) );  
        }
        // 是个关键词就替换了
        if ( pSate->IsEnd( ) )
        {
            MSIZE_T nKeywordLen = pSate->GetMatch( ).length( );
            szRst.replace( nSize - nKeywordLen + 1, nKeywordLen, nKeywordLen, '*' );
        }
        ++nSize;
    }

    MarsLeaveCriticalSection( &m_CritialSection );
    
    return szRst;
}
