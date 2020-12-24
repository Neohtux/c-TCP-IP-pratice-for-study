#include <iostream>
#include "Session.h"
#include "Connector.h"
#include "SessionManager.h"
#define PORT_NUMBER 12880
using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

using boost::asio::ip::tcp;

int main()
{

	SessionManager* sm;
	sm = SessionManager::GetInstance();

	io_context m_iocontext;

	ip::tcp::endpoint _ep(ip::address::from_string("127.0.0.1"), PORT_NUMBER);

	Connector* connector = new Connector(m_iocontext);
	connector->Start_Connect(_ep);
	m_iocontext.run();

	
	

	Session* _session;
	while (true) 
	{
		for (long i = 1; i < 300000000; ++i)
		{
		}
		sm->_sessionMap->at(1)->Send();
		//_session->Read();
		//std::cout << " i ";

	}
	return 0;

}