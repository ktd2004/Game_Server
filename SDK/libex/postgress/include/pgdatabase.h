
/**
@author koeikim
*/

#ifndef _PGDATABASE_H
#define _PGDATABASE_H

#include <Windows.h>
#include <string>
#include "libpq-fe.h"

class PGDatabase
{
public:
	enum eSQL_RESULT{
		eSQL_RESULT_SUCCESS = 0,
		eSQL_SUCCESS_WITH_INFO,
	};
public:
    PGDatabase();
    virtual ~PGDatabase();

    bool openDB( const char* const strConnect );
    void closeDB();
    bool isOpen(){ return m_pqconn != NULL ; }
	bool query( const char* const strQuery ) ;
	inline void ClearResult() {
		m_nRowIndex = -1 ;
		m_nRowNum = 0 ;
		m_nColNum = 0 ;
		PQclear( m_pqresult );
	}

	inline int GetRowNums() { return m_nRowNum ; }
	inline void MoveNext() { ++m_nRowIndex; }
	inline bool fetch() { ++m_nRowIndex; if(m_nRowIndex < m_nRowNum) return true; else return false; }
	
	// Get Field Interface
	bool GetData( int nFieldNum, unsigned char* result ) ;
	bool GetData( int nFieldNum, char* result ) ;
	bool GetData( int nFieldNum, short* result ) ;
	bool GetData( int nFieldNum, unsigned short* result ) ;
	bool GetData( int nFieldNum, int* result ) ;
	bool GetData( int nFieldNum, DWORD* result ) ;
// 	bool GetData( int nFieldNum, long* result ) ;
// 	bool GetData( int nFieldNum, unsigned long* result ) ;
	bool GetData( int nFieldNum, __int64* result ) ;
 	bool GetData( int nFieldNum, unsigned long long* result ) ;
	bool GetData( int nFieldNum, char* const result, int nSize ) ;
	bool GetData( int nFieldNum, float* result );
	
	// ByteA Conversion & get
	bool GetDataBytea ( int nFieldNum, unsigned char* const result, int nSize ) ;
	static unsigned char* SetDataBytea ( const unsigned char* bintext, size_t binlen, unsigned char* rettext, size_t& retlen );
	static bool EscapeBytea ( const unsigned char* bintext, size_t binlen, std::string & strOut );
	
	void DisplayErrorMsg();
	bool m_bOpen;

protected:
	PGconn* m_pqconn ;
	PGresult* m_pqresult ;

	int m_nRowIndex ;	// Current Fetch Row Num
	int m_nRowNum ;		// Fetched Row size
	int m_nColNum ;		// Column Size

	std::string m_strConnect;
};

#endif
