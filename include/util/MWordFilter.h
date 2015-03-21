/********************************************************************
	Copyright(C)
	filename:	MWordFilter.h
	created:	2015/3/15 10:29:21
	author:		YvesMa(ma332567575@163.com)	
	Description: 关键词(The keyword may be rude word)过滤工具，多线程安全
                使用树结构对关键词存储，可以过滤中文、英文(不区分大小写)、数字、特殊符号，或者是它们的自由组合
                约定关键字文件格式，则文件内容如下：
                关键词1
                关键词2
                关键词3
                目前关键词过滤不够完美，问题是：当关键字是"我用我"、"用"时，如果要过滤的句子是"用"，则用不会被过滤。问题产生原因是
                目前只遍历一边过滤句子中的每个字符，发现要替换时就替换了，正常逻辑应该如下：
                while 游标不等于过滤句子长度 do
                    while 从游标处开始遍历到句子末尾，或者找到了一个要过滤的词 do
                        找过滤词
                    end
                    增加游标，增量跟找到没找到过滤词有关，找个了就加过滤词长度，否则根据游标处字符是否是汉字进行+2 or +1
                end
                但是该逻辑存在严重的性能问题，所以暂时不采取这种解决方案

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
        //mark:不提供这种构造函数，因为需要设计成类似c++标准库的iostream才能清楚构造执行后load szPath是否成功
        //MWordFilter( MARS_IN MLPCSTR szPath );

        ~MWordFilter( MVOID );

    public:
        MarsFiniteStateMErrn Load( MARS_IN MLPCSTR szPath );
        std::string Filter( MARS_IN MCONST string& szSrc );
    
    private:
        MarsFiniteStateM* m_Fsm; //只声明未定义的class属于半类型，非static成员变量只是声明为该类型的指针、引用
        MARS_CRITICAL_SECTION m_CritialSection;
        MBOOL m_IsInit;

    };
}


#endif // MWordFilter_h__