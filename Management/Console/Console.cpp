// Console.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Proactor.h>
#include <NetEventHandler.h>
#include <TaskScheduler.h>
#include <getopt.h>

void Usage(void)
{
	std::string bar(50, '-');
	printf("%s\n", bar.c_str());
	printf("usage)\n");
	printf("%s\n", bar.c_str());
	printf("console -a 127.0.0.1,4001 -c \"show\"\n"
		"console -a 127.0.0.1,4001 -c \"show user-PC\"\n"
		"console -a 127.0.0.1,4001 -c \"launch user-PC LogS\"\n"
		"console -a 127.0.0.1,4001 -c \"shutdown user-PC LogS\"\n"
		"console -a 127.0.0.1,4001 -c \"kill user-PC LogS\"\n"
		"console -a 127.0.0.1,4001 -c \"maxclient user-PC GtwS 1000\"\n"
		"console -a 127.0.0.1,4001 -c \"blockclient user-PC GtwS true\"\n"
		"console -a 127.0.0.1,4001 -c \"autorestart user-PC GtwS true\"\n"
		"console -a 127.0.0.1,4001 -c \"kickclient user-PC GtwS accountid\"\n"
		"console -a 127.0.0.1,4001 -c \"kickallclient user-PC GtwS\"\n"
		"console -a 127.0.0.1,4001 -b \"batch world1.launch\"\n"
		"console -a 127.0.0.1,4001 -b \"batch world1.shutdown\"\n");
	printf("%s\n", bar.c_str());
	printf("ex) batch file format\n");
	printf("%s\n", bar.c_str());
	printf("   \"launch user-PC iLog\"\n"
		"   \"launch user-PC iDbgw\"\n"
		"   sleep 5\n");
	printf("%s\n", bar.c_str());
}

class ClientHandler : public NetEventHandler
{
	std::string			m_Cmd;
	std::string			m_sAddr;
	uint16				m_iPort;
	NetLinkPtr			m_spLink;
	TaskScheduler*		m_pSch;
	std::list<std::string>	m_BatchCmd;
public:
	ClientHandler(TaskScheduler* pSch)
		: NetEventHandler(1)
		, m_pSch(pSch)
	{
		SetConnectedFunc(this, &ClientHandler::OnConnected);
		SetClosedFunc(this, &ClientHandler::OnClosed);

		EXPORT_RPC_NM_V0(this, &ClientHandler::OnReady);
		EXPORT_RPC_NM_V1(this, &ClientHandler::OnConsoleExecResult, const std::string&);
	}

	IMPORT_RPC_NM_V0(OnConsoleConnected, 1024);
	IMPORT_RPC_NM_V1(OnConsoleExecCommand, const std::string&, 1024);

	bool OnConnected(NetLinkPtr spLink)
	{
		rpc_OnConsoleConnected(spLink);
		return true;
	}

	bool OnClosed(NetLinkPtr spLink)
	{
		m_pSch->Finalize();
		return true;
	}

	bool OnReady(NetLinkPtr spLink)
	{
		printf("connected %s:%d\n", cstr(m_sAddr), m_iPort);
		while (m_BatchCmd.size() > 0)
		{
			m_Cmd = m_BatchCmd.front();
			m_BatchCmd.pop_front();

			size_t pos = m_Cmd.find("sleep");
			if (std::string::npos != pos)
			{
				stringvector arg;
				util::tokenizer(m_Cmd, arg, ' ');
				if (arg.size() > 1)
				{
					Sleep(stouint32(arg[1]));

				}
				continue;
			}

			printf("send command, %s\n", cstr(m_Cmd));
			return rpc_OnConsoleExecCommand(spLink, m_Cmd);
		}

		printf("send command, %s\n", cstr(m_Cmd));
		return rpc_OnConsoleExecCommand(spLink, m_Cmd);
	}

	bool LoadBatchCmd(const std::string& sFileName)
	{
		FILE *fp = nullptr;
		char szBuf[2048];

		fopen_s(&fp, cstr(sFileName), "r");
		if (fp == nullptr)
		{
			printf("file open failed\n");
			return false;
		}

		while (fgets(szBuf, sizeof(szBuf), fp))
		{
			m_BatchCmd.push_back(util::trim(szBuf));
		}

		fclose(fp);

		return true;
	}

	bool ParseCommand(int32 argc, _TCHAR* argv[])
	{
		int  opt;
		while (1)
		{
			int  option_index = 0;
			opt = getopt(argc, argv, "a:c:b:h");
			if (opt == -1)
				break;

			switch (opt)
			{
			// host
			case  'a':
				m_sAddr = util::trim(util::pairkey(optarg, ","));
				m_iPort = stouint32(util::pairval(optarg, ","));
				if (m_iPort == 0) m_iPort = 9004;
				break;
			// command
			case  'c':
				m_Cmd = util::trim(optarg);
				break;
			case  'h':
				Usage();
				return false;
			case 'b':
				if (LoadBatchCmd(optarg) == false)
				{
					return false;
				}
				break;
			case  '?':
				return false;
			default:
				return false;
			}
		}

		if (m_sAddr.size() == 0 || m_iPort == 0)
		{
			Usage();
			return false;
		}

		m_spLink = Connect(m_iPort, m_sAddr);
		if (m_spLink == nullptr)
			return false;

		CREATE_TIMER(m_iTaskSchedulerID, this, &ClientHandler::OnTimeout, 5000, nullptr);

		return true;
	}

	bool OnConsoleExecResult(NetLinkPtr spLink, const std::string& sResult)
	{
		printf("%s\n", cstr(sResult));
		while (m_BatchCmd.size() > 0)
		{
			m_Cmd = m_BatchCmd.front();
			m_BatchCmd.pop_front();

			size_t pos = m_Cmd.find("sleep");
			if (std::string::npos != pos)
			{
				stringvector arg;
				util::tokenizer(m_Cmd, arg, ' ');
				if (arg.size() > 1)
				{
					Sleep(stouint32(arg[1]));
					
				}
				continue;
			}

			printf("send command, %s\n", cstr(m_Cmd));
			return rpc_OnConsoleExecCommand(spLink, m_Cmd);
		}

		m_pSch->Finalize();
		return true;
	}

	bool OnTimeout(void *pInst)
	{
		printf("error> timeout, no response\n");
		m_pSch->Finalize();
		return true;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	TaskScheduler* pSch = CREATE_TASKSCEDULER(1, 10, 0);

	ClientHandler Hldr(pSch);
	if (!Hldr.Start(0, 1))
	{
		return 1;
	}

	if (!Hldr.ParseCommand(argc, argv))
	{
		Hldr.Stop();
		return 1;
	}

	pSch->RunEventLoop();
	Hldr.Stop();

	return 0;
}

