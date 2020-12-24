#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <functional>
#include <vector>
#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/asio/placeholders.hpp>
#include "Session.h"
#define PORT_NUMBER	12880

class Listener
{
public:
	Listener(boost::asio::io_context& iosvc, boost::asio::ip::tcp::endpoint& ep);


	// Todo  void 는 추후 Session으로 교체
	std::vector<std::function<void()>> Func;
	//std::function<void()> Func;

	void Start_accept();

	//  To do void 는 추후 Session으로 교체
	void operator += (std::function<void()>& func)
	{
		Func.push_back(func);
	}


private:
	boost::asio::io_context& _iosvc;
	boost::asio::ip::tcp::acceptor _acceptor;
	//void OnAcceptComplete();
	void OnAcceptComplete(const boost::system::error_code& error,Session* session);
};