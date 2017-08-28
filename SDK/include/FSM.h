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
#include <Ptr.h>

namespace FSM
{
	class State;
	class Machine;

	#define STATE_CODE(x)	virtual int GetID(){ return x; }

	/** 
		상태정보를 관리하는 클래스
	*/
	class State
	{
		int		m_iSt;
	public:
		State();
		State( int iSt );
		virtual ~State();

	public:
		/** 
			fsm_call 호출하면 이벤트가 발생했다는 것을 알려주기 위해서 호출
			@param m 호출자 주소값
		*/
		virtual State* OnEvent( Machine* m );
		virtual int GetID() { return m_iSt; }
	public:
		/** 
			현재 상태를 변경한다.
			@param m 호출자 주소값
			@param pSt 변경할 상태
		*/
		virtual void Transition( Machine* m, State* pSt );
	};

	//////////////////////////////////////////////////////////////////////////
	/** 
		현재의 상태에 따른 이벤트 호출하는 클래스
	*/
	class Machine
	{
	private:
		State*	m_State;
		State	m_None;
	public:
		Machine();
		virtual ~Machine();

	public:
		/** 
			상태를 변경한다.
			@param pSt 변경할 상태 오브젝트 주소값
		*/
		virtual void Transition( State* pSt );
		/** 
			이벤트 발생했음을 알린다.
			@param pEvt 이벤트 오브젝트 주소값
		*/
		virtual State* Notify( void );
		/** 
			현재의 상태를 얻어온다.
		*/
		virtual State* GetState( void ) const;
		/** 
			현재의 상태를 리셋한다.
		*/
		virtual void Reset( void );
	};

	class Operator
	{
	public:
		enum
		{
			eOP_DO = 1,
			eOP_END = 2,
			eOP_IF = 3,
			eOP_NEXT = 4,
		};
		int		m_iOp;
		State*	m_State;
	public:
		Operator( State* st, int op );
		virtual ~Operator();
		virtual State* Run( Machine* m ) = 0;
	};

	class DO : public Operator
	{
	public:
		DO(State* st);
		virtual ~DO();
		virtual State* Run( Machine* m );
	};

	class NEXT : public Operator
	{
	public:
		NEXT(State* st);
		virtual ~NEXT();
		virtual State* Run( Machine* m );
	};

	class END : public Operator
	{
	public:
		END(State* st);
		virtual ~END();
		virtual State* Run( Machine* m );
	};

	class IF : public Operator
	{
	public:
		IF(State* st);
		virtual ~IF();
		virtual State* Run( Machine* m );
	};

	//////////////////////////////////////////////////////////////////////////

	class Manager
	{
	protected:
		Manager();
		Manager( const Manager& r );
		virtual ~Manager();
	public:
		struct OpElement
		{
			Operator* first_op;
			Operator* second_op;
			OpElement() : first_op(nullptr), second_op(nullptr) {}
			OpElement(const OpElement& r ) : first_op(r.first_op), second_op(r.second_op)	{ }
		};
		typedef std::vector<OpElement>	OperatorVec;
		OperatorVec m_OpArray;

	public:
		static Manager* instance();
		/**
			상태정보를 저장하는 배열의 크기를 재설정한다.
			@param iSize 배열의 크기
		*/
		void Resize( size_t iSize );
		/**
			이벤트를 처리할 상태정보를 등록한다.
		*/
		void Reg( Operator* first_op, Operator* second_op );
		/**
			이벤트에 해당하는 상태정보 오브젝트의 OnEvent함수를 호출한다.
			@param m 호출자 오브젝트
			@param iEvt 이벤트 번호
			@param iNextSt 이벤트 호출후의 상태
		*/
		bool Call( Machine* m );
	};
};

#define AI_SPECFIC( class_name )			class class_name; extern class_name class_name##_ai_inst;
#define AI_DELCARE( class_name )			class_name class_name##_ai_inst;
#define AI_PTR( class_name )				(FSM::State*)&class_name##_ai_inst
#define AI_SIZE( size )						FSM::Manager::instance()->Resize( size );
#define	AI_DEF( first_op, second_op )		FSM::Manager::instance()->Reg( first_op, second_op )
#define AI_CALL( machine )					FSM::Manager::instance()->Call( machine )

#define ai_do(class_name)		new FSM::DO(AI_PTR(class_name))
#define ai_next(class_name)		new FSM::NEXT(AI_PTR(class_name))
#define ai_if(class_name)		new FSM::IF(AI_PTR(class_name))
#define ai_end(class_name)		new FSM::END(AI_PTR(class_name))