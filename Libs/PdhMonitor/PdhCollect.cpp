#include "stdafx.h"

#include <MacroFunc.h>
#include <StringUtil.h>
#include <PdhCollect.h>


namespace
{
	class PdhCollect : public IPdh
	{
	private:
		struct ThreadInfo
		{
			int32		idx;
			std::string name;
			HCOUNTER	hcIDThread;
			HCOUNTER	hcCSperSec;
			HCOUNTER	hcProcTime;
			HCOUNTER	hcUserTime;
		};

		typedef std::vector<ThreadInfo> ThreadInfoVec;
		struct ProcessInfo
		{
			int32 idx;
			std::string name;
			HCOUNTER hcIDProcess;
			HCOUNTER hcThreadCount;
			HCOUNTER hcProcTime;
			HCOUNTER hcUserTime;
			HCOUNTER hcWorkingSet;
			HCOUNTER hcVirtualBytes;
			HCOUNTER hcPrivateBytes;
			HCOUNTER hcPageFileBytes;
			HCOUNTER hcCPUUsage;
			ThreadInfoVec thinfos;
		};
		typedef std::vector<ProcessInfo> ProcessInfoVec;
		typedef std::map<std::string, HCOUNTER> CounterHandleMap;

		struct ProcessorInfo
		{
			HCOUNTER			hcProcTime;
			ProcessorCounter	data;
		};
		typedef std::vector<ProcessorInfo>	ProcessorVec;

		ProcessorVec		m_Processor;
		std::string			m_ProcessName;
		DWORD				m_ProcessID;
		HQUERY				m_hQuery;
		CounterHandleMap	m_Counters;
		ProcessInfoVec		m_ProcessInfo;

	public:
		PdhCollect() : m_hQuery(0)
		{

		}
		~PdhCollect()
		{
			Close();
		}

		static std::string fmtstr(const char* fmt, ...)
		{
			char buf[1024];
			va_list arg;
			va_start(arg, fmt);
			vsprintf_s(buf, sizeof(buf), fmt, arg);
			va_end(arg);
			return buf;
		}

		bool RegCounter(const_string& path, HCOUNTER* phcounter)
		{

			HCOUNTER hc;
			CounterHandleMap::iterator it = m_Counters.find(path);
			if (it != m_Counters.end())
			{
				hc = it->second;
			}
			else
			{
				PDH_STATUS status = PdhAddCounter(m_hQuery, cstr(path), 0, &hc);
				if (status != ERROR_SUCCESS)
				{
					printf( "error....\n" );
					return false;
				}
				m_Counters[path] = hc;
			}
			if (phcounter) *phcounter = hc;
			return true;
		}

		void UnregCounters()
		{
			for (CounterHandleMap::iterator it = m_Counters.begin(); it != m_Counters.end(); ++it)
			{
				const_string& path = it->first;
				HCOUNTER hcounter = it->second;
				PDH_STATUS status = PdhRemoveCounter(hcounter);
				if (status != ERROR_SUCCESS)
				{

				}
			}
		}

		bool QueryCounter()
		{

			PDH_STATUS status = PdhCollectQueryData(m_hQuery);
			if (status != ERROR_SUCCESS)
			{
				return false;
			}
			return true;
		}

		bool GetCounter(HCOUNTER hc, LONG &v)
		{
			DWORD format = PDH_FMT_LONG;
			PDH_FMT_COUNTERVALUE buffer;
			PDH_STATUS status = PdhGetFormattedCounterValue(hc, format, (DWORD) NULL, &buffer);
			if (status != ERROR_SUCCESS)
			{
				v = 0;
				return false;
			}
			v = buffer.longValue;
			return true;
		}

		bool GetCounter(HCOUNTER hc, double& v)
		{
			DWORD format = PDH_FMT_DOUBLE;
			PDH_FMT_COUNTERVALUE buffer;
			PDH_STATUS status = PdhGetFormattedCounterValue(hc, format, (DWORD) NULL, &buffer);
			if (status != ERROR_SUCCESS)
			{
				v = 0;
				return false;
			}
			v = buffer.doubleValue;
			return true;
		}

		bool GetCounter(const_string& path, double& v)
		{

			CounterHandleMap::iterator it = m_Counters.find(path);
			if (it == m_Counters.end())
			{
				v = 0;
				return false;
			}
			HCOUNTER hc = it->second;
			return GetCounter(hc, v);
		}

		bool RegProcessCounters()
		{
			int32 idx = (int32)m_ProcessInfo.size();
			ProcessInfo psi;
			psi.idx = idx;
			std::string psname = fmtstr("%s%s", cstr(m_ProcessName), (idx == 0 ? "" : cstr(fmtstr("#%d", idx)) ));
			psi.name = psname;
			if (!RegCounter(fmtstr("\\Process(%s)\\ID Process", cstr(psname)), &psi.hcIDProcess))
				return false;
			if (!RegCounter(fmtstr("\\Process(%s)\\Thread Count", cstr(psname)), &psi.hcThreadCount))
				return false;
			if (!RegCounter(fmtstr("\\Process(%s)\\%% Processor Time", cstr(psname)), &psi.hcProcTime))
				return false;
// 			if (!RegCounter(fmtstr("\\Process(%s)\\%% User Time", cstr(psname)), &psi.hcUserTime))
// 				return false;
// 			if (!RegCounter(fmtstr("\\Process(%s)\\Working Set", cstr(psname)), &psi.hcWorkingSet))
// 				return false;
			if (!RegCounter(fmtstr("\\Process(%s)\\Virtual Bytes", cstr(psname)), &psi.hcVirtualBytes))
				return false;
			if (!RegCounter(fmtstr("\\Process(%s)\\Private Bytes", cstr(psname)), &psi.hcPrivateBytes))
				return false;
// 			if (!RegCounter(fmtstr("\\Process(%s)\\Page File Bytes", cstr(psname)), &psi.hcPageFileBytes))
// 				return false;
			if (!RegCounter(fmtstr("\\Processor(_Total)\\%% Processor Time"), &psi.hcCPUUsage))
				return false;
// 			if (!RegCounter(fmtstr("\\Memory\\Available Bytes"), &psi.hcCPUUsage))
// 				return false;
			m_ProcessInfo.push_back(psi);

			SYSTEM_INFO si;
			GetSystemInfo(&si);
			for ( uint32 i = 0; i < si.dwNumberOfProcessors; i++ )
			{
				m_Processor.push_back( ProcessorInfo() );
				m_Processor[i].data.ID = i+1;
				std::string counter_name = fmtstr("\\Processor(%d)\\%% Processor Time", i);
				if (!RegCounter(counter_name, &m_Processor[i].hcProcTime))
					return false;
			}
			return true;
		}

		bool RegThreadCounters(ProcessInfo& psi)
		{

			int32 idx = (int32)psi.thinfos.size();
			ThreadInfo thi;
			thi.idx = idx;
			std::string thname = fmtstr("%s/%d%s", cstr(m_ProcessName), idx, (psi.idx == 0 ? "" : cstr(fmtstr("#%d", psi.idx))));
			thi.name = thname;
			if (!RegCounter(fmtstr("\\Thread(%s)\\ID Thread", cstr(thname)), &thi.hcIDThread))
				return false;
			if (!RegCounter(fmtstr("\\Thread(%s)\\Context Switches/sec", cstr(thname)), &thi.hcCSperSec))
				return false;
// 			if (!RegCounter(fmtstr("\\Thread(%s)\\%% Processor Time", cstr(thname)), &thi.hcProcTime))
// 				return false;
// 			if (!RegCounter(fmtstr("\\Thread(%s)\\%% User Time", cstr(thname)), &thi.hcUserTime))
// 				return false;
			psi.thinfos.push_back(thi);
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////

		bool Open(const_string& psname, DWORD psid) override
		{
			m_ProcessName = psname;
			m_ProcessID = psid;

			if ( psid == 0 )
			{
				m_ProcessID = ::GetCurrentProcessId();
			}

			if ( psname.empty() )
			{
				char szProcName[MAX_PATH];
				GetModuleFileName( NULL, szProcName, sizeof(szProcName) );
				m_ProcessName = util::getprocname( szProcName );
			}

			PDH_STATUS status = PdhOpenQuery(NULL, 0, &m_hQuery);
			if (status != ERROR_SUCCESS)
			{
				Close();
				return false;
			}
			if (!RegProcessCounters())
			{
				Close();
				return false;
			}
			return true;
		}

		bool Close() override
		{
			if (m_hQuery)
			{
				UnregCounters();
				PdhCloseQuery(m_hQuery);
				m_hQuery = 0;
			}
			return true;
		}

		bool CollectData( ModuleCounter* pModuleCounter, ThreadCounterVec* pThreadCounterVec, ProcessorCounterVec* pProcessorVec ) override
		{
			if (!m_hQuery) return false;
			for (;;)
			{
				if (!QueryCounter())
					return false;

// 				_tprintf (TEXT(
// 					"There is  %*ld percent of memory in use.\n"
// 					),
// 					7, statex.dwMemoryLoad);
// 				_tprintf (TEXT(
// 					"There are %*I64d total KB of physical memory.\n"
// 					),
// 					7, statex.ullTotalPhys/1024);
// 				_tprintf (TEXT(
// 					"There are %*I64d free  KB of physical memory.\n"
// 					),
// 					7, statex.ullAvailPhys/1024);
// 				_tprintf (TEXT(
// 					"There are %*I64d total KB of paging file.\n"
// 					),
// 					7, statex.ullTotalPageFile/1024);
// 				_tprintf (TEXT(
// 					"There are %*I64d free  KB of paging file.\n"
// 					),
// 					WIDTH, statex.ullAvailPageFile/1024);
// 				_tprintf (TEXT(
// 					"There are %*I64d total KB of virtual memory.\n"
// 					),
// 					7, statex.ullTotalVirtual/1024);
// 				_tprintf (TEXT(
// 					"There are %*I64d free  KB of virtual memory.\n"
// 					),
// 					7, statex.ullAvailVirtual/1024);
// 				_tprintf (TEXT(
// 					"There are %*I64d free  KB of extended memory.\n"
// 					),
// 					7, statex.ullAvailExtendedVirtual/1024);

				int32 pscnt = (int32)m_ProcessInfo.size();
				for (int32 psidx = 0; psidx < pscnt; ++psidx)
				{
					ProcessInfo& psi = m_ProcessInfo[psidx];
					LONG psid;
					
					if (!GetCounter(psi.hcIDProcess, psid))
						return false;

					if (m_ProcessID != psid)
						continue;

					if ( pModuleCounter )
					{
						GetModuleCounter( psi, pModuleCounter );
					}

					if ( pThreadCounterVec )
					{
						pThreadCounterVec->clear();
						GetThreadCounter( psi, pThreadCounterVec );
					}

					if ( pProcessorVec )
					{
						pProcessorVec->clear();
						GetProcessorCounter( pProcessorVec );
					}

					return true;
				}

				if (!RegProcessCounters())
					return false;
			}
			return true;
		}

		inline bool GetModuleCounter( ProcessInfo& psi, ModuleCounter* pModuleCounter )
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof (statex);
			GlobalMemoryStatusEx (&statex);

// 			LONG thcnt;
// 			if (!GetCounter(psi.hcThreadCount, thcnt))
// 				return false;

			pModuleCounter->UsageOfMemory = (double)statex.dwMemoryLoad;
			pModuleCounter->ID = m_ProcessID;
//			pModuleCounter->ThreadCount = thcnt;
			GetCounter(psi.hcProcTime, pModuleCounter->ProcessTime);
//			GetCounter(psi.hcUserTime, pModuleCounter->usertime);
//			GetCounter(psi.hcWorkingSet, pModuleCounter->workingset);
			GetCounter(psi.hcVirtualBytes, pModuleCounter->VirtualBytes);
			GetCounter(psi.hcPrivateBytes, pModuleCounter->PrivateBytes);
//			GetCounter(psi.hcPageFileBytes, pModuleCounter->pagefilebytes);
			GetCounter(psi.hcCPUUsage, pModuleCounter->UsageOfProcessor);

			return true;
		}

		bool CollectData( ModuleCounter* pModuleCounter ) override
		{
			if (!m_hQuery) return false;
			for (;;)
			{
				if (!QueryCounter())
					return false;

				int32 pscnt = (int32)m_ProcessInfo.size();
				for (int32 psidx = 0; psidx < pscnt; ++psidx)
				{
					ProcessInfo& psi = m_ProcessInfo[psidx];
					LONG psid;

					if (!GetCounter(psi.hcIDProcess, psid))
						return false;

					if (m_ProcessID != psid)
						continue;

					if ( pModuleCounter )
					{
						return GetModuleCounter( psi, pModuleCounter );
					}

					return true;
				}

				if (!RegProcessCounters())
					return false;
			}
			return true;
		}

		inline bool GetThreadCounter( ProcessInfo& psi, ThreadCounterVec* pThreadCounterVec )
		{
retry :
			LONG thcnt;
			if (!GetCounter(psi.hcThreadCount, thcnt))
				return false;

			if (psi.thinfos.size() < (size_t)thcnt)
			{
				for (int32 thidx = (int32)psi.thinfos.size(); thidx < thcnt; ++thidx)
				{
					if (!RegThreadCounters(psi))
						return false;
				}

				if (!QueryCounter())
					return false;

				goto retry;
			}
			else
			{
				for (int32 thidx = 0; thidx < thcnt; ++thidx)
				{
					ThreadInfo& thi = psi.thinfos[thidx];
					ThreadCounter td;
					GetCounter(thi.hcIDThread, td.id);
					GetCounter(thi.hcCSperSec, td.ContextSwitch);
//					GetCounter(thi.hcProcTime, td.proctime);
//					GetCounter(thi.hcUserTime, td.usertime);
					pThreadCounterVec->push_back(td);
				}
			}

			return true;
		}

		bool CollectData( ThreadCounterVec* pThreadCounterVec ) override
		{
			if (!m_hQuery) return false;
			for (;;)
			{
				if (!QueryCounter())
					return false;

				int32 pscnt = (int32)m_ProcessInfo.size();
				for (int32 psidx = 0; psidx < pscnt; ++psidx)
				{
					ProcessInfo& psi = m_ProcessInfo[psidx];
					LONG psid;

					if (!GetCounter(psi.hcIDProcess, psid))
						return false;

					if (m_ProcessID != psid)
						continue;

					if ( pThreadCounterVec )
					{
						pThreadCounterVec->clear();
						return GetThreadCounter( psi, pThreadCounterVec );
					}

					return true;
				}

				if (!RegProcessCounters())
					return false;
			}
			return true;
		}

		inline bool GetProcessorCounter( ProcessorCounterVec* pProcessorVec )
		{
			for ( size_t cpu = 0; cpu < m_Processor.size(); cpu++ )
			{
				ProcessorInfo& info = m_Processor[cpu];
				GetCounter(info.hcProcTime, info.data.Usage);
				pProcessorVec->push_back( info.data );
			}
			return true;
		}

		bool CollectData( ProcessorCounterVec* pProcessorVec ) override
		{
			if (!m_hQuery) return false;

			if (!QueryCounter())
				return false;

			if ( pProcessorVec )
			{
				pProcessorVec->clear();
				return GetProcessorCounter( pProcessorVec );
			}

			return true;
		}
	};
};

namespace Pdh
{
	IPdh* GetPdhMonitor()
	{
		static PdhCollect s;
		return &s;
	}

	void PrintModuleCounter( IPdh::ModuleCounter &x )
	{
		printf( "module counters\n" );
		printf( "process id = %lu\n",  x.ID );
		printf( "threads = %lu\n", x.ThreadCount );
		printf( "process time = %lf\n", x.ProcessTime );
		printf( "virutalbytes = %lf\n", x.VirtualBytes );
		printf( "privatebytes = %lf\n", x.PrivateBytes );
		printf( "usage of processor = %f\n", x.UsageOfProcessor );
		printf( "usage of memory = %f\n", x.UsageOfMemory );
	}

	void PrintProcessorCounter( IPdh::ProcessorCounterVec &x )
	{
		printf( "processor counters\n" );
		for ( size_t i = 0; i < x.size(); i++ )
		{
			printf( "cpu = %d, usage = %f\n", x[i].ID, x[i].Usage );
		}
	}

	void PrintThreadCounter( IPdh::ThreadCounterVec& x )
	{
		printf( "thread counters\n" );
		for ( size_t i = 0; i < x.size(); i++ )
		{
			printf( "thread = %d, context switching = %d\n", x[i].id, x[i].ContextSwitch );
		}
	}

}
