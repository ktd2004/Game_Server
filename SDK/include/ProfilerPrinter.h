/** 
* ������ �������� ����Ÿ�� ����ϱ� ���ؼ� ���Ǵ� Ŭ����
*/
#pragma once

#include <ProfilerVisitor.h>

class ProfilerPrinter
{
public:
	// 	printDuration
	// 	�ð� ����(����: msec) �̻�Ǹ� VisitAndUpdteHistory() �ÿ� visitor�� ����Ѵ�. (�ƴϸ� Null visitor�� ����Ѵ�.)
	// 	visitor
	// 	��¿� ����� ProfilerVisitor
	// 	stackCnt
	// 	�� ���� ��ŭ�� print�� �Ҹ��� �ѹ��� VisitAndUpdteHistory() �� �Ҹ���.
	ProfilerPrinter(ProfilerVisitor * visitor, uint32 printDuration, uint32 stackCnt);

	// ���� �������ϸ��� ����Ÿ�� ����Ѵ�.
	void Print();

private:
	const uint32 m_Duration;				///< msec������ ���� ��� duration
	const uint32 m_StackCnt;				///< ��� ������ �����͸� ����� ���ΰ�?
	ProfilerVisitor * const m_Visitor;
	ProfilerVisitorNull m_NullVisitor;

	bool m_IsInit;
	uint64 m_LastPrintTick;
	uint32 m_PrintCnt;
};

//////////////////////////////////////////////////////////////////////////
// ProfilerPrinter ���� ������ Ƚ���� ���� �ʰ� �ð����� �ϴ� ��.
class ProfilerPrinterDuration
{
public:
	ProfilerPrinterDuration(ProfilerVisitor * visitor, uint32 printDuration,  uint32 stackDuration, uint32 bootingTime = 0);

	void Print();

	// ĸ�� ���¸� �����Ѵ�.
	void SetCapture();

private:
	const uint32		m_Duration;					///< msec������ ���� ��� duration
	const uint32		m_StackDuration;			///< �� msec ������ ��踦 ������ ���ΰ�?
	const uint32		m_BootingTime;				///< �ʱ� ���� Ÿ������ �󸶸� ����� ���ΰ�?
	ProfilerVisitor*	const m_Visitor;
	ProfilerVisitorNull m_NullVisitor;

	bool m_IsInit;
	uint64 m_LastPrintTick;
	uint64 m_VisitTick;
};

//////////////////////////////////////////////////////////////////////////
// �������ϸ� ���� ��ũ��
// �ڵ����� AutoFuncProfile ��ü�� automatic variable�� ������ �ش�.
// ���� �̸��� ���� uniqueness�� ����ȴ�.
#define PROFILER_AUTO_FUNC(name)		AutoFuncProfile _autoFuncProfile##__LINE__ (name)
// ProfilerPrinter �� ���ؼ� ����� ����Ѵ�.
#define PROFILER_PRINT(printer)		(printer).Print();