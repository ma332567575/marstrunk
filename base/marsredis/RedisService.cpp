#include "RedisService.h"
#include "RedisPool.h"

static CRedisPool* g_pRedisPool = 0;

CRedisService::CRedisService(void)
{
}

CRedisService::~CRedisService(void)
{
}
bool CRedisService::Init(std::string strIP, unsigned short nPort,int nConnNum)
{
    if (g_pRedisPool != 0)
    {
        return true;
    }

    if (nPort < 0 || strIP.empty())
    {
        return false;
    }
    g_pRedisPool = new CRedisPool;
    if (!g_pRedisPool)
    {
        return false;
    }
    if (g_pRedisPool->InitRedisPool(strIP.c_str(),nPort,nConnNum))
    {
        //日志输出
        return true;
    }
    if (g_pRedisPool)
    {
        delete g_pRedisPool;
        g_pRedisPool = 0;
    }
    return false;
}
void CRedisService::UnInit()
{
    if (g_pRedisPool)
    {
        delete g_pRedisPool;
        g_pRedisPool = 0;
    }
    return;
}
bool CRedisService::GetRedisHash(std::string strKey, MAPKVINFOR& mapKValue)
{
    char szCommand[256] = {0};
    char szTmp[256] = {0};
    sprintf(szCommand,"hmget %s",strKey.c_str());    

    //获取查询字段名称，组织查询命令
    int nSize = mapKValue.size();
    if(nSize <= 0)
    {
        //日志输出
        return false;
    }

    MAPKVINFOR::iterator iter = mapKValue.begin();
    for (; iter != mapKValue.end(); iter++)
    {
        sprintf(szTmp,"%s",szCommand);
        std::string strFieldKey = iter->first;
        if(!strFieldKey.empty())
        {
            sprintf(szCommand,"%s %s", szCommand, strFieldKey.c_str());
        }
    }


    int nConnIndex = 0;
    redisReply* pRedisReply = 0;
    if (g_pRedisPool->ExeCmd(&pRedisReply,nConnIndex,szCommand))
    {
        //日志输出
        if (pRedisReply)
        {
            int i = 0;
            MAPKVINFOR::iterator iter = mapKValue.begin();
            for (; i < pRedisReply->elements; i++,iter++)
            {
                if (REDIS_REPLY_STRING == pRedisReply->element[i]->type)
                {
                    iter->second = pRedisReply->element[i]->str;
                }
                else
                {
                    iter->second = "";
                }
            }

        }

    }
    else
    {
        //日志输出
    }

    if (pRedisReply)
    {
        freeReplyObject(pRedisReply);
    }
    return true;
}

bool CRedisService::SetRedisHash(std::string strKey, MAPKVINFOR& mapKValue)
{
    bool bSet = false;
    char szCommand[256] = {0};
    char szTmp[256] = {0};
    sprintf(szCommand,"hmset %s",strKey.c_str());    

    //获取查询字段名称，组织查询命令
    int nSize = mapKValue.size();
    if(nSize <= 0)
    {
        //日志输出
        return false;
    }

    MAPKVINFOR::iterator iter = mapKValue.begin();
    for (; iter != mapKValue.end(); iter++)
    {
        sprintf(szTmp,"%s",szCommand);
        std::string strFKey = iter->first;
        std::string strFValue= iter->second;
        if(!strFKey.empty())
        {
            sprintf(szCommand,"%s %s %s", szTmp, strFKey.c_str(),strFValue.c_str());
        }
    }
    int nConnIndex = 0;
    redisReply* pRedisReply = 0;
    if (g_pRedisPool->ExeCmd(&pRedisReply,nConnIndex,szCommand))
    {
        //日志输出
        if (pRedisReply)
        {
            if (pRedisReply->element[0]->str == "OK") 
            {
                freeReplyObject(pRedisReply);
                return true;
            }
        }

    }
    else
    {
        //日志输出
    }
    if (pRedisReply)
    {
        freeReplyObject(pRedisReply);
    }
    return true;
}

bool CRedisService::GetRedisList(std::string strKey,LISTKVINFO& listKValue)
{
    char szCom[256] = {0};
    char szTmp[256] = {0};
    sprintf(szCom,"llen %s",strKey.c_str());

    // 查询list长度
    int nConnIndex = 0;
    redisReply* pRedisReply = 0;
    if (g_pRedisPool->ExeCmd(&pRedisReply,nConnIndex,szCom))
    {
        //日志输出
        if (pRedisReply)
        {
            int nLen = pRedisReply->integer;

            // 组织查询list命令
            char szCommand[256] = {0};
            sprintf(szCommand,"lrange %s 0 %d",strKey.c_str(), nLen - 1);

            redisReply* pReply = 0;
            if (g_pRedisPool->ExeCmd(&pReply,nConnIndex,szCommand))
            {
                //日志输出
                if (pReply)
                {
                    listKValue.clear();
                    for (int i = 0; i < pReply->elements; i++)
                    {
                        if (REDIS_REPLY_STRING == pReply->element[i]->type)
                        {
                            listKValue.push_back(pReply->element[i]->str);
                        }
                        else
                        {
                            listKValue.push_back("");
                        }
                    }
                }
            }
            if (pReply)
            {
                freeReplyObject(pReply);
            }
        }
    }
    else
    {
        //日志输出
    }
    if (pRedisReply)
    {
        freeReplyObject(pRedisReply);
    }
    return true;
}

bool CRedisService::SetRedisKValue(std::string strKey, std::string strValue)
{
    char szCommand[128] = {0};
    sprintf(szCommand,"set %s %s",strKey.c_str(),strValue.c_str());
    int nConnIndex = 0;
    redisReply* pRedisReply = 0;
    if (g_pRedisPool->ExeCmd(&pRedisReply,nConnIndex,szCommand))
    {
        //判断返回值是否等于OK
        if (REDIS_OK == pRedisReply->type)
        {
            freeReplyObject(pRedisReply);
            return true;
        }
    }
    if (pRedisReply)
    {
        freeReplyObject(pRedisReply);
    }
    return false;
}
bool CRedisService::GetRedisKValue(std::string strKey, std::string& strValue)
{
    char szCommand[128] = {0};
    sprintf(szCommand,"get %s",strKey.c_str());

    int nConnIndex = 0;
    redisReply* pRedisReply = 0;
    if (g_pRedisPool->ExeCmd(&pRedisReply,nConnIndex,szCommand))
    {
        if (pRedisReply)
        {
            strValue = pRedisReply->str;
            freeReplyObject(pRedisReply);
            return true;
            //if (REDIS_REPLY_STRING == pRedisReply->element[0]->type)
            //{
            //    strValue = pRedisReply->element[0]->str;
            //    freeReplyObject(pRedisReply);
            //    
            //}
            //else
            //{
            //    strValue = "";
            //    freeReplyObject(pRedisReply);
            //    //日志输出
            //    return false;
            //}
        }
    }
    if (pRedisReply)
    {
        freeReplyObject(pRedisReply);
    }
    return true;
}
bool CRedisService::SubscribeMsg(std::string strSubscribeCmd,std::string & strValue)
{
    char szCommand[128] = {0};
    sprintf(szCommand,"%s",strSubscribeCmd.c_str());

    int nConnIndex = 0;
    redisReply* pRedisReply = 0;
    if (g_pRedisPool->ExeCmd(&pRedisReply,nConnIndex,szCommand))
    {
        if (pRedisReply)
        {
            if (REDIS_REPLY_STRING == pRedisReply->element[0]->type)
            {
                strValue = pRedisReply->element[0]->str;
                freeReplyObject(pRedisReply);
                return true;
            }
            else
            {
                strValue = "";
                freeReplyObject(pRedisReply);
                //日志输出
                return false;
            }
        }
    }
    if (pRedisReply)
    {
        freeReplyObject(pRedisReply);
    }
    return true;
}
bool CRedisService::PublishMsg(std::string strPublishCmd,std::string & strValue)
{
    char szCommand[128] = {0};
    sprintf(szCommand,"%s",strPublishCmd.c_str());

    int nConnIndex = 0;
    redisReply* pRedisReply = 0;
    if (g_pRedisPool->ExeCmd(&pRedisReply,nConnIndex,szCommand))
    {
        if (pRedisReply)
        {
            if (REDIS_REPLY_STRING == pRedisReply->element[0]->type)
            {
                strValue = pRedisReply->element[0]->str;
                freeReplyObject(pRedisReply);
                return true;
            }
            else
            {
                strValue = "";
                freeReplyObject(pRedisReply);
                //日志输出
                return false;
            }
        }
    }
    if (pRedisReply)
    {
        freeReplyObject(pRedisReply);
    }
    return true;
}
