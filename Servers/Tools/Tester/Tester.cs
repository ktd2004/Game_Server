using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Util;
using NetProtocol;
using NetEnum;

public class PacketTester
{
	public TesterWorker m_tester = null;
	public NetSocket m_NetGateway = null;
    
    public string m_sWebUrl = string.Empty;
	public string m_sAccount = string.Empty;
	public string m_sVersion = "0.0.0";
	public UInt64 m_iUID = 0;
	public UInt64 m_iAuthKey = 0;

    private double m_fTickCnt;
	private string m_sGatewayHost = "localhost";
	private ushort m_iGatewayPort = 9001;

	public PacketTester(TesterWorker tester, string sWebUrl, string sAccount, string sVersion, double fTickCnt)
    {
		m_tester = tester;
        m_sWebUrl = sWebUrl;
        m_sAccount = sAccount;
        m_sVersion = sVersion;
        m_fTickCnt = fTickCnt;

        m_NetGateway = new NetSocket();
		m_NetGateway.SetEventConnected(OnConnected);
		m_NetGateway.SetEventError(OnError);
		m_NetGateway.RegProtocol((int)eGatewayProtocol.eGTWS_CLIENT_LOGIN_ERROR, new Pk_LoginError(), OnLoginError);
		m_NetGateway.RegProtocol((int)eGatewayProtocol.eGTWS_CLIENT_LOGIN_SUCCESS, new Pk_LoginSuccess(), OnLoginSuccess);
		m_NetGateway.RegProtocol((int)eChannelProtocol.eCLIENT_TEST_PACKET_ACK, new Pk_Test_Ack(), OnTestAck);
    }
	
	public void OnError(int iError, string sErrMsg )
	{
		m_tester.AddLog(sErrMsg);
	}

	public void OnConnected()
	{
		m_tester.AddLog("Connected");
		Pk_ClientLogin pk = new Pk_ClientLogin();
		pk.m_iUID = m_iUID;
		pk.m_sAccount = m_sAccount;
		pk.m_sVersion = m_sVersion;
		m_NetGateway.Send(pk);
	}

	public void OnLoginError(Protocol p)
	{
		m_tester.AddLog("Received: " + p.GetType());
	}

	public void OnLoginSuccess(Protocol p)
	{
		m_tester.AddLog("Received: " + p.GetType());
		Pk_Test pk = new Pk_Test();
		m_NetGateway.Send(pk);
	}

	public void OnTestAck(Protocol p)
	{
		Pk_Test_Ack c = (Pk_Test_Ack)p;
		m_tester.AddLog("Received: " + c.m_data);
	}

    public UInt64 GetUID()
    {
        return m_iUID;
    }

	public void SetUID(UInt64 iUID)
	{
		m_iUID = iUID;
	}

    public string GetVersion()
    {
        return m_sVersion;
    }

    private bool ConnectWeb()
    {
        StringBuilder sPostData = new StringBuilder();
        sPostData.Append("account=" + m_sAccount);
        sPostData.Append("&market=0");
        sPostData.Append("&pushid=tester_push_" + m_sAccount);
        sPostData.Append("&token=tester_token_" + m_sAccount);
        sPostData.Append("&expire=0");
        sPostData.Append("&identify=99999");

        byte[] bytePostData = UTF8Encoding.UTF8.GetBytes(sPostData.ToString());

        try
        {
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(m_sWebUrl);
            req.Method = "POST";
            req.ContentType = "application/x-www-form-urlencoded";
            req.ContentLength = bytePostData.Length;

            Stream stream = req.GetRequestStream();
            stream.Write(bytePostData, 0, bytePostData.Length);
            stream.Close();

            HttpWebResponse res = (HttpWebResponse)req.GetResponse();
            stream = res.GetResponseStream();
            StreamReader streamReader = new StreamReader(stream, Encoding.Default);
            string sResult = streamReader.ReadToEnd();
            string[] sTokens = sResult.Split(',');

            // result
            if (sTokens[0] != "Success")
            {
                return false;
            }

            // status error
            if (sTokens[2] != "0")
            {
                return false;
            }

            m_iUID = Convert.ToUInt64(sTokens[1]);
            m_sGatewayHost = sTokens[3];
            m_iGatewayPort = Convert.ToUInt16(sTokens[4]);
            
            return true;
        }
        catch (WebException e)
        {
			m_tester.AddLog(e.Message);
            return false;
        }
    }

    public bool Login( string sHost, ushort iPort)
    {
        if (!m_NetGateway.IsConnected())
        {
			m_sGatewayHost = sHost;
			m_iGatewayPort = iPort;

            // connect gateway
            if (!m_NetGateway.Connect(m_sGatewayHost, m_iGatewayPort))
            {
                return false;
            }
        }

        return true;
    }

	public void Logout()
	{
		if ( m_NetGateway.IsConnected() )
		{
			m_NetGateway.Disconnect();
		}
	}

	public bool IsConnected()
	{
		return m_NetGateway.IsConnected();
	}

	public bool SendTestPacket()
	{
		return true;
	}
}

public class TesterWorker
{
	private TextBox	m_Textbox = null;
    private volatile bool m_bStop = false;
    delegate void CtrlInvoke(string sLog, bool bCR=true);

    public void AddLog(string sLog, bool bCR=true)
    {
		if (m_Textbox.InvokeRequired)
        {
            CtrlInvoke ci = new CtrlInvoke(AddLog);
			m_Textbox.Invoke(ci, sLog, bCR);
        }
        else
        {
            if (bCR)
            {
                sLog += "\r\n";
				m_Textbox.AppendText(("[" + Time.GetCurrentTime() + "]") + sLog);
            }
            else
            {
				m_Textbox.AppendText(sLog);
            }
        }
    }

    public void WorkerMain(string sWebUrl, int iCreateNum, string sVersion, double fTickCnt, TextBox t)
    {
		m_Textbox = t;
        m_bStop = false;
        string sAccount = "Tester_" + iCreateNum;
        fTickCnt *= 1000.0f;
        int iCurrent = 0;
        int iLastTick = 0;

		AddLog(sAccount + " is starting");

		PacketTester tester = new PacketTester(this, sWebUrl, sAccount, sVersion, fTickCnt);
		tester.SetUID((UInt64)iCreateNum);
		tester.Login("localhost", 9001);

        while (!m_bStop)
        {
            iCurrent = Environment.TickCount;
            if (iCurrent - iLastTick >= fTickCnt)
            {
                iLastTick = iCurrent;
				if ( tester.IsConnected() )
				{
					if (!tester.SendTestPacket())
					{
						m_bStop = true;
						break;
					}
				}
            }
            else
            {
                Thread.Sleep(1);
            }
        }

		tester.Logout();
        AddLog(sAccount + " is stopped");
		tester = null;
    }

    public void Stop()
    {
        m_bStop = true;
    }

    public bool IsRunning()
    {
        return m_bStop ? false : true;
    }
}
