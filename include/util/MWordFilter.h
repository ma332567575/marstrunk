/********************************************************************
	Copyright(C)
	filename:	MWordFilter.h
	created:	2015/3/15 10:29:21
	author:		YvesMa(ma332567575@163.com)	
	Description: �ؼ���(The keyword may be rude word)���˹��ߣ����̰߳�ȫ
                ʹ�����ṹ�Թؼ��ʴ洢�����Թ������ġ�Ӣ��(�����ִ�Сд)�����֡�������ţ����������ǵ��������
                Լ���ؼ����ļ���ʽ�����ļ��������£�
                �ؼ���1
                �ؼ���2
                �ؼ���3
                Ŀǰ�ؼ��ʹ��˲��������������ǣ����ؼ�����"������"��"��"ʱ�����Ҫ���˵ľ�����"��"�����ò��ᱻ���ˡ��������ԭ����
                Ŀǰֻ����һ�߹��˾����е�ÿ���ַ�������Ҫ�滻ʱ���滻�ˣ������߼�Ӧ�����£�
                while �α겻���ڹ��˾��ӳ��� do
                    while ���α괦��ʼ����������ĩβ�������ҵ���һ��Ҫ���˵Ĵ� do
                        �ҹ��˴�
                    end
                    �����α꣬�������ҵ�û�ҵ����˴��йأ��Ҹ��˾ͼӹ��˴ʳ��ȣ���������α괦�ַ��Ƿ��Ǻ��ֽ���+2 or +1
                end
                ���Ǹ��߼��������ص��������⣬������ʱ����ȡ���ֽ������

	History:
	1. 2015/3/15 10:29:21 Create file
	...

*********************************************************************/
#ifndef MWordFilter_h__
#define MWordFilter_h__

#include "mars_stdinclude.h"
#include "MSyncTool.h"

namespace Mars
{
    enum MarsFiniteStateMErrn
    {
        MarsFiniteStateMErrn_NoErr = 0,
        MarsFiniteStateMErrn_Illegal_Param,
        MarsFiniteStateMErrn_Already_Init,
        MarsFiniteStateMErrn_Loadfile_Failed,
    };

    class MarsFiniteStateM;
    class MWordFilter
    {
    public:
        MWordFilter( MVOID );
        //mark:���ṩ���ֹ��캯������Ϊ��Ҫ��Ƴ�����c++��׼���iostream�����������ִ�к�load szPath�Ƿ�ɹ�
        //MWordFilter( MARS_IN MLPCSTR szPath );

        ~MWordFilter( MVOID );

    public:
        MarsFiniteStateMErrn Load( MARS_IN MLPCSTR szPath );
        std::string Filter( MARS_IN MCONST string& szSrc );
    
    private:
        MarsFiniteStateM* m_Fsm; //ֻ����δ�����class���ڰ����ͣ���static��Ա����ֻ������Ϊ�����͵�ָ�롢����
        MARS_CRITICAL_SECTION m_CritialSection;
        MBOOL m_IsInit;

    };
}


#endif // MWordFilter_h__