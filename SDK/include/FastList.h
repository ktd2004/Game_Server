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

/** 
	유틸리티 모음을 위한 네임스페이스.
*/
namespace util
{
	/** 
		노드를 저장할때 별도의 메모리 할당 없어 빠른 저장 및 삭제를 보장하는 리스트 탬플릿 클래스.

		util::list에 저장할 노드는 util::list<클래스>::node 를 상속 받아야 한다. util::list는 저장될 노드의
		m_prev 와 m_next 맴버를 사용하여 double linked list 를 구현하므로 두개의 util::list 에 동시에 저장할 수 없다.

		아래의 사용예는 MyObj 클래스를 util::list에 저장하기 위한 방법을 보여준다.
		@verbatim
		class MyObj : public util::list<MyObj>::node
		{
			int		m_id;
		public:
			MyObj(int id) : m_id(id) {}
			virtual ~MyObj() {}
			void dump( void )
			{
				printf( "id = %d\n", m_id );
			}
		};

		int _tmain(int argc, _TCHAR* argv[])
		{
			util::list<MyObj> lst;

			for ( int i = 0; i < 10; i++ )
			{
				lst.push_back( new MyObj(i) );
			}

			for ( util::list<MyObj>::iterator it = lst.begin(); it != lst.end(); it++ )
			{
				it->dump();
			}

			while ( !lst.empty() )
			{
				MyObj* p = lst.front();
				lst.pop_front();
				delete p;
			}
			
			return 0;
		}
		@endverbatim
	*/
	template <class T1>	class list
	{
	public:
		/**
			util::list 에 저장될 노드에 반드시 상속해야 하는 클래스.

			상속받은 클래스는 m_prev, m_next 맴버가 포함된다.
		*/
		class node
		{
		public:
			T1*		m_prev;		///< 이전노드
			T1*		m_next;		///< 다음노드
			node()
				: m_prev(nullptr)
				, m_next(nullptr)
			{
			}
			~node()
			{
				m_prev = nullptr;
				m_next = nullptr;
			}
		};
		/** 노드 검색 조건 함수 */
		struct fncompare
		{
			virtual bool operator == (T1 *) = 0;
		};

	private:
		node		_Head;		///< 노드의 시작
		node		_Tail;		///< 노드의 마지막
		size_t		_count;		///< 저장된 노드 개수

	public :
		list() { clear(); }
		list(const list<T1>& r) { clear(); }
		virtual ~list() {}

		/** 
			util::list 의 반복자.
		*/
		class iterator
		{
		private:
			node * m_node;		///< 현재 참조하고 있는 노드

		public :
			iterator() : m_node(nullptr) {}
			iterator( T1 *elt ) : m_node( (node*)elt ) {}
			iterator( node *elt ) : m_node( elt ) {}
			~iterator() {}

			T1 * operator* () const { return (T1*)m_node; }
			T1 * operator-> () const { return (T1*)m_node; }

			/** 대입연산자 */
			iterator & operator= (const iterator& _Right)
			{
				m_node = _Right.m_node;
				return *this;
			}
			/** 후위 증가 연산자 */
			iterator & operator++ ()
			{
				m_node = m_node->m_next;
				return *this;
			}
			/** 전위 증가 연산자 */
			iterator operator++ (int)
			{
				iterator _tmp = *this;
				++*this;
				return _tmp;
			}
			/** 후위 감소 연산자 */
			iterator & operator-- ()
			{
				m_node = m_node->m_prev;
				return *this;
			}
			/** 전위 감소 연산자 */
			iterator operator-- (int)
			{
				iterator _tmp = *this;
				--*this;
				return _tmp;
			}
			/** 비교 연산자 */
			bool operator== (const iterator& _Right)
			{ return m_node == _Right.m_node; }
			/** 비교 연산자 */
			bool operator!= (const iterator& _Right)
			{ return (!(*this == _Right)); }
		};

		/** 리스트의 시작 */
		inline iterator begin()
		{
			return ( iterator(_Head.m_next) );
		}
		/** 리스트의 마지막 */
		inline iterator end()
		{
			return ( iterator(&_Tail) );
		}
		/** 리스트가 비어 있는지 조사 */
		inline bool empty()
		{
			return _Head.m_next == (T1*)&_Tail;
		}
		/** 리스트를 모두 지운다. */
		inline void clear()
		{
			_Head.m_prev = nullptr;
			_Head.m_next = (T1*)&_Tail;
			_Tail.m_prev = (T1*)&_Head;
			_Tail.m_next = nullptr;
			_count = 0;
		}
		/** 리스트를 모두 지운다. */
		inline T1 * front()
		{
			if ( empty() ) return nullptr;
			return _Head.m_next;
		}
		/** 리스트의 마지막을 참조 */
		inline T1 * back()
		{
			if ( empty() ) return nullptr;
			return _Tail.m_prev;
		}
		/** 리스트의 앞에 저장 */
		inline void push_front(T1 * _Val)
		{
			if ( _Val->m_prev )
				_Val->m_prev->m_next = _Val->m_next;
			if ( _Val->m_next )
				_Val->m_next->m_prev = _Val->m_prev;

			_Val->m_prev = (T1*)&_Head;
			_Val->m_next = _Head.m_next;
			_Head.m_next->m_prev = _Val;
			_Head.m_next = _Val;
			_count++;
		}
		/** 리스트의 앞에서 제거 */
		inline void	pop_front()
		{
			if ( empty() ) return;
			T1 *p = _Head.m_next;
			_Head.m_next = p->m_next;
			_Head.m_next->m_prev =(T1*)&_Head;
			p->m_prev = nullptr;
			p->m_next = nullptr;
			_count--;
		}
		/** 리스트의 뒤에 저장 */
		inline void push_back(T1 * _Val)
		{
			if ( _Val->m_prev )
				_Val->m_prev->m_next = _Val->m_next;
			if ( _Val->m_next )
				_Val->m_next->m_prev = _Val->m_prev;
			_Val->m_next = (T1*)&_Tail;
			_Val->m_prev = _Tail.m_prev;
			_Tail.m_prev->m_next = _Val;
			_Tail.m_prev = _Val;
			_count++;
		}
		/** 리스트의 뒤에서 제거 */
		inline void pop_back()
		{
			if ( empty() ) return;
			T1 *p = _Tail.m_prev;
			p->m_prev->m_next = (T1*)&_Tail;
			_Tail.m_prev = p->m_prev;
			p->m_next = nullptr;
			p->m_prev = nullptr;
			_count--;
		}
		/** 반복자가 가리키는 노드의 뒤로 삽입 */
		inline iterator	insert(iterator& _Where, T1 * _Val)
		{
			if ( _Val->m_prev )
				_Val->m_prev->m_next = _Val->m_next;
			if ( _Val->m_next )
				_Val->m_next->m_prev = _Val->m_prev;

			iterator it(*_Where);
			_Val->m_prev = _Where->m_prev;
			_Val->m_next = (*_Where);
			_Where->m_prev->m_next = _Val;
			_Where->m_prev = _Val;
			_count++;
			return ( iterator((*it)));
		}
		/** 반복자가 가리키는 노드를 삭제 */
		inline iterator	erase(iterator& _Where)
		{
			iterator it((*_Where)->m_next);
			_Where->m_next->m_prev = _Where->m_prev;
			_Where->m_prev->m_next = _Where->m_next;
			_Where->m_next = nullptr;
			_Where->m_prev = nullptr;
			_count--;
			return ( it );
		}
		/** 노드를 삭제 */
		inline void remove(T1 * _Val)
		{
			if ( empty() || !_Val->m_prev || !_Val->m_next ) return;
			erase( iterator(_Val) );
		}
		/** 리스트의 노드를 교체 */
		inline void swap(util::list<T1> &rlist)
		{
			util::list<T1> target;
			target.merge( rlist );
			rlist.merge( *this );
			merge( target );
		}
		/** 리스트의 노드를 병합 */
		inline void merge(util::list<T1> &rlist)
		{
			if ( rlist.empty() ) return;

			T1 *s = rlist._Head.m_next;
			T1 *e = rlist._Tail.m_prev;
			T1 *p = nullptr;

			p = _Tail.m_prev;
			_Tail.m_prev = e;
			e->m_next = (T1*)&_Tail;
			p->m_next = s;
			s->m_prev = p;
			_count += rlist._count;
			rlist.clear();
		}
		/** 리스트의 노드를 검색 */
		inline iterator find( iterator itS, iterator itE, T1 * Val)
		{
			while ( itS != end() && itS != itE )
			{
				if ( (*itS) == Val )
					return itS;
				itS++;
			}
			return end();
		}
		/** 리스트의 노드를 검색 */
		inline iterator find_if( iterator itS, iterator itE, fncompare &cmp_func )
		{
			while ( itS != end() && itS != itE )
			{
				if ( cmp_func == (*itS) )
					return itS;
				itS++;
			}
			return end();
		}
		/** 리스트의 노드를 검색하여 삭제 */
		inline iterator remove_if( iterator itS, iterator itE, fncompare &cmp_func )
		{
			while ( itS != end() && itS != itE )
			{
				if ( cmp_func == (*itS) )
				{
					return erase( itS );
				}
				itS++;
			}
			return end();
		}
		/** 리스트에 저장된 노드 개수 조회 */
		inline size_t size()
		{ return _count; }
	};
};
/**
	@def util::list 에 저장할 노드를 정의하는 매크로.
*/
#define listnode(classname)		util::list<classname>::node
/**
	@def util::list 에 저장된 노드를 검색하는 조건함수정의 매크로.
*/
#define listcompare(classname)	util::list<classname>::fncompare