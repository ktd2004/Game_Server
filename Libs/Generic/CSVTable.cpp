#include "stdafx.h"

#include <CSVTable.h>
#include <algorithm>


CSVTable::CSVTable(void)
	: m_sTableFilePath(".\\CSVTable\\")
{
}

CSVTable::~CSVTable(void)
{
}

void CSVTable::SetPath(const std::string& sGametableFilePath)
{
	m_sTableFilePath = sGametableFilePath;
}

bool CSVTable::Load( void )
{
	bool bResult = OnLoad();
	return bResult;
}

bool CSVTable::Load(const std::string& sFileName, functional_v1<bool, CSVFile&>& loader)
{
	if ( !loader )
		return false;

	std::string	sPath;
	if ( m_sTableFilePath.size() != 0 &&
		 m_sTableFilePath[m_sTableFilePath.size()-1] != '\\' &&
		 m_sTableFilePath[m_sTableFilePath.size()-1] != '/' )
		sPath = m_sTableFilePath + "\\";
	sPath += sFileName;

	CSVFile table;
	if( table.Load( sPath.c_str() ) == true )
	{
		if( loader( table ) == false )
			return false;
	}
	else
	{
		return false;
	}

	return true;
}
