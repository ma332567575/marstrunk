#pragma once
#include "RedisInstance.h"
#include <queue>
#include "MSyncTool.h"
using namespace std;
using namespace Mars;
class CRedisPool
{
public:
    CRedisPool(void);
    virtual ~CRedisPool(void);

public:
    //Init
    bool InitRedisPool(const char* aszIP, int awPort, int awConnNum);
    //�ر�
    void Close();
    //ִ������
    bool ExeCmd(redisReply** apRedisReply,int &awRedisIndex,const char *aszCmd,DWORD dwTimeout = 30*1000);
    //�ͷ�RedisӦ��
    void FreeReply(redisReply* apreply);
    //��ȡһ�����õ�redisʵ��
    CRedisInstance* GetConnectInst();

private:
    typedef queue<CRedisInstance*>          QueRedisInst;

private:
    char   m_szIpAdd[16];                               //IP��ַ
    int    m_nPort;                                     //�˿�
    int    m_nRedisConnNum;                             //Redis������

    MARS_CRITICAL_SECTION        m_mapConnectCS;
    QueRedisInst            m_ConnectInstSet;           //���Ӽ���

    HANDLE  m_Semaphore;                                //���ӳ��ź���(��֤������)

};
