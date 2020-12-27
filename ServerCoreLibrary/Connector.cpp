#include<string>
#include<memory>
#include<iostream>
#include"Connector.h"

Connector::Connector(boost::asio::io_context& io_context):_io_context(io_context){}

void Connector::Start_Connect(boost::asio::ip::tcp::endpoint& ep, ServerSession* new_session)
{
	std::cout << "���� �õ���..." << '\n';
	try
	{
		new_session->Socket().async_connect(ep, boost::bind(&Connector::OnConnectedCompleted, this,
			boost::asio::placeholders::error, new_session));
	}
	catch (std::exception e)
	{
		std::cout << e.what() << '\n';
	}
}
void Connector::OnConnectedCompleted(const boost::system::error_code& error, ServerSession* session)
{
	if (error) // error == ture �� ���ӿ� �����Ѱ��
	{
		std::cout << "���� ����  " << error.message() << '\n';
		delete session;
		return;
	}
	bool isSocketOpen = session->Socket().is_open();

	if (isSocketOpen && (!error)) //������ ���� �Ѱ��
	{

		std::cout << "���� ����" << '\n';
		session->StartSession();
 		session->Send("i'am cli ent!!");
	
	}
}