#pragma once

#include <DataTypes.h>
#include <Ptr.h>
#include <Functional.h>
#include <Logger.h>

class Command
{
public:
	typedef functional_v1<bool, stringvector&> CommandFunc;
	typedef std::map<std::string, CommandFunc*> CommandMap;
	CommandMap			m_CommandMap;

public:
	Command() {}
	virtual ~Command() {}

public:
	template<class O, class F>
	void COMMAND_FUNC( const std::string& sCmd, const O& o, F f )
	{
		CommandFunc* pFunc = new CommandFunc; pFunc->assign(o, f);
		m_CommandMap.insert( CommandMap::value_type(sCmd, pFunc) );
	}

	bool DoAction( const std::string& sCmd )
	{
		stringvector vec;
		util::tokenizer( sCmd, vec, ',' );
		if ( vec.size() > 0 )
		{
			CommandMap::iterator it = m_CommandMap.find( util::upper(vec[0]) );
			if ( it != m_CommandMap.end() )
			{
				CommandFunc& pFunc = *(it->second);
				if ( pFunc(vec) )
				{
					return true;
				}
			}
		}
		printf( "error: command invalid(%s)\n", cstr(sCmd) );
		return false;
	}
};