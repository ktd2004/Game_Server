using System.Collections;
using System;
using System.Text;
using System.Net;

public class Packet
{
	private byte[]		m_Buff;
	private int			m_iFront;
	private int			m_iBack;
	private int			m_iPacketBytes;
	private int			m_iBuffSize;
	private bool		m_iByteOrder = false;
	private const int	m_iHeaderSize = sizeof(int) + sizeof(int);

	public Packet()
	{
        m_iBuffSize = 4096;
        m_Buff = new byte[m_iBuffSize];
        Reset();
    }

	public Packet(int iSize)
	{
        m_iBuffSize = iSize;
        m_Buff = new byte[m_iBuffSize];
        Reset();
    }

	public void Reset()
	{
		m_iPacketBytes = 0;
		m_iFront = m_iBack = m_iHeaderSize;
		SetLength(0);
		SetProtocol(0);
	}

	public int GetPacketSize()
	{
		return m_iPacketBytes;
	}

	public int AddPacketSize( int iPktBytes )
	{
		m_iPacketBytes += iPktBytes;
		return m_iPacketBytes;
	}

	public Packet Parse()
	{
		if ( m_iPacketBytes > 0 )
		{
			int iPktBytes = GetLength();
			if (iPktBytes <= m_iPacketBytes)
			{
				Packet pk = new Packet(iPktBytes);
				Buffer.BlockCopy(m_Buff, 0, pk.GetBytes(), 0, iPktBytes);
				pk.m_iBack = iPktBytes;
				pk.m_iPacketBytes = iPktBytes;
				Shift();
				return pk;
			}
		}
		return null;
	}

	private void Shift()
	{
		int iPktBytes = GetLength();
		m_iPacketBytes -= iPktBytes;
		if ( m_iPacketBytes > 0 )
		{
			byte[] byBuf = new byte[m_iBuffSize];
			Buffer.BlockCopy(m_Buff, iPktBytes, byBuf, 0, m_iPacketBytes);
			m_iFront = m_iHeaderSize;
			m_Buff = byBuf;
		}
		else
		{
			Reset();
		}
	}
	
	public void SetBytes( byte[] bytes )
	{
		m_Buff = bytes;
	}

	public byte[] GetBytes()
	{
        return m_Buff;
    }

	public int GetBodySize()
	{
		return m_iBack - m_iFront;
    }

	public int GetCapacity()
	{
        return m_iBuffSize;
    }
	
	public int GetSpace()
	{
		return m_iBuffSize - m_iBack;
    }

	private void Reserve(int iSize)
	{
        m_iBack += iSize;
    }

	public void SetLength(int iLen)
	{
		if (m_iByteOrder)
			iLen = (short)IPAddress.HostToNetworkOrder(iLen);
		byte[] byVal = BitConverter.GetBytes(iLen);
		byVal.CopyTo(m_Buff, 0);
	}

	public void SetProtocol(int iProtocol)
	{
		if (m_iByteOrder)
			iProtocol = (int)IPAddress.HostToNetworkOrder(iProtocol);
		byte[] byVal = BitConverter.GetBytes(iProtocol);
		byVal.CopyTo(m_Buff, sizeof(int));
	}

	public int GetLength()
	{
		int iLen = BitConverter.ToInt32(m_Buff, 0);
		if ( m_iByteOrder )
			iLen = (int)IPAddress.NetworkToHostOrder(iLen);
		return iLen;
	}

	public int GetProtocol()
	{
		int iProtocol = BitConverter.ToInt32(m_Buff, sizeof(int));
		if ( m_iByteOrder )
			iProtocol = (int)IPAddress.NetworkToHostOrder(iProtocol);
		return iProtocol;
	}

	public int WriteInt64(Int64 iVal)
	{
		if (m_iByteOrder)
			iVal = IPAddress.HostToNetworkOrder(iVal);
		byte[] byVal = BitConverter.GetBytes(iVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return byVal.Length;
	}

	public int WriteUInt64(UInt64 iVal)
	{
		if (m_iByteOrder)
			iVal = (UInt64)IPAddress.HostToNetworkOrder((Int64)iVal);
		byte[] byVal = BitConverter.GetBytes(iVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return byVal.Length;
	}

	public int WriteInt(int iVal)
	{
		if (m_iByteOrder)
			iVal = IPAddress.HostToNetworkOrder(iVal);
		byte[] byVal = BitConverter.GetBytes(iVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return byVal.Length;
	}

	public int WriteUInt(uint iVal)
	{
		if (m_iByteOrder)
			iVal = (uint)IPAddress.HostToNetworkOrder((int)iVal);
		byte[] byVal = BitConverter.GetBytes(iVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return byVal.Length;
	}
	
	public int WriteUInt16(ushort iVal)
	{
		if (m_iByteOrder)
			iVal = (ushort)IPAddress.HostToNetworkOrder((ushort)iVal);
		byte[] byVal = BitConverter.GetBytes(iVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return byVal.Length;
	}
	
	public int WriteByte(byte iVal)
	{
		if (m_iByteOrder)
			iVal = (byte)IPAddress.HostToNetworkOrder(iVal);
		byte[] byVal = BitConverter.GetBytes(iVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return byVal.Length;
	}

	public int WriteFloat(float fVal)
	{
		byte[] byVal = BitConverter.GetBytes(fVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return byVal.Length;
	}

	public int WriteDouble(double fVal)
	{
		byte[] byVal = BitConverter.GetBytes(fVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return byVal.Length;
	}
	/*
	public int WriteString(string sVal)
	{
		int iLen = WriteInt(sVal.Length);
		byte[] byVal = Encoding.ASCII.GetBytes(sVal);
		byVal.CopyTo(m_Buff, m_iBack);
		m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return iLen + byVal.Length;
	}
	*/
	public int WriteString(string sVal)
    {
		if( sVal == null )
			sVal = "";
			
        byte[] byVal = Encoding.Default.GetBytes(sVal);
        int iLen = WriteInt(byVal.Length);
        byVal.CopyTo(m_Buff, m_iBack);
        m_iBack += byVal.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
        return iLen + byVal.Length;
    }
	
	public int WriteByteArr(byte[] arr)
	{
		arr.CopyTo(m_Buff, m_iBack);
		m_iBack += arr.Length;
		SetLength(GetBodySize() + m_iHeaderSize);
		return arr.Length;
	}
	
	public int ReadByte(out byte iVal)
	{
		iVal = m_Buff[m_iFront];
		if (m_iByteOrder)
			iVal = (byte)IPAddress.NetworkToHostOrder(iVal);
		m_iFront += sizeof(byte);
		return sizeof(byte);
	}
	
	public int ReadUInt16(out ushort iVal)
	{
		iVal = BitConverter.ToUInt16(m_Buff, m_iFront);
		if (m_iByteOrder)
			iVal = (ushort)IPAddress.NetworkToHostOrder(iVal);
		m_iFront += sizeof(ushort);
		return sizeof(ushort);
	}
	
	public int ReadInt64(out Int64 iVal)
	{
		iVal = BitConverter.ToInt64(m_Buff, m_iFront);
		if (m_iByteOrder)
			iVal = IPAddress.NetworkToHostOrder(iVal);
		m_iFront += sizeof(Int64);
		return sizeof(Int64);
	}

	public int ReadUInt64(out UInt64 iVal)
	{
		iVal = BitConverter.ToUInt64(m_Buff, m_iFront);
		if (m_iByteOrder)
			iVal = (UInt64)IPAddress.NetworkToHostOrder((Int64)iVal);
		m_iFront += sizeof(UInt64);
		return sizeof(UInt64);
	}

	public int ReadInt(out int iVal)
	{
		iVal = BitConverter.ToInt32(m_Buff, m_iFront);
		if (m_iByteOrder)
			iVal = IPAddress.NetworkToHostOrder(iVal);
		m_iFront += sizeof(int);
		return sizeof(int);
	}

	public int ReadUInt(out uint iVal)
	{
		iVal = BitConverter.ToUInt32(m_Buff, m_iFront);
		if (m_iByteOrder)
			iVal = (uint)IPAddress.NetworkToHostOrder((int)iVal);
		m_iFront += sizeof(int);
		return sizeof(int);
	}

	public int ReadFloat(out float fVal)
	{
		fVal = BitConverter.ToSingle(m_Buff, m_iFront);
		m_iFront += sizeof(float);
		return sizeof(int);
	}
	
	public int ReadDouble(out double dVal)
	{
		dVal = BitConverter.ToDouble(m_Buff, m_iFront);
		m_iFront += sizeof(double);
		return sizeof(double);
	}
	/*
	public int ReadString(out string sVal)
	{
		// string packet = uint32(string length) + string
		uint iVal = BitConverter.ToUInt32(m_Buff, m_iFront);
		sVal = Encoding.UTF8.GetString(m_Buff, m_iFront + sizeof(uint), (int)iVal);
		m_iFront += sizeof(uint) + (int)iVal;
		return sVal.Length;
	}
	*/
	public int ReadString(out string sVal)
    {			
		uint iVal = BitConverter.ToUInt32(m_Buff, m_iFront);
		byte[] temp = new byte[iVal];
		
		Buffer.BlockCopy( m_Buff,  m_iFront + sizeof(uint), temp, 0, (int)iVal );
		sVal = Encoding.UTF8.GetString( temp );
		sVal = sVal.Trim();
		
		m_iFront += sizeof(uint) + (int)iVal;
        return sVal.Length;
    }
}
