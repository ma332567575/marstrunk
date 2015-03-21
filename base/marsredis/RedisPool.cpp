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

//启动
bool CRedisPool::InitRedisPool(const char* aszIP, int nPort, int nConnNum)
{
    //配置
    strcpy_s(m_szIpAdd, sizeof(m_szIpAdd), aszIP);
    m_nPort = nPort;
    m_nRedisConnNum = nConnNum;

    //创建信号量
    m_Semaphore         = CreateSemaphore(NULL, nConnNum, nConnNum, "redissemaphore");
    
    if (m_Semaphore == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    return true;
}

void CRedisPool::Close()
{    
    //释放所有连接
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

        //执行Redis指令
        *apRedisReply = pRedisInstance->ExecuteRedisCommand(aszCmd,lap);

        MarsEnterCriticalSection(&m_mapConnectCS);
        //成功，把连接实例放回去
        if (pRedisInstance->GetConnStatus()==REDIS_CONN_RUNNING)
        {
            m_ConnectInstSet.push(pRedisInstance);
        }
        else//失败，连接实例删除
        {
            delete pRedisInstance;
        }
        MarsLeaveCriticalSection(&m_mapConnectCS);
        
        bSuc = true;
    } while (0);
    
    //增加信号量
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

        // 打开连接
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
        // 获取连接实例
        pInst = m_ConnectInstSet.front();
        m_ConnectInstSet.pop();
    }

    MarsLeaveCriticalSection(&m_mapConnectCS);
    return pInst;
}
