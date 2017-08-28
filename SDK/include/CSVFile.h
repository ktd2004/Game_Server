#pragma once

//////////////////////////////////////////////////////////////////////
// CSVFile.h: interface for the CCSVFile class.
// Write by zinee
//////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <map>

using namespace std;

class CSVFile  
{
private:

	std::string					m_strFileName;
	std::map<std::string,int>	m_mapFieldNameIndex;

	int		m_nFieldCount;
	int		m_nRowCount;

	char*	m_pBuf;
	int		m_nBufSize;

	char**	m_ppText;

public:
	CSVFile();
	virtual ~CSVFile();

	void	GetFileName( std::string &strFileName );
	bool	Load( const char* szFileName );
	void	Free( void );

	bool	Add( const char* szFileName, vector<string> &vFields );
	int		GetFieldCount( void ) { return m_nFieldCount; }
	int		GetRowCount( void ) { return m_nRowCount; }
	bool	GetText( int nRowIndex, int nFieldIndex, char* pText, int nMaxLength );
	int		GetFieldIndex( const char* szFieldName );

	template <typename T>
	T		Get( int nRowIndex, const char* szFieldName );

private:
	void	Init( void );
	int		GetFieldCount( char* buf, int length );
	int		GetRowCount( char* buf, int length );
	int		LoadTextPtr( void );
};

template <typename T>
T CSVFile::Get( int nRowIndex, const char* szFieldName )
{
	if( nRowIndex < 0 || nRowIndex >= m_nRowCount ) 
		return (T)0;

	int nFieldIndex = GetFieldIndex( szFieldName );
	if( nFieldIndex == -1 ) return (T)0;

	char szText[30];
	GetText( nRowIndex, nFieldIndex, szText, sizeof(szText) );
	__int64 n = _atoi64(szText);	
	return (T)n;
}

template <>
inline float CSVFile::Get<float>( int nRowIndex, const char* szFieldName )
{
	if( nRowIndex < 0 || nRowIndex >= m_nRowCount ) 
		return (float)0.0f;

	int nFieldIndex = GetFieldIndex( szFieldName );
	if( nFieldIndex == -1 ) return (float)0.0f;

	char szText[30];
	GetText( nRowIndex, nFieldIndex, szText, sizeof(szText) );
	float f = (float)atof(szText);	
	return (float)f;
}

template <>
inline std::string CSVFile::Get<std::string>(int nRowIndex, const char* szFieldName)
{
	std::string s;
	if( nRowIndex < 0 || nRowIndex >= m_nRowCount ) 
		return s;

	int nFieldIndex = GetFieldIndex( szFieldName );
	if( nFieldIndex == -1 ) return s;


	char szText[1024];
	GetText( nRowIndex, nFieldIndex, szText, sizeof(szText) );
	s = szText;
	return s;
}