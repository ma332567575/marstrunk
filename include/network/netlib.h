/********************************************************************
	Copyright(C)
	filename:	netlib.h
	created:	2015/2/27 15:05:42
	author:		YvesMa(ma332567575@163.com)	
	Description:    ���ȣ����ṩ����һ�����ߣ����Խ��պͷ�����Ϣ�Ĺ��ߣ���ô��ֻҪ�����net��һ��Ĺ��ܾ�����
                ���磺�Ҳ��ù�����Ҫ��ĳ����ҷ�����Ϣ����ֻ��Ҫ�ṩ�ӿ��ܹ���ĳ��socket����Ϣ�����ˡ�������һ�£�
                ����Ҫ�ṩ����Ĺ��ܣ�
                Ϊ������ṩ����
                �� ����ָ��Ip��Port�����������ɹ������Ϊ�����ʣ��accept��صĲ�������Ҫ�û����ģ�������Լ�����
                ��Ϊ����ˣ���Ҫ��ָ���ͻ��˷�����Ϣ����ʱ���ĸ��ͻ��˷�������Ҫָ��һ��socket�ģ�����θ�socke�������ҿ��Է�װ����XXClient�����
                Clientӵ��һ��SendMsg��������ô���XXClient�Ǻ�ʱ�������أ�Ӧ����Accept�ɹ�ʱ��Ϊ�����û��ܹ��������Client����Accept�ɹ�ʱ���Ҹ�
                �û�һ���¼�֪ͨ�����û��Լ�ȥ��������¼�֪ͨ����Ϊ���������÷�ʽ�����������û���������һ���ص����¼�����ʱ�ҵ�������ص���
                �� ��ָ���ͻ��˷�����Ϣ���û��Լ�ָ��id�Լ���Ϣ��(protobuf)
                �� ���տͻ�����Ϣ����ȡid�Լ���Ϣ��(protobuf)

                Ϊ�ͻ����ṩ����
                �� ����ָ����Ip��Port�������Ӻ��socket��Ϊ�������

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
