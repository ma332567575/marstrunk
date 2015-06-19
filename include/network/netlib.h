/********************************************************************
	Copyright(C)
	filename:	netlib.h
	created:	2015/2/27 15:05:42
	author:		YvesMa(ma332567575@163.com)	
	Description:    首先，我提供的是一个工具，可以接收和发送消息的工具，那么我只要处理好net这一层的功能就行了
                比如：我不用关心我要给某个玩家发送消息，我只需要提供接口能够给某个socket发消息就行了。考虑了一下，
                我需要提供下面的功能：
                为服务端提供功能
                · 监听指定Ip和Port，监听操作成功与否作为结果，剩下accept相关的操作不需要用户关心，网络库自己管理
                作为服务端，需要给指定客户端发送消息，这时给哪个客户端发送是需要指定一个socket的，而如何跟socke关联，我可以封装出个XXClient，这个
                Client拥有一个SendMsg方法。那么这个XXClient是何时创建的呢？应该是Accept成功时，为了让用户能够管理到这个Client，当Accept成功时，我给
                用户一个事件通知，让用户自己去设置这个事件通知的行为，这里设置方式基本就是让用户可以设置一个回调，事件发生时我调用这个回调。
                · 给指定客户端发送消息，用户自己指定id以及消息体(protobuf)
                · 接收客户端消息，获取id以及消息体(protobuf)

                为客户端提供功能
                · 连接指定的Ip和Port，把连接后的socket作为结果返回

	History:
	1. 2015/2/27 15:05:42 Create file
	...

*********************************************************************/

#ifndef netlib_h__
#define netlib_h__

#include "common.h"

class IServer
{
public:
  IServer( MVOID );
  virtual ~IServer( MVOID );

public:
  // A client connected event
  // @Result    true 
  //            false   Consider the client is error
  virtual MBOOL OnListen( MUI16 uServerPort, MLPCSTR szClientIp, MUI16 uClientPort, net_handle_t handleClient ) = 0;
  // A client data can be received
  virtual MVOID OnReceive( net_handle_t handleClient, MPVOID szBuff, MI32 nBuffLen ) = 0;
  // A client close
  virtual void OnClose( net_handle_t handleClient ) = 0;

};

/********************************************
 * Function
 * Description: You cant call this function repeatly once calling the function successfully.
 * 
 * @Param pPortList: port list, the ports you want listen.
 * @Param pServer: Event pump
 * @Result: MTRUE  Suc
 *          MFALSE Failed
 *
 ********************************************/
MBOOL netlib_listen( MUI16* pPortList, MI32 nPortNum, IServer* pServer );

/********************************************
 * Function
 * Description: Send data to a socket 
 * 
 * @Param Socket: Object socket
 * @Param pBuf: the data you want send
 * @Param nLen: the size of data you want send
 * @Result: MTRUE  Suc
 *          MFALSE Failed
 *
 ********************************************/
MBOOL netlib_send( net_handle_t handleSocket, MPVOID pBuf, MI32 nLen );

/********************************************
 * Function
 * Description: Close a socket 
 * 
 * @Param Socket: Object socket
 * @Param pBuf: the data you want send
 * @Param nLen: the size of data you want send
 * @Result: MTRUE  Suc
 *          MFALSE Failed
 *
 ********************************************/
MBOOL netlib_close( net_handle_t handleSocket );

/********************************************
 * Function
 * Description: It's neccesary. Perform the network module. It will deal the network io.If there are msg, the callback that user set will be triggered.
 * 
 * @Result: MTRUE  Suc
 *          MFALSE Failed
 *
 ********************************************/
MBOOL netlib_oneloop( MVOID );

#endif // netlib_h__
