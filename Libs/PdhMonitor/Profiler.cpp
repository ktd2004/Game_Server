#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <Mmsystem.h>
#include <MacroFunc.h>
#include <Profiler.h>

FuncNode::FuncNode( const char* pszFuncName, int32 iDepth )
	: m_sName(pszFuncName)
	, m_iDepth(iDepth)
	, m_iCalled(0)
	, m_fAccumulatedTime(0.0f)
	, m_fAvgTime(0.0f)
	, m_fMaxTime(0.0f)
	, m_fMinTime(1000000.0f)
	, m_fVarianceTime(0.0f)
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&m_iTimeFreq));
}

FuncNode::~FuncNode()
{
	for ( size_t i = 0; i < m_Childs.size(); ++i )
	{
		FuncNode* p = m_Childs[i];
		safe_delete(p);
	}
	m_Childs.clear();
}


void FuncNode::BeginFunc( void )
{
	m_iCalled++;
	QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>(&m_iBeginTime) );
}

void FuncNode::EndFunc( )
{
	uint64 iNow;
	QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>(&iNow) );
	// 소요시간
	float fElapseTime = (float)((float)(iNow - m_iBeginTime)/(float)m_iTimeFreq);
	// 누적시간
	m_fAccumulatedTime += fElapseTime;
	// 평균
	m_fAvgTime = (float)(m_fAccumulatedTime / (float)m_iCalled);
	// 분산
	m_fVarianceTime = (m_fAvgTime - fElapseTime) / (float)m_iCalled;
	// 최소/최대 시간
	if ( m_fMaxTime < fElapseTime ) m_fMaxTime = fElapseTime;
	if ( m_fMinTime > fElapseTime ) m_fMinTime = fElapseTime;
}

void FuncNode::OnVisit( Visitor* pVisitor )
{
	if ( pVisitor )
	{
// 		std::string blank1( m_iDepth, ' ' );
// 		printf( "%s%s\t%d\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n", cstr(blank1), cstr(m_sName), m_iCalled, m_fAccumulatedTime, m_fAvgTime, m_fMinTime, m_fMaxTime, m_fVarianceTime );
		pVisitor->OnVisit( this );
		for ( size_t i = 0; i < m_Childs.size(); ++i )
		{
			FuncNode* p = m_Childs[i];
			if ( p )
			{
				p->OnVisit( pVisitor );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
PrintScreen::PrintScreen()
{

}

PrintScreen::~PrintScreen()
{

}


void PrintScreen::OnVisit( FuncNode* pNode )
{
	std::string blank1( pNode->m_iDepth, ' ' );
	printf( "%s%s\t%d\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n",
		cstr(blank1),
		cstr(pNode->m_sName),		// 함수이름
		pNode->m_iCalled,			// 호출횟수
		pNode->m_fAccumulatedTime,	// 누적시간
		pNode->m_fAvgTime,			// 평균시간
		pNode->m_fMinTime,			// 최소시간
		pNode->m_fMaxTime,			// 최대시간
		pNode->m_fVarianceTime );	// 분산시간
}

//////////////////////////////////////////////////////////////////////////
Profiler * Profiler::m_This = nullptr;

Profiler* Profiler::instance( void )
{
	if (m_This == nullptr)
	{
		m_This = new Profiler;
	}

	return m_This;
}


Profiler::Profiler()
	: m_pVisitor(new PrintScreen)
{
}

Profiler::Profiler( const Profiler& copy )
	: m_pVisitor(new PrintScreen)
{

}

Profiler::~Profiler()
{
	Destroy();
	safe_delete(m_pVisitor);
}

void Profiler::Destroy( void )
{
	m_Frame.clear();
	for (size_t i = 0; i < m_CallStacks.size(); ++i)
	{
		FuncNode* p = m_CallStacks[i];
		safe_delete(p);
	}
	m_CallStacks.clear();
}

void Profiler::SetVisitor( Visitor* pVisitor )
{
	safe_delete(m_pVisitor);
	m_pVisitor = pVisitor;
}

void Profiler::BeginFunc( const char* pszFuncName )
{
	// 현재 프레임을 조사해서 호출중인 함수를 정보를 얻어온다.
	FuncNode* pNode = m_Frame.back();
	FuncNode* pNewNode = nullptr;
	if (pNode != nullptr)
	{
		for ( size_t i = 0; i < pNode->m_Childs.size(); ++i )
		{
			if ( pNode->m_Childs[i]->m_sName == pszFuncName )
			{
				pNode->m_Childs[i]->BeginFunc();
				m_Frame.push_back( pNode->m_Childs[i] );
				return;
			}
		}

		pNewNode = new FuncNode( pszFuncName, pNode->m_iDepth+1 );
		pNode->m_Childs.push_back( pNewNode );

		if ( m_FuncMap.count(pszFuncName) == 0 )
		{
			m_FuncMap[pszFuncName] = pNewNode;
		}

		pNewNode->BeginFunc();
		m_Frame.push_back( pNewNode );
	}
	else
	{
		if ( m_FuncMap.count(pszFuncName) == 0 )
		{
			pNewNode = new FuncNode( pszFuncName, 0 );
			m_CallStacks.push_back( pNewNode );
			m_FuncMap[pszFuncName] = pNewNode;
		}
		else
		{
			pNewNode = m_FuncMap[pszFuncName];
		}

		pNewNode->BeginFunc();
		m_Frame.push_back( pNewNode );
	}
}

void Profiler::EndFunc( const char* pszFuncName )
{
	FuncNode* pNode = m_Frame.back();
	if (pNode != nullptr && pNode->m_sName == pszFuncName)
	{
		pNode->EndFunc();
		m_Frame.pop_back();
	}
}

void Profiler::Visit( void )
{
	for ( size_t i = 0; i < m_CallStacks.size(); ++i )
	{
		FuncNode* p = m_CallStacks[i];
		if ( p )
		{
			if ( m_pVisitor )
			{
				p->OnVisit( m_pVisitor );
			}
		}
	}
}

AutoProfiler::AutoProfiler( const char* pszFuncName )
	: m_pszFuncName(pszFuncName)
{
	Profiler::instance()->BeginFunc( pszFuncName );
}

AutoProfiler::~AutoProfiler()
{
	Profiler::instance()->EndFunc( m_pszFuncName );
}