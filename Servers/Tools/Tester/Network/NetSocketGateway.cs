using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

using NetProtocol;

//============================================================================================
// NetSocketAuth
// - 게이트웨이서버 응답패킷 
//============================================================================================
// public class NetSocketGateway : NetSocket
// {
//     public NetSocketGateway()
//     {
//         m_SocketName = "GatewaySocket";
// 
// 		RegProtocol((int)eGatewayProtocol.eGTWS_CLIENT_LOGIN_ERROR, new Pk_LoginError());
// 		RegProtocol((int)eGatewayProtocol.eGTWS_CLIENT_LOGIN_SUCCESS, new Pk_LoginSuccess());
//     }
// }

//============================================================================================
// Pk_ClientLogin
// -
//============================================================================================
public class Pk_ClientLogin : Protocol
{
    public UInt64 m_iUID;
    public string m_sAccount;
    public string m_sVersion;

	public Pk_ClientLogin() : base(1024) { }

    public override bool Serialize()
    {
		m_SendBuf.SetProtocol((int)eGatewayProtocol.eCLIENT_GTWS_LOGIN);
        m_SendBuf.WriteUInt64(m_iUID);
        m_SendBuf.WriteString(m_sAccount);
        m_SendBuf.WriteString(m_sVersion);

        return true;
    }
}

//============================================================================================
// Pk_LoginError
// - 로그인 에러응답 
//============================================================================================
public class Pk_LoginError : Protocol
{
    public int m_iResult;
    public Pk_LoginError() : base(1024)
    {
    }

    public override bool Deserialize(Packet recv)
    {
        recv.ReadInt(out m_iResult);
        if (m_iResult != 0)
        {
            return false;
        }

        return true;
    }
}

//============================================================================================
// Pk_LoginSuccess
// - 로그인 성공 응답 
//============================================================================================
public class Pk_LoginSuccess : Protocol
{
	public UInt32 m_iCID;
    public UInt32 m_iAuthKey;

    public Pk_LoginSuccess() : base(1024)
    {
    }

    public override bool Deserialize(Packet recv)
    {
		recv.ReadUInt(out m_iCID);
        recv.ReadUInt(out m_iAuthKey);
        return true;
    }
}

public class Pk_Test : Protocol
{
	public string m_data = "this is test packet";

	public Pk_Test()
		: base(1024)
	{
	}

	public override bool Serialize()
	{
		m_SendBuf.SetProtocol((int)eChannelProtocol.eCLIENT_TEST_PACKET);
		m_SendBuf.WriteString(m_data);
		return true;
	}
}

public class Pk_Test_Ack : Protocol
{
	public string m_data;

	public Pk_Test_Ack()
		: base(1024)
	{
	}

	public override bool Deserialize(Packet recv)
	{
		recv.ReadString(out m_data);
		return true;
	}
}