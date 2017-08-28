using System.Collections;

public class Protocol
{
	public int m_iProtocol;
	public Packet m_SendBuf;

	public delegate void EventFunction(Protocol pkt);
	private EventFunction m_Cb = null;

	public void SetEventFunction(EventFunction evt)
	{
		m_Cb = evt;
	}

	public void OnEvent()
	{
		if (m_Cb != null)
			m_Cb(this);
	}

	public Protocol()
	{
		m_SendBuf = new Packet( 4096 );
	}

	public Protocol(int size)
	{
		m_SendBuf = new Packet( size );
	}
	
	public virtual bool Serialize()
	{
		return false;
	}
	
	public virtual bool Deserialize(Packet RecvBuf)
	{
		return false;
	}
}
