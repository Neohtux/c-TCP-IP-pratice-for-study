#pragma once
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include <queue>
#include <vector>
#define BUFFER_SIZE	4096

class Session
{

public:
	static std::mutex _cSessionMsgLock; //Client Session Send Message Queue Lock
	Session() {	}
	Session(boost::asio::io_context& io_context, const int& session_ID);
	void StartSession();

	void Send(char* buffer, int sessionID);
	void Read();
	void Read(std::size_t transferredLength);
	void StartSend();
	void DisConnect();
	unsigned int getDataLength(const char* buffer);
	void InitSocket(boost::asio::io_context& io_context);

	
	std::unique_ptr<boost::asio::ip::tcp::socket>& Socket() { return _socket; };
	
protected:
	//TODO Buffer 관리자 생성 필요 세션 분리화 (Client, Server) 작업 이후 해야함.	
	char _buffer[4096]; 
	
	std::unique_ptr<boost::asio::ip::tcp::socket> _socket = nullptr;
	bool isDisconnected = false;
	bool _isSendWait = false;
	int _session_ID;
	
	// TODO. race condition need to lock  락 필요
	std::queue<char*> _sendMessageQueue;

	 virtual void OnSendCompleted(const boost::system::error_code& error,char* buffer);
	 virtual void OnReadCompleted(const boost::system::error_code& error, const size_t& byte_transffered);
	 virtual void OnDisConnected();
};

