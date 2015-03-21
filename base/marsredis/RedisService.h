/********************************************************************
	Copyright(C)
	filename:	RedisService.h
	created:	2015/3/17 10:37:05
	author:		
	Description: 目前的redis不跨平台，暂时不用，先留下个window版本在这

	History:
	1. 2015/3/17 10:37:05 Create file
	...

*********************************************************************/
#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include <list>

typedef std::map<std::string, std::string> MAPKVINFOR;
typedef std::list<std::string> LISTKVINFO;
enum OPERATE_ENUM
{
    ENUM_READ = 0,
    ENUM_WRITE
};

class CRedisService
{
public:
    CRedisService(void);
    virtual ~CRedisService(void);

public:
    // --------------------------------------------------------------------------------------------------------
    // 函数名称：Init
    // 函数功能：初始化
    // 入口参数：string strIP                           Master IP
    //           unsigned short wMasterPort             Master Port 
    //           unsigned short wConnNum                连接数
    // 返回结果：bool                                    处理是否成功  
    // --------------------------------------------------------------------------------------------------------
    bool Init(std::string strIP, unsigned short nPort,int nConnNum);

    // --------------------------------------------------------------------------------------------------------
    // 函数名称：UnInit
    // 函数功能：反初始化
    // 入口参数：无
    // 返回结果：无   
    // --------------------------------------------------------------------------------------------------------
    void UnInit();

    // --------------------------------------------------------------------------------------------------------
    // 函数名称：GetRedisHash
    // 函数功能：通过KEY获得Redis中hash内容，装入容器map中
    // 入口参数：std::string strKey                      查询字段名
    //           MAPKVINFOR& mapKValue                   返回hash内容(map)
    // 返回结果：bool                                    处理是否成功
    // --------------------------------------------------------------------------------------------------------
    bool GetRedisHash(std::string strKey,MAPKVINFOR& mapKValue);

    // --------------------------------------------------------------------------------------------------------
    // 函数名称：SetRedisHash
    // 函数功能：插入指定Key的hash内容
    // 入口参数：std::string strKey                      查询字段名
    //           MAPKVINFOR mapKValue                   输入查询hash内容(map)
    // 返回结果：bool                                    处理是否成功
    // --------------------------------------------------------------------------------------------------------
    bool SetRedisHash(std::string strKey,MAPKVINFOR& mapKValue);

    // --------------------------------------------------------------------------------------------------------
    // 函数名称：GetRedisList
    // 函数功能：通过KEY获得Redis中list内容，装入容器list中
    // 入口参数：std::string strKey                      查询字段名
    //           LISTKVINFO& listKValue                  返回list内容(list)
    // 返回结果：bool                                    处理是否成功
    // --------------------------------------------------------------------------------------------------------
    bool GetRedisList(std::string strKey,LISTKVINFO& listKValue);

    // --------------------------------------------------------------------------------------------------------
    // 函数名称：GetRedisKValue
    // 函数功能：获得指定key的value值
    // 入口参数：std::string strKey                      查询字段名
    //           std::string& strValue                   引用返回查询结果
    // 返回结果：bool                                    处理是否成功
    // --------------------------------------------------------------------------------------------------------
    bool GetRedisKValue(std::string strKey, std::string& strValue);

    // --------------------------------------------------------------------------------------------------------
    // 函数名称：SetRedisKValue
    // 函数功能：获得指定key的value值
    // 入口参数：std::string strKey                      查询字段名
    //           std::string strValue                    输入字段值
    // 返回结果：bool                                    处理是否成功
    // --------------------------------------------------------------------------------------------------------
    bool SetRedisKValue(std::string strKey, std::string strValue);
    
    // --------------------------------------------------------------------------------------------------------
    // 函数名称：ExcRedisCmd
    // 函数功能：执行redis命令
    // 入口参数：std::string strCmd                      redis命令内容
    // 返回结果：bool                                    处理是否成功
    // --------------------------------------------------------------------------------------------------------
    bool ExcRedisCmd(std::string strCmd);

    // --------------------------------------------------------------------------------------------------------
    // 函数名称：SubscribeMsg
    // 函数功能：订阅消息命令
    // 入口参数：std::string strSubscribeCmd             redis订阅命令
    // 返回结果：bool                                    处理是否成功
    // --------------------------------------------------------------------------------------------------------
    bool SubscribeMsg(std::string strSubscribeCmd,std::string & strValue);

    // --------------------------------------------------------------------------------------------------------
    // 函数名称：PublishMsg
    // 函数功能：订阅消息命令
    // 入口参数：std::string strPublishCmd               redis订阅命令
    // 返回结果：bool                                    处理是否成功
    // --------------------------------------------------------------------------------------------------------
    bool PublishMsg(std::string strPublishCmd,std::string & strValue);
};
