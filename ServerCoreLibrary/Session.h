#pragma once
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#define BUFFER_SIZE	4096
class Session
{

public:
	Session(boost::asio::io_context& io_context, const int& session_ID);
	void StartSession();

	void Send();
	void Read();
	void Read(std::size_t transferredLength);

	void DisConnect();
	boost::asio::ip::tcp::socket& Socket() { return _socket; };

private:
	//TODO Buffer 관리자 생성 필요 세션 분리화 (Client, Server) 작업 이후 해야함.
	char _buffer[4096];
	boost::asio::ip::tcp::socket _socket;
	bool isDisconnected;
	int _session_ID;

	void OnSendCompleted(const boost::system::error_code& error);
	void OnReadCompleted(const boost::system::error_code& error, const size_t& byte_trasffered);
	void OnDisConnected();


};