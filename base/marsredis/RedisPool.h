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
    //关闭
    void Close();
    //执行命令
    bool ExeCmd(redisReply** apRedisReply,int &awRedisIndex,const char *aszCmd,DWORD dwTimeout = 30*1000);
    //释放Redis应答
    void FreeReply(redisReply* apreply);
    //获取一个可用的redis实例
    CRedisInstance* GetConnectInst();

private:
    typedef queue<CRedisInstance*>          QueRedisInst;

private:
    char   m_szIpAdd[16];                               //IP地址
    int    m_nPort;                                     //端口
    int    m_nRedisConnNum;                             //Redis连接数

    MARS_CRITICAL_SECTION        m_mapConnectCS;
    QueRedisInst            m_ConnectInstSet;           //连接集合

    HANDLE  m_Semaphore;                                //连接池信号量(保证连接数)

};
