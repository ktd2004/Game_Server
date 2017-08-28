#pragma once

//<
#include <NetLink.h>

/**
	p2p ��Ʈ�� �̺�Ʈ �ڵ鷯

	���� pc ���� �߻��ϴ� ��Ʈ�� �̺�Ʈ�� ó���ϴ� �������̽��� �����Ѵ�.
*/
struct P2PAgentHandler
{
	/**
		���� Pc �� �����ϸ� ȣ��Ǵ� �̺�Ʈ �Լ�
		@param spLink Ȧ��Ī�� �Ϸ�� ���� pc�� NetLink
	*/
	virtual bool			OnConnected( NetLinkPtr spLink ) = 0;
	/**
		���� Pc�� ������ ����Ǹ� ȣ��Ǵ� �̺�Ʈ �Լ�
		@param spLink ������ ����� ���� pc�� NetLink
	*/
	virtual bool			OnClosed( NetLinkPtr spLink ) = 0;
	/**
		���� Pc���� p2p ��Ŷ�� �����ϸ� ȣ��Ǵ� �̺�Ʈ �Լ�
		@param spLink ��Ŷ�� ������ ���� pc�� NetLink
		@param ���ŵ� ��Ŷ�� ������ ������ �ּҰ�
	*/
	virtual bool			OnReceived( NetLinkPtr spLink, Buffer* pBuf ) = 0;
};

/** 
	���� pc �� Ȧ��Ī, ���� ������ �����ϸ� ���� pc �� reliable/unreliable ��Ŷ �ۼ����ϴ� �������̽��� �����Ѵ�.
*/
struct P2PAgent
{
	/** 
		Ȧ��Ī�� �Ϸ�� ���� pc ���� ����� NetLink �������� ��ȸ�Ѵ�.
	*/
	virtual NetLinkMap&		GetNetLinkMap(void ) = 0;
	/** 
		P2P ��Ŷ�� �ۼ����ϴ� UDP ��Ĺ�� ��Ʈ�� ������ ���´�.
	*/
	virtual Network_IF&		Self( void ) = 0;
	/** 
		���� pc �� Ȧ��Ī�� �����Ѵ�.
		@param nif ������ P2PAgent�� ��Ʈ�� �ּ�
	*/
	virtual NetLink*		Connect( const Network_IF& nif ) = 0;
	/** 
		���� pc �� ������ �����Ѵ�.
		@param pLink ���� pc�� NetLink
	*/
	virtual bool			Close( NetLink* pLink ) = 0;
	/** 
		���� pc �� ã�´�.
		@param nif ������ P2PAgent�� ��Ʈ�� �ּ�
	*/
	virtual NetLink*		Find( const Network_IF& nif ) = 0;
	/** 
		p2p ��Ŷ ������ ������ NetLink�� ��ȸ�Ѵ�
	*/
	virtual NetLink*		Relay( void ) = 0;
	/** 
		���� pc ���� ����� NetLink �������� ��� �����.
	*/
	virtual void			Clear( void ) = 0;
	/** 
		Ȧ��Ī, ��������, reliable/unreliable ��Ŷ�� �ۼ����Ѵ�.
		@param iWait ��Ŷ ���� ��� �ð�
	*/
	virtual void			Process( unsigned long iWait ) = 0;
};

/** 
	P2PAgent �� �����ϴ� �����Լ��� �����ϴ� ���ӽ����̽�.
*/
struct P2PAgentFactory
{
	/** 
		P2PAgent ��ü�� �����Ѵ�.
		@param rSelf �ڽ��� ��Ʈ�� ����
		@param pHandler ��Ʈ�� �̺�Ʈ�� ó���� �ڵ鷯 ��ü�� �ּҰ�
		@param iMaxBufferSize �ۼ�����Ŷ�� �ִ� ũ��
		@param iKeepConnection �������� ��Ŷ �ۼ��� ����(ms)
	*/
	static P2PAgent*		Create( const Network_IF& rSelf, P2PAgentHandler* pHandler,
								unsigned int iMaxBufferSize = 4096, unsigned long iKeepConnection = 1000 );
};