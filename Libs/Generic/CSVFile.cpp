// CSVFile.cpp: implementation of the CCSVFile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <windows.h>
#include "CSVFile.h"

CSVFile::CSVFile()
{
	m_nFieldCount = 0;
	m_nRowCount = 0;
	m_nBufSize = 0;

	m_pBuf = 0;
	m_ppText = 0;
}

CSVFile::~CSVFile()
{
	Free();
}

void CSVFile::Free( void )
{
	m_nFieldCount = 0;
	m_nRowCount = 0;
	m_nBufSize = 0;

	if( m_pBuf != 0 ) free( m_pBuf );
	if( m_ppText != 0 ) free( m_ppText );

	m_pBuf = 0;
	m_ppText = 0;
}

void CSVFile::GetFileName( std::string &strFileName )
{
	strFileName = m_strFileName;
}


bool CSVFile::Load( const char* szFileName )
{
	FILE *fp;
	long size;
	char* buf;

	fopen_s( &fp, szFileName, "rb");
	if( fp == 0 ) return false;

	// reading size of file.
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);

	if( size <= 0 ) return false;

	buf = (char*)malloc( size );

	fseek(fp, 0L, SEEK_SET);
	fread( buf, sizeof(char), size, fp );
	fclose( fp );

	m_nFieldCount	= GetFieldCount( buf, size );
	m_nRowCount		= GetRowCount( buf, size );

	if( m_nRowCount < 0 )
	{
		free(buf);
		return false;
	}

	m_strFileName = szFileName;
	m_pBuf = buf;
	m_nBufSize = size;

	LoadTextPtr();

	char szFieldName[100];

	for( int i = 0; i < m_nFieldCount; i++ )
	{
		GetText( 0, i, szFieldName, sizeof(szFieldName) );
		m_mapFieldNameIndex.insert( make_pair( szFieldName, i) );
	}

	return true;
}

int CSVFile::GetFieldCount( char* buf, int length )
{
	int i;
	int count;
	bool bInQuote = false;

	count = 0;

	for( i = 0; i < length; i++ )
	{
		if( buf[i] == 0x0A || i == (length-1) ) { count++; break; }
		if( buf[i] == '"' ) { if( bInQuote ) bInQuote = false; else bInQuote = true; }
		if( bInQuote ) continue;
		if( buf[i] == ',' ) { count++; continue; }
	}

	return count;
}

int CSVFile::GetRowCount( char* buf, int length )
{
	int i;
	int nFieldCount, nRowCount;
	bool bCheckField;

	nRowCount = 0;
	bCheckField = false;

	for( i = 0; i < length; i++ )
	{
		if( !bCheckField )
		{
			bCheckField = true;

			nFieldCount = GetFieldCount( buf+i, length-i );
			if( m_nFieldCount != nFieldCount ) return (nRowCount * -1);
		}

		if( buf[i] == 0x0A || i == (length-1) ) 
		{
			bCheckField = false;
			nRowCount++;
		}
	}

	return nRowCount;
}

int CSVFile::LoadTextPtr( void )
{
	int		i;
	int		nFieldIndex, nRowIndex;
	bool	bStartPtr;
	bool	bInQuote = false;

	m_ppText = (char**)malloc( m_nFieldCount * m_nRowCount * sizeof(char*) );

	nFieldIndex = 0;
	nRowIndex = 0;
	bStartPtr = true;

	for( i = 0; i < m_nBufSize; i++ )
	{
		if( bStartPtr ) 
		{
			m_ppText[ (nRowIndex * m_nFieldCount) + nFieldIndex ] = &m_pBuf[i];
			bStartPtr = false;
		}

		if( m_pBuf[i] == 0x0A ) 
		{ 
			nFieldIndex = 0;
			nRowIndex++;
			bStartPtr = true;
			continue;
		}

		if( m_pBuf[i] == '"' ) 
		{ 
			if( bInQuote ) bInQuote = false; 
			else bInQuote = true; 
		}

		if( !bInQuote && m_pBuf[i] == ',' ) 
		{ 
			nFieldIndex++;
			bStartPtr = true;
			continue;
		}
	}

	return 0;
}

bool CSVFile::GetText( int nRowIndex, int nFieldIndex, char* pText, int nMaxLength )
{
	char	*pBuf, *pEnd;
	int		count;
	bool	bInQuote = false;
	int		start, end;
	int		w;

	if( nRowIndex >= m_nRowCount ) return false;
	if( nFieldIndex >= m_nFieldCount ) return false;

	pBuf = m_ppText[ (nRowIndex * m_nFieldCount) + nFieldIndex ];
	pEnd = m_pBuf + m_nBufSize - 1;

	start = -1;
	end = -1;
	count = 0;
	w = 0;

	for( int i = 0; pBuf + i <= pEnd; i++ )
	{
		if( w == nMaxLength-1 )	break;
		if( (!bInQuote && pBuf[i] == ',') || pBuf[i] == 0x0D || pBuf[i] == 0x0A ) break;

		if( start == -1 )
		{
			if( pBuf[i] == ' ' ) { pText[w++] = pBuf[i]; continue; }
			if( pBuf[i] == '"' && bInQuote == false ) { bInQuote = true; continue; }
			start = i;
		}

		if( IsDBCSLeadByteEx( 949, pBuf[i] ) )
		{
			memcpy( pText+w, pBuf+i, 2 );
			w += 2;
			i += 1;
			continue;
		}

		if( pBuf[i] == '"' )
		{
			if( pBuf+i+1 == pEnd ) continue;
			if( pBuf[i+1] != '"' ) { bInQuote = false; continue; }
			pText[w++] = pBuf[i++];
			continue;
		}

		pText[w++] = pBuf[i];
	}

	pText[w] = 0;

	return true;
}

bool CSVFile::Add( const char* szFileName, vector<string> &vFields )
{
	int i;
	FILE *fp;
	std::string	str;

	fopen_s( &fp, szFileName, "a+");
	if( fp == 0 ) return false;


	for( i = 0; i < vFields.size(); i++ )
	{
		str += vFields[i].c_str();

		if( i != vFields.size()-1 ) str += ",";
		else str += "\n";
	}

	fwrite( str.c_str(), 1, str.size(), fp );
	fclose( fp );
	return true;
}

int	CSVFile::GetFieldIndex( const char* szFieldName )
{
	auto iter = m_mapFieldNameIndex.find(szFieldName);
	if( iter != m_mapFieldNameIndex.end() )
	{
		return iter->second;
	}

	return -1;
}

// template <>
// float CCSVFile::Get<float>( int nRowIndex, const char* szFieldName )
// {
// 	if( nRowIndex < 0 || nRowIndex >= m_nRowCount ) 
// 		return (float)0.0f;
// 
// 	int nFieldIndex = GetFieldIndex( szFieldName );
// 	if( nFieldIndex == -1 ) return (float)0.0f;
// 
// 	char szText[30];
// 	GetText( nRowIndex, nFieldIndex, szText, sizeof(szText) );
// 	float f = (float)atof(szText);	
// 	return (float)f;
// }

// template <>
// std::string CCSVFile::Get<std::string>(int nRowIndex, const char* szFieldName)
// {
// 	std::string s;
// 	if( nRowIndex < 0 || nRowIndex >= m_nRowCount ) 
// 		return s;
// 
// 	int nFieldIndex = GetFieldIndex( szFieldName );
// 	if( nFieldIndex == -1 ) return s;
// 
// 
// 	char szText[30];
// 	GetText( nRowIndex, nFieldIndex, szText, sizeof(szText) );
// 	s = szText;
// 	return s;
// }
