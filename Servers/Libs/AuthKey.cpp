#include "stdafx.h"
#include <TimeUtil.h>
#include <Md5Hash.h>
#include "Authkey.h"

namespace AuthKey
{

	uint32 Create( UID iUID )
	{
		enum{ MAX_BUFFER = 128 };
		
		time_t t = util::time2msec();
		char buf[MAX_BUFFER] = {0};

		sprintf_s( buf, "%I64u%u", iUID, t );
		return Md5Hash::ToHash( buf );
	}

}
