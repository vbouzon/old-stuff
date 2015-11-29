#ifndef		SINGLETON_H
#define		SINGLETON_H

template <typename T> class CSingleton
{
protected:
	CSingleton( void ) { }
	~CSingleton( void ) { }


public:
	static T *GetInstance( void )
	{
		if( m_Singleton == 0 )
			m_Singleton = new T;

		return ((T *)m_Singleton);
	}

	static void Kill( void )
	{
		if( m_Singleton != 0 )
		{
			delete m_Singleton;
			m_Singleton = 0;
		}
	}


protected:
	static T	*m_Singleton;

};

template <typename T> T *CSingleton<T>::m_Singleton = 0;


#endif
