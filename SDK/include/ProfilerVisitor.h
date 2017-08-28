/** 
* Profiler Ŭ������ ������ ���� ����Ÿ�� tree ������ �����Ѵ�. �� node �� ���� node ��
* ���� ��忡 ���ӵǴµ�, �̷��� ���Ӽ��� callstack�� ��Ÿ����. ���� ���
* a() �Լ����� b() �Լ��� ȣ���ϸ�, tree�� a node �Ʒ� b node �� ����ȴ�.
*
* Vistor Ŭ������ �̷��� tree �� ��ȸ�ϱ� ���ؼ� ���Ǵ� Ŭ�����̸�,
* ProfilerVisitor Ŭ������ ��ӹ޾Ƽ� Visitor Ŭ������ �������� ProfilerPrintDuration Ŭ������
* �Ѱ��ָ�, PROFILER_PRINT ��ũ�θ� ȣ���Ҷ�, ��ü ��带 ��ȸ�ϸ�, ������ ���� ������ 
* ���� �� ����ϰ� �ȴ�.
*/
#pragma once

#include <Profiler.h>

class ProfilerVisitor
{
public:
	ProfilerVisitor() : m_Updatable(true), m_IsSkipZeroCnt(false) {}

	//-----------------------------------------------------------------------------------------


	// Visitor�� �湮�� ������ �� ���� ó�� �Ҹ���.
	virtual void OnStart() {}
	

	// ��ϵ� ��� Func�� ���� ��带 Visit�� �ѹ� �Ҹ���.
	virtual void OnBeforeFunc(uint64 duration) {}


	// ��ϵ� ��� func�� ���ؼ� ������� �Ҹ���.
	virtual void OnFunc(const char * name, ProfileFunc & info, uint64 duration) { };

	// ��ϵ� ��� func�� ���� �湮�� �����ɶ� �ѹ� �Ҹ���.
	virtual void OnAfterFunc(uint64 duration) {}


	// callstack�� ����� �Լ� ȣ������� ���� ��带 Visit���� ���� ó������ �Ҹ���.
	virtual void OnBeforeTreeNode(uint64 duration) {}

	// callstack�� ��ϵ� ��� TreeNode�� ���ؼ� ������� �Ҹ���.
	// DFS ������ �Ҹ��� depth�� ���ؼ� root������ ���̸� �� �� �ִ�.
	virtual void OnTreeNode(const char * name, size_t depth, ProfileInfo & info, uint64 duration) { };

	// DFS ������ OnTreeNode()�� �Ҹ��� ���� child node�� ��� �湮�� ���� �Ҹ���.
	virtual void OnTreeNodeExit(const char * name, size_t depth) {}

	// callstack �� ��ϵ� ��� TreeNode�� ���� ��ȸ�� �������� ȣ��ȴ�.
	virtual void OnAfterTreeNode(uint64 duration) {}

	
	// Visitor�� �湮�� ��� ������ �Ҹ���.
	virtual void OnFinish() {}

	bool IsUpdatable() const { return m_Updatable; }
	void SetUpdatable(bool update) { m_Updatable = update; }

	bool IsSkipZeroCnt() const { return m_IsSkipZeroCnt; }
	void SetSkipZeroCnt( bool flag ) { m_IsSkipZeroCnt = flag; }

private:
	bool m_Updatable;			///< ��¸� �� �� update���� �ʴ� �������� �˻�.
	bool m_IsSkipZeroCnt;		///< 0�� ����� �Լ��� Callstack ��½� �����ϵ��� ��.
};

//////////////////////////////////////////////////////////////////////////
// �ƹ��͵� ����� ���� �ʰ� ���� ��� ������ update���ִ� Visitor Ŭ����.
class ProfilerVisitorNull : public ProfilerVisitor
{
public:
	virtual void OnTreeNode(const char * name, size_t depth, ProfileInfo & info, uint64 duration) override {}
	virtual void OnFunc(const char * name, ProfileFunc & info, uint64 duration) override {}
};

//////////////////////////////////////////////////////////////////////////
// ǥ�� ������� ���� ������ �����ִ� Visitor Ŭ����
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