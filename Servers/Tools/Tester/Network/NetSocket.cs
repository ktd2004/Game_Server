using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

using NetProtocol;
using NetEnum;

//using Ionic.Zlib;

public class NetSocket
{
    public string m_SocketName = "NetSocket";

    protected Socket m_Socket = null;
    protected string m_sAddr = "localhost";
    protected ushort m_iPort = 9001;

    protected Hashtable m_PacketHash;
	protected Packet m_RecvBuffer = new Packet(4096);

    public NetSocket()
    {
        m_PacketHash = new Hashtable();
    }

    public bool RegProtocol(int iProtocol, Protocol p, Protocol.EventFunction cb = null)
    {
        p.m_iProtocol = iProtocol;
		p.SetEventFunction(cb);
        m_PacketHash[iProtocol] = p;
        return true;
    }

    public bool Connect(string sAddr, ushort iPort)
    {
        m_sAddr = sAddr;
        m_iPort = iPort;

        try
        {
            m_Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            m_Socket.BeginConnect(m_sAddr, m_iPort, new AsyncCallback(Callback_Connect), m_Socket);
            return true;
        }
        catch (SocketException)
        {
			OnError((int)eSocketError.eError_ConnectFailed, "Connect failed");
            return false;
        }
    }

	public delegate void ErrorFunction(int iError, string sErrMsg);
	private ErrorFunction m_EvtError = null;
	public void SetEventError(ErrorFunction o)
	{
		m_EvtError = o;
	}

	public void OnError( int iError, string sErrMsg )
	{
		if (m_EvtError != null)
			m_EvtError(iError, sErrMsg);
	}

	public delegate void EventFunction();
	
	private EventFunction m_EvtConnected = null;
	public void SetEventConnected(EventFunction o)
	{
		m_EvtConnected = o;
	}
	public void OnConnected()
	{
		if (m_EvtConnected != null)
			m_EvtConnected();
	}

	private EventFunction m_EvtClosed = null;
	public void SetEventClosed(EventFunction o)
	{
		m_EvtClosed = o;
	}

	public void OnClosed()
	{
		if (m_EvtClosed != null)
			m_EvtClosed();
	}

    private void Callback_Connect(IAsyncResult ar)
    {
        try
        {
            m_Socket.EndConnect(ar);
            m_Socket.BeginReceive(
				m_RecvBuffer.GetBytes(),				// buffer
                m_RecvBuffer.GetPacketSize(),			// offset
                m_RecvBuffer.GetCapacity(),				// receive bytes
                SocketFlags.None,						// socket option
				new AsyncCallback(Callback_Receive),	// callback
				m_Socket);								// socket
			OnConnected();
        }
        catch (Exception ex)
        {
			OnError((int)eSocketError.eError_ConnectFailed, "Callback_Connect failed, " + ex.Message);
			m_Socket.Close();
        }
    }

    private void Callback_Receive(IAsyncResult ar)
    {
        Socket socket = (Socket)ar.AsyncState;

        try
        {
            int iLen = socket.EndReceive(ar);
			if (iLen > 0)
            {
				m_RecvBuffer.AddPacketSize(iLen);

				Packet pk = null;
				while( (pk = m_RecvBuffer.Parse()) != null )
				{
					PacketProc(pk);
				}

				socket.BeginReceive(
					m_RecvBuffer.GetBytes(),				// buffer
					m_RecvBuffer.GetPacketSize(),			// offset
					m_RecvBuffer.GetCapacity(),				// receive bytes
					SocketFlags.None,						// socket option
					new AsyncCallback(Callback_Receive),	// callback
					socket);								// socket
            }
            else
            {
				Disconnect();
            }
        }
        catch (Exception ex)
        {
			OnError((int)eSocketError.eError_RecvFailed, "Callback_Receive failed, " + ex.Message);
			socket.Close();
        }
    }

	private void PacketProc(Packet pk)
	{
		int iProtocol = pk.GetProtocol();
		bool bIsContain = m_PacketHash.ContainsKey(iProtocol);
		if (bIsContain)
		{
			Protocol p = (Protocol)m_PacketHash[iProtocol];
// 			Type tProtocol = p.GetType();
// 			p = (Protocol)Activator.CreateInstance(tProtocol);
			p.Deserialize(pk);
			p.OnEvent();
		}
	}

    public void Send(Protocol p)
    {
        if (!IsConnected())
        {
            return;
        }

        p.Serialize();
        Packet pk = (Packet)p.m_SendBuf;
        int iLen = pk.GetLength();

        try
        {
            SocketError errorCode = 0;
            m_Socket.BeginSend(pk.GetBytes(), 0, iLen, SocketFlags.None, out errorCode, new AsyncCallback(Callback_Send), m_Socket);
        }
        catch (Exception ex)
        {
			OnError((int)eSocketError.eError_SendFailed, "Send failed, " + ex.Message);
			m_Socket.Close();
        }
    }

    private void Callback_Send(IAsyncResult ar)
    {
        int iBytesSent = 0;
        Socket socket = (Socket)ar.AsyncState;

        try
        {
            iBytesSent = socket.EndSend(ar);
        }
        catch (Exception ex)
        {
			OnError((int)eSocketError.eError_SendFailed, "Callback_Send failed, " + ex.Message);
        }
    }

    public void Disconnect()
    {
        if (m_Socket == null)
        {
            return;
        }

        try
        {
            m_Socket.BeginDisconnect(false, new AsyncCallback(Callback_Close), m_Socket);
        }
        catch (Exception ex)
        {
			OnError((int)eSocketError.eError_DisconnectFailed, "Disconnect failed, " + ex.Message);
        }
    }

    private void Callback_Close(IAsyncResult ar)
    {
        Socket socket = (Socket)ar.AsyncState;
        try
        {
            socket.EndDisconnect(ar);
			OnClosed();
        }
        catch (Exception ex)
        {
			OnError((int)eSocketError.eError_DisconnectFailed, "Callback_Close failed, " + ex.Message);
			socket.Close();
        }
    }

    public Socket GetSocket()
    {
        return m_Socket;
    }

    public bool IsConnected()
    {
        if (m_Socket != null)
        {
            return m_Socket.Connected;
        }

        return false;
    }

    // Ionic.Zlib 예제
    /*
    String str = "aaaaaaaaaaaaaaaa{";
    Debug.Log( "Prev Length : " + str.Length );
	
    byte[] arr = new byte[1024];
    arr = System.Text.Encoding.Default.GetBytes( str );
	
    var compressed = Ionic.Zlib.ZlibStream.CompressBuffer( arr );
    Debug.Log( compressed.Length );
	
    var decom = Ionic.Zlib.ZlibStream.UncompressBuffer( compressed );
    String re = System.Text.Encoding.Default.GetString( decom );
    Debug.Log( decom.Length + "string : " + re );
    */
    /*
    public virtual int Compress(byte[] arr, out byte[] buffer)
    {
        buffer = Ionic.Zlib.ZlibStream.CompressBuffer(arr);
        return buffer.Length;
    }

    public virtual int Uncompress(byte[] buffer, out byte[] arr)
    {
        arr = Ionic.Zlib.ZlibStream.UncompressBuffer(buffer);
        return arr.Length;
    }
    */
}
