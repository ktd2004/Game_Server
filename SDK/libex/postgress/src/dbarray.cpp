// DBArray.cpp: implementation of the CDBArray class.
// Make : ks2000 (may be...)
// Edit : Jeon Jin Soo : 2004-06-24
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>

#include "pgdatabase.h"
#include "dbarray.h"

///#include "encodeconvertor.h"
///#include "dbrecord.h"

//#include "extern.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBArray::CDBArray()
{
    m_strConnect = "";
	m_nLastIndex = 0;
	m_iTotalConnection = 0;
}

CDBArray::~CDBArray()
{
    Clear();
}

/*!
    \fn CDBArray::Clear(void)
	Memory Clear & DB Close
 */
void CDBArray::Clear(void)
{
    int i;
    for( i = 0; i < (int)m_arDB.size(); i++)
    {
		if(m_arDB[i]->bConnect)
		{
			m_arDB[i]->connection.closeDB();
		}
        if( m_arDB[i] )
        {
            delete m_arDB[i];
        }
    }
    m_arDB.clear();
}

///////////////////////////////////////////////////////////////////////
//	DB Connection 정보를 초기화 한다.
//
void CDBArray::Init(int num)
{
	if(m_arDB.size() > 0) Clear();

    DB_CONNECTION* pNewDB = NULL;

    for( int i = 0; i < num; i++ )
    {
        pNewDB = new DB_CONNECTION;
        pNewDB->bConnect = FALSE;
        pNewDB->iCount = 0;
		pNewDB->bUse = false;

        m_arDB.push_back( pNewDB );
    }

    m_iTotalConnection = num;
    m_nLastIndex = 0;
}

///////////////////////////////////////////////////////////////////////
//	DB 에 연결한다.
//
BOOL CDBArray::DBConnect(const char *strConnect, int nDBIndex)
{
    char szConnect[256]; memset(szConnect, 0, sizeof(szConnect) );

    memset(szConnect, 0, sizeof(szConnect) );
    memcpy(szConnect, strConnect, strlen(strConnect)+1);

    DB_CONNECTION* pDB = NULL;
    m_strConnect = strConnect;

    if (nDBIndex == -1)
    {
        for( int i = 0; i < (int)m_arDB.size(); i++)
        {
            if( m_arDB[i] )
            {
                pDB = m_arDB[i];
                if( !pDB->bConnect )
                {
					if ( false == pDB->connection.openDB(szConnect) )               /* did the connection fail? */
					{
						pDB->bConnect = FALSE;
                        return FALSE;
					}

                    pDB->bConnect = TRUE;
					pDB->iCount = 0;
					pDB->bUse = false;
					pDB->dwUsedTime = 0;
                }
                else
                {
                    pDB->connection.closeDB() ;
					if ( false == pDB->connection.openDB(szConnect) )               /* did the connection fail? */
					{
						pDB->connection.DisplayErrorMsg();
						pDB->bConnect = FALSE;
                        return FALSE;
					}

                    pDB->bConnect = TRUE;
					pDB->iCount = 0;
					pDB->bUse = false;
					pDB->dwUsedTime = 0;
                }
            }
            else
                return FALSE;
        }
    }
    else
    {
        if( m_arDB[nDBIndex] )
        {
            pDB = m_arDB[nDBIndex];

            if( !pDB->bConnect )
            {
				if ( false ==  pDB->connection.openDB(szConnect) )               /* did the connection fail? */
				{
					pDB->bConnect = FALSE;
                    return FALSE;
				}
                pDB->bConnect = TRUE;
				pDB->iCount = 0;
				pDB->bUse = false;
            }
            else
            {
				pDB->connection.closeDB() ;
				if ( pDB->connection.openDB(szConnect) )               /* did the connection fail? */
				{
					pDB->bConnect = FALSE;
                    return FALSE;
				}
                pDB->bConnect = TRUE;
				pDB->iCount = 0;
				pDB->bUse = false;
            }
        }
        else
            return FALSE;
    }

    return TRUE;
}

///
///	현재 사용하지 않고 있는 디비에 연결한다.
///
PGDatabase* CDBArray::GetDB(int &index)
{
    int i;
    index = -1;
	PGDatabase* pDB = NULL;
/*
	index = m_nLastIndex;

	if( m_arDB[index] && m_arDB[index]->bConnect && m_arDB[index]->bUse == false)
	{
		m_arDB[index]->bUse = true;
		m_arDB[index]->dwUsedTime = GetTickCount();
		return &(m_arDB[index]->db);
	}
*/
    for(i = 0; i < (int)m_arDB.size(); i++ )	// 안쓰는 데이타 선택
    {
		// chapossi
		//if (m_iTotalConnection > MAX_THREAD_COUNT) m_iTotalConnection = MAX_THREAD_COUNT;

        if (m_nLastIndex >= m_iTotalConnection)  m_nLastIndex = 0;
        index = m_nLastIndex++;
		// chapossi
		if (index < 0 && index >= m_iTotalConnection) continue;

		if( m_arDB[index]->connection.m_bOpen == false )
		{
			if( m_arDB[index]->bConnect == TRUE )
				m_arDB[index]->connection.closeDB();
			
			m_arDB[index]->bConnect = FALSE;
		}

        if( m_arDB[index] && m_arDB[index]->bConnect && m_arDB[index]->bUse == false)
        {
			m_arDB[index]->bUse = true;
			m_arDB[index]->dwUsedTime = GetTickCount();
            return &(m_arDB[index]->connection);
        }
    }
	
	// 더이상 쓸 연결이 없으면 모든 설정을 재설정 한다.
	for(i = 0; i < (int)m_arDB.size(); i++ ) DBConnect(m_strConnect.data(), i);
    
	for(i = 0; i < (int)m_arDB.size(); i++ )	// 안쓰는 데이타 선택
    {
        if (m_nLastIndex >= m_iTotalConnection)  m_nLastIndex = 0;
        index = m_nLastIndex++;
		
        if( m_arDB[index] && m_arDB[index]->bConnect && m_arDB[index]->bUse == false)
        {
			m_arDB[index]->bUse = true;
			m_arDB[index]->dwUsedTime = GetTickCount();
            return &(m_arDB[index]->connection);
        }
    }
	
	printf("***>Warnning CDBArray::GetDB() Empty, ReConnect Try, Not Connection***");
    return NULL;
}

/*!
    \fn CDBArray::ReleaseDB(int index)
	현재 사용하고 있는 DB를 사용하지 않는 DB 로 셋팅한다.
	만약 현재의 connection 이 MAX_QUERY_COUNT 보다 크다면 
	속도를 위해 연결을 재설정 한다.
	/// @todo : Time Out 개념을 집어넣어야 한다.
 */
void CDBArray::ReleaseDB(int index, int nType, const char *pBuf)
{
    DB_CONNECTION* pDB = NULL;
	
	if( index < 0 || index >= m_arDB.size() ) return;
	
//	index = m_nLastIndex;

	DWORD dwUsed = GetTickCount();

	if( m_arDB[index] )
	{
		pDB = m_arDB[index];
		pDB->bUse = false;
		
		if( (dwUsed - pDB->dwUsedTime) > 5000)
		{
			printf("ReleaseDB::OverTime[%d], Type[%d], SQL[%s]", (dwUsed - pDB->dwUsedTime), nType, pBuf);
		}
//		if( m_iTotalConnection <= 1 ) return;

/*		if( (dwUsed - pDB->dwUsedTime) > 10000)
		{
			PRINT("ReleaseDB::Change Handle[%d]", index);
			m_nLastIndex += 1;
			m_nLastIndex = m_nLastIndex % m_iTotalConnection;

			pDB->db.closeDB();

			if( !pDB->db.openDB((SQLCHAR*)m_strConnect.c_str(),(int)m_strConnect.size()) )
			{
				pDB->bConnect = FALSE;
				PRINT(">***Err ReConnect Handle[%d]***", index);
				return ;
			}
			pDB->bConnect = TRUE;
			pDB->iCount = 0;
			pDB->bUse = false;
			pDB->dwUsedTime = 0;
		}*/
/*		
		if( pDB->iCount++ >= MAX_QUERY_COUNT && pDB->bConnect)
		{
			pDB->db.closeDB();
			if( !pDB->db.openDB((SQLCHAR*)m_strConnect.c_str(), (int)m_strConnect.size()) )
			{
				return;
			}
			pDB->bConnect = TRUE;
			pDB->iCount = 0;
		}
*/	}
}

void CDBArray::Close()
{
    DB_CONNECTION* pDB = NULL;
	
	for(int i = 0; i < (int)m_arDB.size(); i++ )
	{
		if( m_arDB[i] )
		{
			pDB = m_arDB[i];
			pDB->bUse = false;
			pDB->connection.closeDB();
		}
	}

	Clear();
}
