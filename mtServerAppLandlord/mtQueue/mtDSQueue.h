#ifndef		__MT_DS_QUEUE_H
#define		__MT_DS_QUEUE_H
#include "mtPlatform.h"
#include "mtException.h"

template<class _Tclass, int _iNodeNumTotal>
class mtDSQueue : public mtException
{
public:
	enum 
	{
		E_EXCEPTION_BEG	= 0,
		E_EXCEPTION_INIT_CRITICAL_SECTION_FAILED,
		E_EXCEPTION_END
	};
public:
	mtDSQueue();
	~mtDSQueue();

	int		init(int iSpinCount);
	int		exit(void);

	int		push(_In_ _Tclass* pT);
	int		push(_In_ _Tclass* pTList, int iTListNum);
	int		pop(_Out_ _Tclass* pT);
	int		pop(_In_ _Tclass* pTList, int iTListNum);

	int		remove(_In_ _Tclass* pT);

	int		getNodeNumUsed();

	virtual int exception();

	_Tclass		m_pTHead[_iNodeNumTotal];	/// 数组头部

	int		m_iNodeIdxFront;			/// 弹出点下表
	int		m_iNodeIdxBack;				/// 插入点下表
	const int	mc_iNodeNumTotal; 			/// 结点总数
	int		m_iNodeNumUsed;				/// 当前已经使用了的结点数目

	CRITICAL_SECTION	m_kCriticalSection;
	int		m_iRetCirticalSection;
};

 /************************************************************************
  * @function:	pop                                                     
  * @describe:	从队列中弹出一个元素
  * @param-out:	pT
  * @return:	
 		-成功			> 0
 		-pT为空			-1
 		-队列为空		-2
  * @author:		fuke
  ************************************************************************
  */
template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::pop( _In_ _Tclass* pTList, int iTListNum )
{
	if (NULL == pTList)
	{
		return	-1;
	}

	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed < iTListNum)
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	m_iNodeNumUsed	-= iTListNum;

	int		iNodeIdx;
	for (iNodeIdx = 0; iNodeIdx < iTListNum; iNodeIdx++)
	{
		pTList[iNodeIdx]		= m_pTHead[m_iNodeIdxBack++];

		if (m_iNodeIdxBack >= mc_iNodeNumTotal)
		{
			m_iNodeIdxBack	-= mc_iNodeNumTotal;
		}
	}

	LeaveCriticalSection(&m_kCriticalSection);

	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
int	mtDSQueue<_Tclass, _iNodeNumTotal>::push(_In_ _Tclass* pTList, int iTListNum)
{
	if (NULL == pTList)
	{
		return	-1;	
	}

	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed >= (mc_iNodeNumTotal - iTListNum + 1))
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	m_iNodeNumUsed += iTListNum;

	int		iNodeIdx;
	for (iNodeIdx = 0; iNodeIdx < iTListNum; iNodeIdx++)
	{
		m_pTHead[m_iNodeIdxFront++] = pTList[iNodeIdx];

		if (m_iNodeIdxFront >= mc_iNodeNumTotal)
		{
			m_iNodeIdxFront	-= mc_iNodeNumTotal;
		}
	}

	LeaveCriticalSection(&m_kCriticalSection);
	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::getNodeNumUsed()
{
	return	m_iNodeNumUsed;
}

template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::exception()
{
	switch (m_iExceptionId)
	{
		MT_EXCETPION_STRING(E_EXCEPTION_INIT_CRITICAL_SECTION_FAILED)
	default:
		MT_DEBUG(7, "该异常没有被处理!\n");
		break;
	}

	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::exit( void )
{
	if (0 != m_iRetCirticalSection) 
	{
		DeleteCriticalSection(&m_kCriticalSection);
	}

	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::init( int iSpinCount )
{
	m_iRetCirticalSection	= InitializeCriticalSectionAndSpinCount(&m_kCriticalSection, iSpinCount);
	MT_EXCEPTION_DEBUG(0 == m_iRetCirticalSection, E_EXCEPTION_INIT_CRITICAL_SECTION_FAILED);
	return	1;
}

 /************************************************************************
  * @function:	pop                                                     
  * @describe:	从队列中弹出一个元素
  * @param-out:	pT
  * @return:	
 		-成功			> 0
 		-pT为空			-1
 		-队列为空		-2
  * @author:		fuke
  ************************************************************************
  */
template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::pop( _Out_ _Tclass* pT )
{
	if (NULL == pT)
	{
		return	-1;
	}

//	TryEnterCriticalSection(&m_kCriticalSection);
	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed <= 0)
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	m_iNodeNumUsed	--;
	*pT		= m_pTHead[m_iNodeIdxBack++];

	if (m_iNodeIdxBack >= mc_iNodeNumTotal)
	{
		m_iNodeIdxBack	-= mc_iNodeNumTotal;
	}

	LeaveCriticalSection(&m_kCriticalSection);
	return	1;
}

 /************************************************************************
  * @function:	push                                                     
  * @describe:	向队列中压入一个元素
  * @param-in:	pT
  * @return:	
 		-成功			> 0
 		-pT为空			-1
 		-队列已满		-2
  * @author:		fuke
  ************************************************************************
  */
template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::push( _In_ _Tclass* pT )
{
	if (NULL == pT)
	{
		return	-1;	
	}

//	TryEnterCriticalSection(&m_kCriticalSection);
	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed >= mc_iNodeNumTotal)
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	m_iNodeNumUsed ++;
	m_pTHead[m_iNodeIdxFront++]	= *pT;

	if (m_iNodeIdxFront >= mc_iNodeNumTotal)
	{
		m_iNodeIdxFront	-= mc_iNodeNumTotal;
	}

	LeaveCriticalSection(&m_kCriticalSection);
	return	1;

}

template<class _Tclass, int _iNodeNumTotal>
int	mtDSQueue<_Tclass, _iNodeNumTotal>::remove(_In_ _Tclass* pT)
{
	if (NULL == pT)
	{
		return	-1;	
	}

	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed <= 0)
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	int iCount = m_iNodeNumUsed;
	while (0 < iCount)
	{
		/// 先取出一个
		m_iNodeNumUsed	--;
		_Tclass tTmp;
		tTmp	= m_pTHead[m_iNodeIdxBack++];
		if (m_iNodeIdxBack >= mc_iNodeNumTotal)
		{
			m_iNodeIdxBack	-= mc_iNodeNumTotal;
		}

		if (tTmp == *pT)
		{
			break;
		}

		/// 不是想要的，再放进队列里去
		m_iNodeNumUsed ++;
		m_pTHead[m_iNodeIdxFront++]	= tTmp;
		if (m_iNodeIdxFront >= mc_iNodeNumTotal)
		{
			m_iNodeIdxFront	-= mc_iNodeNumTotal;
		}

		iCount--;
	}

	LeaveCriticalSection(&m_kCriticalSection);
	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
mtDSQueue<_Tclass, _iNodeNumTotal>::~mtDSQueue()
{

}

template<class _Tclass, int _iNodeNumTotal>
mtDSQueue<_Tclass, _iNodeNumTotal>::mtDSQueue() 
	: mc_iNodeNumTotal(_iNodeNumTotal)
{
	m_iNodeIdxFront		= 0;
	m_iNodeIdxBack		= 0;
	m_iNodeNumUsed		= 0;

	m_iRetCirticalSection	= 0;
}

#endif	///	__MT_DS_QUEUE_H