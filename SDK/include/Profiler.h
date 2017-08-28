/* Copyright 2013 by Lee yong jun
 * All rights reserved
 *
 * Distribute freely, except: don't remove my name from the source or
 * documentation (don't take credit for my work), mark your changes (don't
 * get me blamed for your possible bugs), don't alter or remove this
 * notice.  May be sold if buildable source is provided to buyer.  No
 * warrantee of any kind, express or implied, is included with this
 * software; use at your own risk, responsibility for damages (if any) to
 * anyone resulting from the use of this software rests entirely with the
 * user.
 *
 * Send bug reports, bug fixes, enhancements, requests, flames, etc., and
 * I'll try to keep a version up to date.  I can be reached as follows:
 * Lee yong jun          iamhere01@naver.com
 */
#pragma once

#include <DataTypes.h>
#include <FastList.h>

//////////////////////////////////////////////////////////////////////////
struct Visitor;
class FuncNode : public listnode(FuncNode)
{
public:
	const std::string	m_sName;
	int32				m_iDepth;
	int32				m_iCalled;				///< 호출횟수
	uint64				m_iTimeFreq;			///< CPU Freq
	uint64				m_iBeginTime;			///< 호출시작 tick
	float				m_fAvgTime;				///< 평균시간(sec)
	float				m_fAccumulatedTime;		///< 누적시간(sec)
	float				m_fMaxTime;				///< 최대시간(sec)
	float				m_fMinTime;				///< 최소시간(sec)
	float				m_fVarianceTime;		///< 분산시간(sec)

	typedef std::vector<FuncNode*>	ChildNode;
	ChildNode			m_Childs;

public:
	FuncNode( const char* pszFuncName, int iDepth );
	virtual ~FuncNode();

	void	BeginFunc( void );
	void	EndFunc( void );
	void	OnVisit( Visitor* pVisitor );

};

//////////////////////////////////////////////////////////////////////////
struct Visitor
{
	virtual void OnVisit( FuncNode* pNode ) = 0;
};

class PrintScreen : public Visitor
{
public:
	PrintScreen();
	virtual ~PrintScreen();

	void OnVisit( FuncNode* pNode );
};

//////////////////////////////////////////////////////////////////////////
class Profiler
{
private:
	typedef util::list<FuncNode>	Frame;
	Frame		m_Frame;
	typedef std::vector<FuncNode*>	CallStacks;
	CallStacks	m_CallStacks;

	typedef std::map<const char*, FuncNode*>	FuncMap;
	FuncMap		m_FuncMap;

	Visitor*	m_pVisitor;

private:
	Profiler();
	Profiler( const Profiler& copy );
	virtual ~Profiler();
	void	Destroy( void );

	static __declspec(thread) Profiler * m_This;

public:
	static Profiler* instance( void );

	void	SetVisitor( Visitor* pVisitor );

	void	BeginFunc( const char* pszFuncName );
	void	EndFunc( const char* pszFuncName );

	void	Visit( void );
};

//////////////////////////////////////////////////////////////////////////
class AutoProfiler
{
private:
	const char*		m_pszFuncName;

public:
	AutoProfiler( const char* pszFuncName );
	virtual ~AutoProfiler();
};

#define PF_FN			AutoProfiler _autoFuncProfile##__LINE__ (__FUNCTION__)
#define PF_NM(name)		AutoProfiler _autoFuncProfile##__LINE__ (name)
#define PF_VISITOR(v)	Profiler::instance()->SetVisitor(v);
#define PF_PRINT()		Profiler::instance()->Visit()