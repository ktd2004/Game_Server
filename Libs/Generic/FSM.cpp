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
#include "stdafx.h"
#include <FSM.h>
#include <Logger.h>

namespace FSM
{
	State::State()
	{
	}

	State::State( int iSt )
		: m_iSt(iSt)
	{

	}

	State::~State()
	{
	}

	State* State::OnEvent( Machine* m )
	{
		return nullptr;
	}

	void State::Transition(Machine* m, State* pSt)
	{
		if (m)
		{
			m->Transition(pSt);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Machine::Machine()
		: m_State (&m_None)
	{
	}

	Machine::~Machine()
	{
	}

	State* Machine::GetState( void ) const
	{
		return m_State;
	}

	void Machine::Transition( State* st )
	{	
		if ( st )
			m_State = st;
	}

	State* Machine::Notify()
	{
		if ( m_State )
		{
			return m_State->OnEvent( this );
		}
		return nullptr;
	}

	void Machine::Reset( void )
	{
		m_State = &m_None;
	}

	//////////////////////////////////////////////////////////////////////////
	Manager* Manager::instance()
	{
		static Manager inst;
		return &inst;
	}

	Manager::Manager()
	{
		m_OpArray.resize(200);
	}

	Manager::Manager(const Manager& r)
	{
		m_OpArray.resize(200);
	}

	Manager::~Manager()
	{

	}

	void Manager::Resize(size_t iSize)
	{
		m_OpArray.resize( iSize );
	}

	void Manager::Reg( Operator* first_op, Operator* second_op )
	{
		// 첫번째 아규먼트는 next 가 올수 없다.
		if ( first_op->m_iOp == Operator::eOP_NEXT )
		{
			prn_err( "next opertor can't assigned first_op" );
			return;
		}

		if ( second_op->m_iOp == Operator::eOP_DO )
		{
			prn_err( "do opertor can't assigned second_op" );
			return;
		}

		if ( second_op->m_iOp == Operator::eOP_IF )
		{
			prn_err( "if opertor can't assigned second_op" );
			return;
		}

		OpElement t;
		t.first_op = first_op;
		t.second_op = second_op;
		safe_delete(m_OpArray[t.first_op->m_State->GetID()].first_op);
		safe_delete(m_OpArray[t.first_op->m_State->GetID()].second_op);
		m_OpArray[t.first_op->m_State->GetID()] = t;
	}

	bool Manager::Call( Machine* m )
	{
		while ( true )
		{
			State* st = m->GetState();
			if ( !st )
				return false;

			OpElement* elt = &m_OpArray[st->GetID()];
			if ( !elt )
			{
				prn_err( "ai state(%d) is not founded", st->GetID() );
				return false;
			}
			
			Operator* op = elt->first_op;
			if ( elt->first_op == nullptr )
			{
				prn_err( "ai state(%d) first_op is null", st->GetID() );
				return false;
			}
			if ( elt->second_op == nullptr )
			{
				prn_err( "ai state(%d) second_op is null", st->GetID() );
				return false;
			}

			switch( op->m_iOp )
			{
			case Operator::eOP_DO : // do
				op->Run( m );
				break;
			case Operator::eOP_END : // end
				op->Run( m );
				return true;
			case Operator::eOP_IF : // if
				st = op->Run( m );
// 				if ( st != nullptr )
// 					continue;
				return true;
			default :
				prn_err( "ai invalid first_op operator assigned" );
				return false;
			}

			op = elt->second_op;
			switch( op->m_iOp )
			{
			case Operator::eOP_END : // end
				op->Run( m );
				return true;
			case Operator::eOP_NEXT : // next
				op->Run( m );
				break;
			case Operator::eOP_IF : // if
				st = op->Run( m );
				break;
			default :
				prn_err( "ai invalid second_op operator assigned" );
				return false;
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////

	Operator::Operator(State* st, int op) : m_State(st), m_iOp(op)
	{

	}

	Operator::~Operator()
	{

	}


	DO::DO(State* st) : Operator(st, Operator::eOP_DO)
	{

	}

	DO::~DO()
	{

	}

	State* DO::Run(Machine* m)
	{
		m->Notify();
		return nullptr;
	}


	END::END(State* st) : Operator(st, Operator::eOP_END)
	{

	}

	END::~END()
	{

	}

	State* END::Run(Machine* m)
	{
		m->Transition( m_State );
		return nullptr;
	}


	IF::IF(State* st) : Operator(st, Operator::eOP_IF)
	{

	}

	IF::~IF()
	{

	}

	State* IF::Run(Machine* m)
	{
		State* st = m->Notify();
		m->Transition( st );
		return st;
	}


	NEXT::NEXT(State* st) : Operator(st, Operator::eOP_NEXT)
	{
	}

	NEXT::~NEXT()
	{

	}

	State* NEXT::Run(Machine* m)
	{
		m->Transition( m_State );
		return m_State;
	}
 };