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
	��ƿ��Ƽ ������ ���� ���ӽ����̽�.
*/
namespace util
{
	/** 
		��带 �����Ҷ� ������ �޸� �Ҵ� ���� ���� ���� �� ������ �����ϴ� ����Ʈ ���ø� Ŭ����.

		util::list�� ������ ���� util::list<Ŭ����>::node �� ��� �޾ƾ� �Ѵ�. util::list�� ����� �����
		m_prev �� m_next �ɹ��� ����Ͽ� double linked list �� �����ϹǷ� �ΰ��� util::list �� ���ÿ� ������ �� ����.

		�Ʒ��� ��뿹�� MyObj Ŭ������ util::list�� �����ϱ� ���� ����� �����ش�.
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
			util::list �� ����� ��忡 �ݵ�� ����ؾ� �ϴ� Ŭ����.

			��ӹ��� Ŭ������ m_prev, m_next �ɹ��� ���Եȴ�.
		*/
		class node
		{
		public:
			T1*		m_prev;		///< �������
			T1*		m_next;		///< �������
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
		/** ��� �˻� ���� �Լ� */
		struct fncompare
		{
			virtual bool operator == (T1 *) = 0;
		};

	private:
		node		_Head;		///< ����� ����
		node		_Tail;		///< ����� ������
		size_t		_count;		///< ����� ��� ����

	public :
		list() { clear(); }
		list(const list<T1>& r) { clear(); }
		virtual ~list() {}

		/** 
			util::list �� �ݺ���.
		*/
		class iterator
		{
		private:
			node * m_node;		///< ���� �����ϰ� �ִ� ���

		public :
			iterator() : m_node(nullptr) {}
			iterator( T1 *elt ) : m_node( (node*)elt ) {}
			iterator( node *elt ) : m_node( elt ) {}
			~iterator() {}

			T1 * operator* () const { return (T1*)m_node; }
			T1 * operator-> () const { return (T1*)m_node; }

			/** ���Կ����� */
			iterator & operator= (const iterator& _Right)
			{
				m_node = _Right.m_node;
				return *this;
			}
			/** ���� ���� ������ */
			iterator & operator++ ()
			{
				m_node = m_node->m_next;
				return *this;
			}
			/** ���� ���� ������ */
			iterator operator++ (int)
			{
				iterator _tmp = *this;
				++*this;
				return _tmp;
			}
			/** ���� ���� ������ */
			iterator & operator-- ()
			{
				m_node = m_node->m_prev;
				return *this;
			}
			/** ���� ���� ������ */
			iterator operator-- (int)
			{
				iterator _tmp = *this;
				--*this;
				return _tmp;
			}
			/** �� ������ */
			bool operator== (const iterator& _Right)
			{ return m_node == _Right.m_node; }
			/** �� ������ */
			bool operator!= (const iterator& _Right)
			{ return (!(*this == _Right)); }
		};

		/** ����Ʈ�� ���� */
		inline iterator begin()
		{
			return ( iterator(_Head.m_next) );
		}
		/** ����Ʈ�� ������ */
		inline iterator end()
		{
			return ( iterator(&_Tail) );
		}
		/** ����Ʈ�� ��� �ִ��� ���� */
		inline bool empty()
		{
			return _Head.m_next == (T1*)&_Tail;
		}
		/** ����Ʈ�� ��� �����. */
		inline void clear()
		{
			_Head.m_prev = nullptr;
			_Head.m_next = (T1*)&_Tail;
			_Tail.m_prev = (T1*)&_Head;
			_Tail.m_next = nullptr;
			_count = 0;
		}
		/** ����Ʈ�� ��� �����. */
		inline T1 * front()
		{
			if ( empty() ) return nullptr;
			return _Head.m_next;
		}
		/** ����Ʈ�� �������� ���� */
		inline T1 * back()
		{
			if ( empty() ) return nullptr;
			return _Tail.m_prev;
		}
		/** ����Ʈ�� �տ� ���� */
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
		/** ����Ʈ�� �տ��� ���� */
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
		/** ����Ʈ�� �ڿ� ���� */
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
		/** ����Ʈ�� �ڿ��� ���� */
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
		/** �ݺ��ڰ� ����Ű�� ����� �ڷ� ���� */
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
		/** �ݺ��ڰ� ����Ű�� ��带 ���� */
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
		/** ��带 ���� */
		inline void remove(T1 * _Val)
		{
			if ( empty() || !_Val->m_prev || !_Val->m_next ) return;
			erase( iterator(_Val) );
		}
		/** ����Ʈ�� ��带 ��ü */
		inline void swap(util::list<T1> &rlist)
		{
			util::list<T1> target;
			target.merge( rlist );
			rlist.merge( *this );
			merge( target );
		}
		/** ����Ʈ�� ��带 ���� */
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
		/** ����Ʈ�� ��带 �˻� */
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
		/** ����Ʈ�� ��带 �˻� */
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
		/** ����Ʈ�� ��带 �˻��Ͽ� ���� */
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
		/** ����Ʈ�� ����� ��� ���� ��ȸ */
		inline size_t size()
		{ return _count; }
	};
};
/**
	@def util::list �� ������ ��带 �����ϴ� ��ũ��.
*/
#define listnode(classname)		util::list<classname>::node
/**
	@def util::list �� ����� ��带 �˻��ϴ� �����Լ����� ��ũ��.
*/
#define listcompare(classname)	util::list<classname>::fncompare