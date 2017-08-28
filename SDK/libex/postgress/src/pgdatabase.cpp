#include "pgdatabase.h"

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

PGDatabase::PGDatabase()
        : m_pqconn(NULL), m_pqresult(NULL)
{
	m_bOpen = false;
}

PGDatabase::~PGDatabase()
{
    if( isOpen() )
        closeDB();
}

bool PGDatabase::openDB( const char* const strConnect )
{
	m_strConnect = strConnect;
    if( isOpen() )
        return false;
	if (PQstatus(m_pqconn) == CONNECTION_OK)               /* did the connection fail? */
	{
		PQreset(m_pqconn);
	}
	else
	{
		m_pqconn = PQconnectdb(strConnect);
	}
	
	if (PQstatus(m_pqconn) == CONNECTION_BAD)               /* did the connection fail? */
	{
		return false;
	}

#if defined(_CHINA_MODE_)
	PQsetClientEncoding(m_pqconn, "GB18030");
#elif defined(_JAPAN_MODE_)
	PQsetClientEncoding(m_pqconn, "SJIS");
#elif defined(_KOREA_MODE_)
	PQsetClientEncoding(m_pqconn, "UHC");
#else
	PQsetClientEncoding(m_pqconn, "UTF-8");
#endif

	m_bOpen = true;
    
    return true;
}

void PGDatabase::closeDB()
{
	if ( m_pqconn == NULL )
		return ;

	PQfinish( m_pqconn ) ;
	m_pqconn = NULL ;
	m_bOpen = false;
}

bool PGDatabase::query( const char* const strQuery  )
{
	if( !m_bOpen ) return false;

	m_pqresult = PQexec(m_pqconn, strQuery);
	if ( m_pqresult == NULL ) 
	{
		return false;
	}
	else if ( PQresultStatus( m_pqresult ) != PGRES_TUPLES_OK && PQresultStatus( m_pqresult ) != PGRES_COMMAND_OK)
	{
		///@note 에러 중 DB와 연결이 끊어진 경우 다시 재접속 하도록 하는 루틴..
		if(strstr(PQresultErrorMessage(m_pqresult), "server closed the connection unexpectedly"))
		{
			PQfinish( m_pqconn ) ;
			m_pqconn = PQconnectdb(m_strConnect.c_str());
#if defined(_CHINA_MODE_)
			PQsetClientEncoding(m_pqconn, "GB18030");
#elif defined(_JAPAN_MODE_)
			PQsetClientEncoding(m_pqconn, "SJIS");
#elif defined(_KOREA_MODE_)
			PQsetClientEncoding(m_pqconn, "UHC");
#else
			PQsetClientEncoding(m_pqconn, "UTF-8");
#endif
		}
		
		return false;
	}
	
	m_nRowIndex = 0;
	m_nRowNum = PQntuples( m_pqresult ) ;
	m_nColNum = PQnfields( m_pqresult ) ;
	
	return true;
}

bool PGDatabase::GetData( int nFieldNum, char* result )
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return false;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return true;
  *result = (char)(atoi(PQgetvalue(m_pqresult, m_nRowIndex, nFieldNum-1)));

  return 1;	
}

bool PGDatabase::GetData( int nFieldNum, unsigned char* result )
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return 0;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return true;
  *result = (unsigned char)(atoi(PQgetvalue(m_pqresult, m_nRowIndex, nFieldNum-1)));

  return true;	
}

bool PGDatabase::GetData( int nFieldNum, short* result ) 
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return 0;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1) ) return true;
  *result = (short)(atoi(PQgetvalue(m_pqresult, m_nRowIndex, nFieldNum-1)));

  return true;	
}

bool PGDatabase::GetData( int nFieldNum, unsigned short* result ) 
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return false;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1) ) return true;
  *result = (unsigned short)(atoi(PQgetvalue(m_pqresult, m_nRowIndex, nFieldNum-1)));

  return 1;	
}

bool PGDatabase::GetData( int nFieldNum, int* result )
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return false;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return true;
  *result = atoi(PQgetvalue(m_pqresult, m_nRowIndex, nFieldNum-1 ));

  return true;	
}

bool PGDatabase::GetData( int nFieldNum, DWORD* result )
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return false;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return true;
  *result = (unsigned int) atoi(PQgetvalue(m_pqresult, m_nRowIndex, nFieldNum-1 ));

  return true;	
}

// bool PGDatabase::GetData( int nFieldNum, long* result )
// {
//   if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return false;
// 
//   if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return true;
//   *result = atol(PQgetvalue(m_pqresult, m_nRowIndex, nFieldNum-1));
// 
//   return true;	
// }

// bool PGDatabase::GetData( int nFieldNum, unsigned long* result )
// {
//   if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return false;
// 
//   if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return true;
//   *result = (unsigned long)atol(PQgetvalue(m_pqresult, m_nRowIndex, nFieldNum-1));
// 
//   return true;	
// }

bool PGDatabase::GetData( int nFieldNum, __int64* result )
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return 0;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return 0;
  *result = _atoi64(PQgetvalue(m_pqresult,m_nRowIndex,nFieldNum-1));

  return 1;	
}

bool PGDatabase::GetData( int nFieldNum, unsigned long long* result )
{
	if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return 0;

	if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return 0;
	*result = static_cast<unsigned long long>(_atoi64(PQgetvalue(m_pqresult,m_nRowIndex,nFieldNum-1)));

	return 1;
}

bool PGDatabase::GetData( int nFieldNum, float* result )
{
	if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return 0;

	if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return 0;
	*result = (float)atof(PQgetvalue(m_pqresult,m_nRowIndex,nFieldNum-1));

	return 1;
}

bool PGDatabase::GetData( int nFieldNum, char* const result, int nSize ) // IN: limit OUT : actual size
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return false;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return true;

  int length = PQgetlength( m_pqresult, m_nRowIndex, nFieldNum-1 );

  if ( length > nSize )
  {
	length = nSize;
  }
  else {
	nSize = length ;
  }

  memcpy(result,(char*)PQgetvalue(m_pqresult,m_nRowIndex,nFieldNum-1) ,length);
  
  return true;	
}

bool PGDatabase::GetDataBytea ( int nFieldNum, unsigned char* const result, int nSize )
{
  if ( m_pqresult == NULL || nFieldNum > m_nColNum ) return false;

  if ( PQgetisnull( m_pqresult, m_nRowIndex, nFieldNum-1 ) ) return true;
	
  size_t length = 0 ;
  unsigned char* pData = PQunescapeBytea( (const unsigned char*) PQgetvalue(m_pqresult,m_nRowIndex,nFieldNum-1), &length ) ;

  if ( length > nSize )
  {
    length = nSize;
  }
  else {
	nSize = (int)length ;
  }

  memcpy(result, pData,length);

  PQfreemem( pData ) ;

  return true ;
}

unsigned char* PGDatabase::SetDataBytea ( const unsigned char* bintext, size_t binlen, unsigned char* rettext, size_t& retlen )
{
        const unsigned char *vp;
        unsigned char *rp;
        unsigned char *result;
        size_t          i;
        size_t          len;

        /*
         * empty string has 1 char ('\0')
         */
        len = 1;

        vp = bintext;
        for (i = binlen; i > 0; i--, vp++)
        {
                if (*vp < 0x20 || *vp > 0x7e)
                        len += 5;                       /* '5' is for '\\ooo' */
                else if (*vp == '\'')
                        len += 5;
                else if (*vp == '\\')
                        len += 5;
                else
                        len++;
        }
        
        if( len > retlen )
        {
	        retlen = 0;
          	return NULL;
        }

        retlen = len;
        rp = result = rettext;
        if (rp == NULL)
        {
            return NULL;
        }

        vp = bintext;
        retlen = len;

		int inc;
		int remain = (int)retlen;

        for (i = binlen; i > 0; i--, vp++)
        {
                if (*vp < 0x20 || *vp > 0x7e)
                {
					inc = sprintf_s((char*)rp, remain, "\\\\%03o", *vp);
                }
                else if (*vp == '\'')
                {
					inc = sprintf_s((char*)rp, remain, "\\\\%03o", *vp);
//                  rp[0] = '\\';
//                  rp[1] = '\'';
//                  rp += 2;
                }
                else if (*vp == '\\')
                {
					inc = sprintf_s((char*)rp, remain, "\\\\%03o", *vp);
//                  rp[0] = '\\';
//                  rp[1] = '\\';
//                  rp[2] = '\\';
//                  rp[3] = '\\';
//                  rp += 4;
                }
                else
				{
					*rp = *vp;
					inc = 1;
				}

				rp		+= inc;
				remain	-= inc;
        }

        *rp = '\0';

        return result;
}

void PGDatabase::DisplayErrorMsg()
{
}

bool PGDatabase::EscapeBytea ( const unsigned char* buffer_ptr, size_t buffer_len, std::string & strOut )
{
	strOut.clear();
	strOut.reserve( (buffer_len * 2) + 4 );
	strOut = "\\\\x";

	char temp[3];

	for( int i= 0; i < buffer_len; i++ )
	{
		sprintf_s( temp, 3, "%02x", buffer_ptr[i] );
		strOut += temp;
	}

	return true;
}
