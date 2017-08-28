#pragma once

#include <DataTypes.h>
#include <CSVFile.h>
#include <Functional.h>

#define	GET_FIELD_VALUE_INT(field_name,var)		var = table.Get<int>( i, field_name )
#define	GET_FIELD_VALUE_UINT(field_name,var)	var = table.Get<DWORD>( i, field_name )
#define	GET_FIELD_VALUE_WORD(field_name,var)	var = (WORD)table.Get<DWORD>( i, field_name )
#define	GET_FIELD_VALUE_BYTE(field_name,var)	var = (BYTE)table.Get<DWORD>( i, field_name )
#define	GET_FIELD_VALUE_FLOAT(field_name,var)	var = table.Get<float>( i, field_name )
#define GET_FIELD_VALUE_STR(field_name,var)		var = table.Get<std::string>( i, field_name )

#define	GET_ARRAY_FIELD_VALUE_UINT(field_name,index,var)\
		{\
			char szFieldName[1024]; \
			sprintf_s( (char*)szFieldName, sizeof(szFieldName), (const char*)field_name, index );\
			GET_FIELD_VALUE_UINT	(szFieldName,var);\
		}


#define	GET_ARRAY_FIELD_VALUE_WORD(field_name,index,var)\
		{\
			char szFieldName[1024]; \
			sprintf_s( (char*)szFieldName, sizeof(szFieldName), (const char*)field_name, index );\
			GET_FIELD_VALUE_WORD	(szFieldName,var);\
		}


#define CSV_TABLE_DEFINE(table_name) \
	std::map<DWORD,table_name*>	m_map_##table_name; \
	bool Load_##table_name(CSVFile& table); \
	inline std::map<DWORD,table_name*>& Get_##table_name(void) { return m_map_##table_name; } \
	inline const table_name* Get_##table_name( DWORD dwKey ) \
	{ \
		auto it = m_map_##table_name.find( dwKey ); \
		if ( it != m_map_##table_name.end() ) \
		{ \
			return it->second; \
		} \
		return nullptr; \
	}

#define CSV_LOAD_TABLE_DATA(class_name, table_name, file_name) \
{ \
	functional_v1<bool, CSVFile&> m_loader; \
	m_loader.assign(this, &##class_name::Load_##table_name ); \
	if ( Load(file_name, m_loader) == false ) \
	return false; \
}

#define CSV_LOAD_TABLE_FIELD(class_name, table_name, key_name) \
bool class_name::Load_##table_name(CSVFile& table) { \
	table_name* data = nullptr; \
	for( int i = 1; i < table.GetRowCount(); i++ ) \
	{ \
		DWORD dwKey = table.Get<DWORD>( i, key_name ); \
		auto it = m_map_##table_name.find(dwKey); \
		if ( it != m_map_##table_name.end() ) \
		{ \
			data = it->second; \
		} \
		else \
		{ \
			data = new table_name; \
			m_map_##table_name.insert( std::make_pair(dwKey,data) ); \
		}

#define CSV_LOAD_TABLE_FIELD_END(table_name)	} return true; }

class CSVTable
{
public:
	std::string			m_sTableFilePath;

public:
	CSVTable(void);
	CSVTable(const CSVTable& r) {}
	~CSVTable(void);

public:
	void	SetPath( const std::string& sGametableFilePath );
	bool	Load( void );

protected:
	bool	Load( const std::string& file_name, functional_v1<bool, CSVFile&>& loader );
	virtual bool OnLoad( void ) { return false; }

};