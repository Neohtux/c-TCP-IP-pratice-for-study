#pragma once
#include <iostream>
#include <memory>
#include "Listener.h"
#include "SessionManager.h"
Listener::Listener(boost::asio::io_context& iosvc, boost::asio::ip::tcp::endpoint& ep) :
	 _acceptor(iosvc, ep), _iosvc(iosvc) {}

void Listener::Start_accept()
{
	using namespace boost::asio;

	//클라이언트 세션 생성
	
	Session* new_session = SessionManager::GetInstance()->GenerateSession(_iosvc);
	_acceptor.async_accept(new_session->Socket(),
	boost::bind(&Listener::OnAcceptComplete, this, boost::asio::placeholders::error, new_session));
}
void Listener::OnAcceptComplete(const boost::system::error_code& error,Session* session)
{
	if (!error) //정상적으로 accept가 된경우
	{
		std::cout << "accept completed" << '\n';
		
		//session->Send();
		session->Read();


	}

	else
	{
		std::cout << error.message() << '\n';
	}
	Start_accept();
}
