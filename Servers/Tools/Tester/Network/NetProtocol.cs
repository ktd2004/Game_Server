
namespace NetProtocol
{
	// !!! 패킷 추가 시에 반드시 서버로부터 응답을 받아야 하는 패킷은 NetProtocolMatch::Set에 추가해 줘야 한다. 
	enum eGatewayProtocol
	{
		eCLIENT_GTWS_LOGIN			= 3001, 	// send to gateway
		eGTWS_CLIENT_LOGIN_ERROR	= 2001, 	// from gateway
		eGTWS_CLIENT_LOGIN_SUCCESS	= 2002, 	// from gateways
	}
	
	enum eChannelProtocol
	{
		eCLIENT_TEST_PACKET			= 3003, 	// send to channel
		eCLIENT_TEST_PACKET_ACK		= 3004, 	// recv from channel
	}
}	// namespace NetProtocol
