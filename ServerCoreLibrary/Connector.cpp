#include<string>
#include<memory>
#include<iostream>
#include"Connector.h"
#include "SessionManager.h"
Connector::Connector(boost::asio::io_context& io_context):_io_context(io_context){}

void Connector::Start_Connect(boost::asio::ip::tcp::endpoint& ep)
{
	std::cout << "���� �õ���..." << '\n';
	try
	{
		Session* new_session = SessionManager::GetInstance()->GenerateSession(_io_context);

		new_session->Socket().async_connect(ep, boost::bind(&Connector::OnConnectedCompleted, this,
			boost::asio::placeholders::error, new_session));
	}
	catch (std::exception e)
	{
		std::cout << e.what() << '\n';
	}
}
void Connector::OnConnectedCompleted(const boost::system::error_code& error,Session* session)
{
	if (error)
	{
		std::cout << "���� ����  " << error.message() << '\n';
		return;
	}
	bool isSocketOpen = session->Socket().is_open();

	if (isSocketOpen && (!error))
	{
		std::cout << "���� ����" << '\n';

		//TODO		 make Session 
		//session->Read();
		session->Send();


	}
}