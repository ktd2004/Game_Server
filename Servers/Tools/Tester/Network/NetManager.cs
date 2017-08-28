using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Text;

using NetEnum;

public class NetManager
{
    protected Hashtable m_Sockets;
    private PacketTester tester = null;

	public NetManager(PacketTester t)
    {
        m_Sockets = new Hashtable();
        tester = t;
    }

    public void AddSocket(NetSocket ns)
    {
		lock(typeof(NetManager))
		{
			m_Sockets[ns.GetSocket()] = ns;
		}
    }

    public void QuitSockets()
    {
		lock (typeof(NetManager))
		{
			if (m_Sockets.Count <= 0)
			{
				return;
			}

			foreach (DictionaryEntry entry in m_Sockets)
			{
				NetSocket ns = (NetSocket)entry.Value;
				ns.Disconnect();
			}

			m_Sockets.Clear();
		}
    }

    void OnApplicationQuit()
    {
		lock (typeof(NetManager))
		{
			if (m_Sockets.Count <= 0)
			{
				return;
			}

			foreach (DictionaryEntry entry in m_Sockets)
			{
				NetSocket ns = (NetSocket)entry.Value;
				ns.Disconnect();
			}

			m_Sockets.Clear();
		}
    }
}
