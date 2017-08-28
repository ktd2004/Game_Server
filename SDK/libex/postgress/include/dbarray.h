// DBArray.h: interface for the CDBArray class.
//
#ifndef __DBARRAY_H
#define __DBARRAY_H

using namespace std;

#include <Windows.h>

#include <vector>
#include <string>

#define MAX_QUERY_COUNT 30

#include "pgdatabase.h"

typedef struct _DB_CONNECTION
{
	PGDatabase	connection ;
    BOOL		bConnect;
    int			iCount;
	bool		bUse;
	DWORD		dwUsedTime;
} DB_CONNECTION;

typedef vector<DB_CONNECTION*> DBArray;

class CDBArray
{
public:
    PGDatabase* GetDB(int &index);
    BOOL DBConnect(const char *strConnect, int nDBIndex = -1);
    void Init(int num);

    DBArray m_arDB;
    int m_iTotalConnection;
    int m_nLastIndex;
    string m_strConnect;

    CDBArray();
    virtual ~CDBArray();
    void ReleaseDB(int index, int nType, const char *pBuf);
	void Close();
private:
    void Clear(void);
};

#endif
