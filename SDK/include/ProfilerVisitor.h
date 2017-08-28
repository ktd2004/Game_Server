/** 
* Profiler 클래스는 수집된 성능 데이타를 tree 구조로 관리한다. 각 node 의 하위 node 는
* 상위 노드에 종속되는데, 이러한 종속성은 callstack을 나타낸다. 예를 들면
* a() 함수에서 b() 함수를 호출하면, tree는 a node 아래 b node 가 저장된다.
*
* Vistor 클래스는 이러한 tree 를 순회하기 위해서 사용되는 클래스이며,
* ProfilerVisitor 클래스를 상속받아서 Visitor 클래스를 생성한후 ProfilerPrintDuration 클래스에
* 넘겨주면, PROFILER_PRINT 매크로를 호출할때, 전체 노드를 순회하며, 수집된 성능 정보를 
* 갱신 및 출력하게 된다.
*/
#pragma once

#include <Profiler.h>

class ProfilerVisitor
{
public:
	ProfilerVisitor() : m_Updatable(true), m_IsSkipZeroCnt(false) {}

	//-----------------------------------------------------------------------------------------


	// Visitor의 방문이 시작할 때 제일 처음 불린다.
	virtual void OnStart() {}
	

	// 등록된 모든 Func에 대한 노드를 Visit전 한번 불린다.
	virtual void OnBeforeFunc(uint64 duration) {}


	// 등록된 모든 func에 대해서 순서대로 불린다.
	virtual void OnFunc(const char * name, ProfileFunc & info, uint64 duration) { };

	// 등록된 모든 func에 대한 방문이 마감될때 한번 불린다.
	virtual void OnAfterFunc(uint64 duration) {}


	// callstack의 저장된 함수 호출순서에 따라 노드를 Visit전에 제일 처음으로 불린다.
	virtual void OnBeforeTreeNode(uint64 duration) {}

	// callstack에 등록된 모든 TreeNode에 대해서 순서대로 불린다.
	// DFS 순서로 불리며 depth를 통해서 root에서의 깊이를 알 수 있다.
	virtual void OnTreeNode(const char * name, size_t depth, ProfileInfo & info, uint64 duration) { };

	// DFS 순서로 OnTreeNode()이 불릴때 하위 child node를 모두 방문한 다음 불린다.
	virtual void OnTreeNodeExit(const char * name, size_t depth) {}

	// callstack 에 등록된 모든 TreeNode에 대한 순회가 마감된후 호출된다.
	virtual void OnAfterTreeNode(uint64 duration) {}

	
	// Visitor의 방문이 모두 끝나면 불린다.
	virtual void OnFinish() {}

	bool IsUpdatable() const { return m_Updatable; }
	void SetUpdatable(bool update) { m_Updatable = update; }

	bool IsSkipZeroCnt() const { return m_IsSkipZeroCnt; }
	void SetSkipZeroCnt( bool flag ) { m_IsSkipZeroCnt = flag; }

private:
	bool m_Updatable;			///< 출력만 할 뿐 update하지 않는 상태인지 검사.
	bool m_IsSkipZeroCnt;		///< 0번 수행된 함수를 Callstack 출력시 생략하도록 함.
};

//////////////////////////////////////////////////////////////////////////
// 아무것도 출력해 주지 않고 내부 통계 정보만 update해주는 Visitor 클래스.
class ProfilerVisitorNull : public ProfilerVisitor
{
public:
	virtual void OnTreeNode(const char * name, size_t depth, ProfileInfo & info, uint64 duration) override {}
	virtual void OnFunc(const char * name, ProfileFunc & info, uint64 duration) override {}
};

//////////////////////////////////////////////////////////////////////////
// 표준 출력으로 성능 정보를 보여주는 Visitor 클래스
class ProfilerVisitorRunLog : public ProfilerVisitor
{
public:
	ProfilerVisitorRunLog(const_string & category, size_t historyLevel = MAX_HISTORY);
	virtual void OnStart() override;
	virtual void OnBeforeTreeNode(uint64 duration) override;
	virtual void OnAfterTreeNode(uint64 duration) override;
	virtual void OnTreeNode(const char * name, size_t depth, ProfileInfo & info, uint64 duration) override;
	virtual void OnBeforeFunc(uint64 duration) override;
	virtual void OnAfterFunc(uint64 duration) override;
	virtual void OnFunc(const char * name, ProfileFunc & info, uint64 duration) override;
	virtual void OnFinish() override;

private:
	const_string m_Category;
	const size_t m_HistoryLevel;

private:
	void PrintInfo(const char * name, size_t tab, ProfileInfo & info, uint64 duration);
	void PrintInfoFunc(const char * name, size_t tab, ProfileFunc & info, uint64 duration);
};