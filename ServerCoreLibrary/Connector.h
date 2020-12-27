#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include "Session.h"
class Connector
{
public:
	Connector(boost::asio::io_context& io_context);
	void Start_Connect(boost::asio::ip::tcp::endpoint& ep, ServerSession* new_session);

private:
	boost::asio::io_context& _io_context;
	void OnConnectedCompleted(const boost::system::error_code& error, ServerSession* session);
};