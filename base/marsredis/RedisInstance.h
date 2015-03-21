#pragma once
#include "hiredis.h"

enum ENUM_REDIS_CONN_STATUS
{
    REDIS_CONN_INIT = 1,        //Redis���ӳ�ʼ״̬
    REDIS_CONN_RUNNING,         //Redis����������
    REDIS_CONN_FAIL,            //Redis���ӶϿ�
};

class CRedisInstance
{
public:
    CRedisInstance(void);
    virtual ~CRedisInstance(void);

public:
    //����Redis
    bool Open(const char* aszIP, int nPort);
    //�ر�����
    void Close(void);
    //ִ��һ��Redisָ��
    redisReply* ExecuteRedisCommand(const char *aszformat,va_list lpVa=NULL);

    //����Redis
    bool ReConnect(const char* aszIP, int nPort);

    ENUM_REDIS_CONN_STATUS GetConnStatus(void);
    INT64 GetLastTime(void);

private:
    redisContext                *m_pRedisContext;       //Redis����������
    ENUM_REDIS_CONN_STATUS      m_enumConn;             //Redis����״̬
    INT64						m_i64LastTime;			//�������ϴ�ִ������ʱ��
     
};
