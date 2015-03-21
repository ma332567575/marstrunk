#include "RedisInstance.h"

CRedisInstance::CRedisInstance(void)
{
    m_pRedisContext = NULL;
    m_enumConn = REDIS_CONN_INIT;
    m_i64LastTime = 0;
}

CRedisInstance::~CRedisInstance(void)
{
    Close();
}
//����Redis
bool CRedisInstance::Open(const char* aszIP, int nPort)
{
    struct timeval tv;              //1.5����
    tv.tv_sec = 1;
    tv.tv_usec = 500000;
    m_pRedisContext = redisConnectWithTimeout(aszIP, nPort, tv);
    if (m_pRedisContext ==NULL || 0 != m_pRedisContext->err)
    {
        return false;
    }
    m_enumConn = REDIS_CONN_RUNNING;
    m_i64LastTime = ::GetTickCount();

    return true;
}
//�ر�����
void CRedisInstance::Close(void)
{
    if (m_pRedisContext != NULL)
    {
        redisFree(m_pRedisContext);
        m_pRedisContext = NULL;
    }
}
//ִ��һ��Redisָ��
redisReply* CRedisInstance::ExecuteRedisCommand(const char *aszformat,va_list lpVa)
{
    redisReply *lpReply = NULL;
    if (m_pRedisContext == NULL)
    {
        return NULL;
    }
    if (m_enumConn != REDIS_CONN_RUNNING)
    {
        return NULL;
    }
    m_i64LastTime = ::GetTickCount();
    lpReply = (redisReply*)redisvCommand(m_pRedisContext,aszformat,lpVa);
    if (lpReply == NULL)
    {
        if (m_pRedisContext == NULL)
        {
            m_enumConn = REDIS_CONN_FAIL;
            //�ر�����
            Close();
        }
        else
        {
            switch(m_pRedisContext->err)
            {
            case REDIS_ERR_PROTOCOL:
            case REDIS_ERR_IO:
            case REDIS_ERR_EOF:
            case REDIS_ERR_OTHER:
                //��������ʧ��
                m_enumConn = REDIS_CONN_FAIL;
                //�ر�����
                Close();
                break;
            default:
                break;
            }
        }
    }

    return lpReply;
}
//����Redis
bool CRedisInstance::ReConnect(const char* aszIP, int nPort)
{
    //�رյ�ǰ����
    Close();
    return Open(aszIP,nPort);
}

ENUM_REDIS_CONN_STATUS CRedisInstance::GetConnStatus(void)
{
    return m_enumConn;
}

INT64 CRedisInstance::GetLastTime(void)
{
    return m_i64LastTime;
}