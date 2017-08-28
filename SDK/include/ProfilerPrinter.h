/** 
* 수집된 성능측정 데이타는 출력하기 위해서 사용되는 클래스
*/
#pragma once

#include <ProfilerVisitor.h>

class ProfilerPrinter
{
public:
	// 	printDuration
	// 	시간 간격(단위: msec) 이상되면 VisitAndUpdteHistory() 시에 visitor를 사용한다. (아니면 Null visitor를 사용한다.)
	// 	visitor
	// 	출력에 사용할 ProfilerVisitor
	// 	stackCnt
	// 	이 개수 만큼의 print가 불릴때 한번의 VisitAndUpdteHistory() 가 불린다.
	ProfilerPrinter(ProfilerVisitor * visitor, uint32 printDuration, uint32 stackCnt);

	// 성능 프로파일링한 데이타를 출력한다.
	void Print();

private:
	const uint32 m_Duration;				///< msec단위의 실제 출력 duration
	const uint32 m_StackCnt;				///< 몇번 누적한 데이터를 출력할 것인가?
	ProfilerVisitor * const m_Visitor;
	ProfilerVisitorNull m_NullVisitor;

	bool m_IsInit;
	uint64 m_LastPrintTick;
	uint32 m_PrintCnt;
};

//////////////////////////////////////////////////////////////////////////
// ProfilerPrinter 에서 누적을 횟수로 하지 않고 시간으로 하는 것.
class ProfilerPrinterDuration
{
public:
	ProfilerPrinterDuration(ProfilerVisitor * visitor, uint32 printDuration,  uint32 stackDuration, uint32 bootingTime = 0);

	void Print();

	// 캡쳐 상태를 설정한다.
	void SetCapture();

private:
	const uint32		m_Duration;					///< msec단위의 실제 출력 duration
	const uint32		m_StackDuration;			///< 몇 msec 단위로 통계를 누적할 것인가?
	const uint32		m_BootingTime;				///< 초기 부팅 타임으로 얼마를 고려할 것인가?
	ProfilerVisitor*	const m_Visitor;
	ProfilerVisitorNull m_NullVisitor;

	bool m_IsInit;
	uint64 m_LastPrintTick;
	uint64 m_VisitTick;
};

//////////////////////////////////////////////////////////////////////////
// 프로파일링 헐퍼 매크로
// 자동으로 AutoFuncProfile 객체를 automatic variable로 생성해 준다.
// 변수 이름에 대한 uniqueness가 보장된다.
#define PROFILER_AUTO_FUNC(name)		AutoFuncProfile _autoFuncProfile##__LINE__ (name)
// ProfilerPrinter 를 통해서 결과를 출력한다.
#define PROFILER_PRINT(printer)		(printer).Print();