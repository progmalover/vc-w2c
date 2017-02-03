#pragma once 

template<class T>
class StaticInst
{
	static T* m_pInstance;
public:
	static T* Instance()
	{
		if(!m_pInstance)
			m_pInstance = new T();
		return m_pInstance;
	}
	static T* Instance(T* pObject)
	{
		if(pObject!=m_pInstance)
		{
			if(m_pInstance)
			{
				delete m_pInstance;
				m_pInstance = NULL;
			}
		}
		return m_pInstance = pObject;
	}
	static void ReleaseInstance()
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = 0;
		}
	}
};


template<class T>
T* StaticInst<T>::m_pInstance = 0;