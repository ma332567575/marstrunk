/********************************************************************
	Copyright(C)
	filename:	singleton.h
	created:	2015/1/13 20:53:11
	author:		YvesMa(ma332567575@163.com)	
	Description: Singleton tool class

	History:
	1. 2015/1/13 20:53:11 Create file
	...

*********************************************************************/

#ifndef singleton_h__
#define singleton_h__

template<class T>
class CSingleton
{
public:
    static inline T* Instance( void );
    void Release( void );

protected:
    CSingleton( void ){ }
    ~CSingleton( void ){ }

    static T* m_Instance;

};

template<class T>
inline T* CSingleton<T>::Instance( void )
{
    if ( !m_Instance )
        m_Instance = new T;

    return m_Instance;
}

template<class T>
void CSingleton<T>::Release( void )
{
    if ( !m_Instance )
        return;

    delete m_Instance;
    m_Instance = 0;
}

#define DECLARE_SINGLETON_MEBER(_Ty) \
    template <> _Ty* CSingleton<_Ty>::m_Instance = NULL;

#endif // singleton_h__