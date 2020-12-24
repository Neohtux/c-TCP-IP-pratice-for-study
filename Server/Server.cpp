#include <iostream>
#include "Listener.h"
#include "SessionManager.h"
using namespace boost::asio;
SessionManager* _sessionManager;
int main()
{
	_sessionManager = SessionManager::GetInstance();
	try
	{
		boost::asio::io_context iocontext;
		ip::tcp::endpoint ep(ip::tcp::v4(), PORT_NUMBER); //ipv4, 포트번호
		auto _listener = std::make_unique<Listener>(iocontext, ep);

		_listener->Start_accept();
		iocontext.run();

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	

	/*while (true) 
	{
		int w = _sessionManager->_sessionMap->size();
		int q = 1;
		int m = 3;
		std::cout << "asdasdsadasdasdadsad \n" << '\n';
	}*/
	return 0;

}