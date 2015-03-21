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

MBOOL netlib_listen( MLPCSTR szIp, MUI16 uPort, ListenCallback pCallback, MVOID* pCallbackData );

MBOOL netlib_send( net_handle_t Socket, MVOID* pBuf, MI64 nLen );

MBOOL netlib_close( net_handle_t Socket );

#endif // netlib_h__