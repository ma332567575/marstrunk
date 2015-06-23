#include "MSimpleBuffer.h"

using namespace Mars;

MCSimpleBuffer::MCSimpleBuffer( MVOID ):m_Buffer(MNULL), m_AllocSize(0), m_WriteSize(0)
{/* */}

MCSimpleBuffer::~MCSimpleBuffer( MVOID )
{/* */}

MBOOL MCSimpleBuffer::ExpendBuffer( MI64 n64Size )
{
    // 1. ��麯�����л���
    if ( n64Size <= 0 )
    {
        return MFALSE;
    }

    // 2. ����BUFFER����
    MI64 nIncSize       = 0;
    MI64 nAllocHelper   = 0;

    nIncSize = n64Size;
    
    nAllocHelper = m_AllocSize + nIncSize;

    // 3. Realloc
    MPBYTE pBuf = MNULL;
    pBuf        = (MPBYTE)realloc( m_Buffer, nAllocHelper );

    // ����ʧ��
    if (pBuf == MNULL)
    {
        return MFALSE;
    }

    m_Buffer        = pBuf;
    m_AllocSize     = nAllocHelper;

    return MTRUE;
}

MI64 MCSimpleBuffer::Write( MPVOID pBuf, MI64 nSize )
{
    // 1. ���������������
    if ( !pBuf || nSize <= 0 )
    {
        return 0;
    }

    // 2. ����д�ͷ���
    if ( m_AllocSize - m_WriteSize < nSize )
    {
        ExpendBuffer( nSize );
    }

    // 3. д����
    MMemcpy( m_Buffer + m_WriteSize, m_AllocSize - m_WriteSize, pBuf, nSize );

    m_WriteSize += nSize;

    return nSize;
}

MI64 MCSimpleBuffer::Read( MPVOID pBuf, MI64 nSize )
{
    // 1. ���������������
    if ( nSize <= 0 )
    {
        return 0;
    }

    // 2. �ܶ����ٶ�����
    if ( nSize > m_WriteSize )
    {
        nSize = m_WriteSize;
    }

    // 3. ������ȥ
    if ( pBuf )
    {
        MMemcpy( pBuf, nSize, m_Buffer, nSize );
    }

    // 4. �ƶ�����
    // TODO: �����û���BUFFER
    m_WriteSize -= nSize;
    MMemmove( m_Buffer, m_AllocSize, m_Buffer + nSize, m_WriteSize );

    return nSize;
}

MVOID MCSimpleBuffer::CleanBuffer( MVOID )
{
    MZeroMemory( m_Buffer, m_WriteSize );
}
