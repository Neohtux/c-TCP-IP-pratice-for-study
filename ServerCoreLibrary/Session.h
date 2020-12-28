#pragma once
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include <queue>
#define BUFFER_SIZE	4096

class Session
{

public:
	static std::mutex _cSessionMsgLock; //Client Session Send Message Queue Lock
	Session(boost::asio::io_context& io_context, const int& session_ID);
	void StartSession();

	virtual void Send(char* buffer, int sessionID);
	virtual void Read();
	void Read(std::size_t transferredLength);
	virtual void StartSend();
	void DisConnect();
	unsigned int getDataLength(const char* buffer);
	void initSocket(boost::asio::io_context& io_context);

	//boost::asio::ip::tcp::socket& Socket() { return _socket; };
	std::unique_ptr<boost::asio::ip::tcp::socket>& Socket() { return _socket; };
	
protected:
	//TODO Buffer 관리자 생성 필요 세션 분리화 (Client, Server) 작업 이후 해야함.
	char _buffer[4096];
	//boost::asio::ip::tcp::socket _socket;
	std::unique_ptr<boost::asio::ip::tcp::socket> _socket;
	bool isDisconnected = false;
	bool _isSendWait = false;
	int _session_ID;
	
	// TODO. race condition need to lock  락 필요
	std::queue<char*> _sendMessageQueue;

	 virtual void OnSendCompleted(const boost::system::error_code& error);
	 virtual void OnReadCompleted(const boost::system::error_code& error, const size_t& byte_transffered);
	 virtual void OnDisConnected();

};

class ServerSession
{
public:
	static std::mutex _sSessionMsgLock; //Server Session Send Message Queue Lock
	
	ServerSession(boost::asio::io_context& io_context, const int& session_ID);

	unsigned int getDataLength(const char* buffer);
	void StartSession();
	void Send(char* buffer, int _sessionID);
	void Read();
	void StartSend();
	void Disconnect();
	void OnSendCompleted(const boost::system::error_code& error) ;
	void OnReadCompleted(const boost::system::error_code& error, const size_t& byte_transffered);
	void initSocket(boost::asio::io_context& io_context);
	

	//void OnDisConnected();
	//boost::asio::ip::tcp::socket& Socket() { return _socket; };
	std::unique_ptr<boost::asio::ip::tcp::socket>& Socket() { return _socket; };
private:
	std::queue<char*> _sendMessageQueue;
	char _buffer[4096];
	std::unique_ptr<boost::asio::ip::tcp::socket> _socket;
	bool isDisconnected;
	bool _isSendWait;
	const int _session_ID;
};