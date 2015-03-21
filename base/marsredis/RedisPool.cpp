#include "RedisPool.h"
#include <assert.h> 
#include <process.h>

CRedisPool::CRedisPool(void)
{
    m_Semaphore             = INVALID_HANDLE_VALUE;
    MarsInitCriticalSection(&m_mapConnectCS);
}

CRedisPool::~CRedisPool(void)
{
    MarsDelCriticalSection(&m_mapConnectCS);
}

//����
bool CRedisPool::InitRedisPool(const char* aszIP, int nPort, int nConnNum)
{
    //����
    strcpy_s(m_szIpAdd, sizeof(m_szIpAdd), aszIP);
    m_nPort = nPort;
    m_nRedisConnNum = nConnNum;

    //�����ź���
    m_Semaphore         = CreateSemaphore(NULL, nConnNum, nConnNum, "redissemaphore");
    
    if (m_Semaphore == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    return true;
}

void CRedisPool::Close()
{    
    //�ͷ���������
    MarsEnterCriticalSection(&m_mapConnectCS);
    while(!m_ConnectInstSet.empty())
    {
        CRedisInstance* pInst = m_ConnectInstSet.front();
        m_ConnectInstSet.pop();
        if (pInst)
        {
            pInst->Close();
            delete pInst;
        }
    }
    MarsLeaveCriticalSection(&m_mapConnectCS);
    return;
}

bool CRedisPool::ExeCmd(redisReply** apRedisReply, int& nRedisIndex, const char* aszCmd, DWORD dwTimeout /*=30*1000*/)
{
    if (WaitForSingleObject(m_Semaphore, dwTimeout) == WAIT_TIMEOUT)
        return false;

    bool bSuc = false;
    do 
    {
        CRedisInstance* pRedisInstance = GetConnectInst();

        if (!pRedisInstance)
        {
            break;
        }

        va_list lap = NULL;

        //ִ��Redisָ��
        *apRedisReply = pRedisInstance->ExecuteRedisCommand(aszCmd,lap);

        MarsEnterCriticalSection(&m_mapConnectCS);
        //�ɹ���������ʵ���Ż�ȥ
        if (pRedisInstance->GetConnStatus()==REDIS_CONN_RUNNING)
        {
            m_ConnectInstSet.push(pRedisInstance);
        }
        else//ʧ�ܣ�����ʵ��ɾ��
        {
            delete pRedisInstance;
        }
        MarsLeaveCriticalSection(&m_mapConnectCS);
        
        bSuc = true;
    } while (0);
    
    //�����ź���
    if (!ReleaseSemaphore(m_Semaphore, 1, NULL))
    {
    }

    return bSuc;
}
void CRedisPool::FreeReply(redisReply* apreply)
{
    if(NULL != apreply)
    {
        freeReplyObject(apreply);
        apreply = NULL;
    }

    return;
}

CRedisInstance* CRedisPool::GetConnectInst()
{
    MarsEnterCriticalSection(&m_mapConnectCS);

    CRedisInstance* pInst = NULL;

    if (m_ConnectInstSet.empty())
    {
        try
        {
            pInst = new CRedisInstance();
        }
        catch(bad_alloc& exp)
        {
            MarsLeaveCriticalSection(&m_mapConnectCS);
            return pInst;
        }

        // ������
        if (!pInst->Open(m_szIpAdd, m_nPort))
        {
            delete pInst;
            pInst = NULL;

            MarsLeaveCriticalSection(&m_mapConnectCS);
            return pInst;
        }
    }
    else
    {
        // ��ȡ����ʵ��
        pInst = m_ConnectInstSet.front();
        m_ConnectInstSet.pop();
    }

    MarsLeaveCriticalSection(&m_mapConnectCS);
    return pInst;
}
