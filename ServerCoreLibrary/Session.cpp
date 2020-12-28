#pragma once
#include<iostream>
#include "SessionManager.h"
#include "RecvBuffer.h"
#define SEND_LENGTH	256

Session::Session(boost::asio::io_context& io_context, const int& session_ID)
	: _session_ID(session_ID) 
{
	InitSocket(io_context);
}
void Session::InitSocket(boost::asio::io_context& io_context)
{
	_socket = std::make_unique<boost::asio::ip::tcp::socket>(io_context);
}
void Session::StartSession()
{
	isDisconnected = false;
	std::cout << _socket->remote_endpoint() << '\n';
	Read();
}
#pragma region Send
void Session::Send(char* buffer,int sessionID)
{
	//Danger zone 각 세션간 큐에 대해 레이스 컨디션 발생
	std::scoped_lock(_cSessionMsgLock);

	char* _buffer = buffer;
	//strcat_s(_buffer,1,"1");
	_sendMessageQueue.push(_buffer);
	
	if (!_isSendWait || !_sendMessageQueue.empty()) //현재 Send중이 아니라면 StartSend() 
	{
		StartSend();
	}
}
void Session::StartSend()
{
	if (isDisconnected) return;

	_isSendWait = true;
	 char* sendbuffer = nullptr;

	sendbuffer = _sendMessageQueue.front();
	_sendMessageQueue.pop();
	
	
	unsigned int _dataLength = getDataLength(sendbuffer);


	boost::asio::async_write(*_socket,
		boost::asio::buffer(sendbuffer, _dataLength),
		boost::bind(&Session::OnSendCompleted, this, boost::asio::placeholders::error,sendbuffer));

}
void Session::OnSendCompleted(const boost::system::error_code& error,char* buffer)
{
	if (!error)
	{
		std::cout << "Send completed Message : " << buffer << '\n';
		_isSendWait = false;
	}

	else
	{
		std::cout << error.message() << '\n';
	}
}
#pragma endregion
#pragma region Read()
void Session::Read(size_t transferredLength)
{
	// TODO overloading received sized

}
void Session::Read()
{
	if (isDisconnected)
	{
		DisConnect();
		return;
	}
	memset(&_buffer, '\0', sizeof(_buffer));
	
	_socket->async_read_some(boost::asio::buffer(_buffer, sizeof(_buffer)),
		boost::bind(&Session::OnReadCompleted, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}
void Session::OnReadCompleted(const boost::system::error_code& error, const std::size_t& byte_transferred)
{

	//
	// TODO. 조건 (내용물이 있는 경우 받는걸로 추가) , 그게아니면 Disconnect()
	//
	// TODO. 받은다음 파싱해야함. 파싱 추가.

	if (!error && byte_transferred > 0)
	{
		std::cout << "byte_transferred  : " << byte_transferred << '\n';
		std::cout << "[From Client] " << _session_ID << " : " << _buffer << '\n';
	

		Read();
		Send("server response 어후 제발 성공하자 이번프로젝트", 2);
	}
	else
	{
		std::cout << error.message() << '\n';
		DisConnect();
	}

	// TODO. 파싱이 끝난 데이터를 클라이언트 소켓에 전달해야함.

}
#pragma endregion
void Session::DisConnect()
{
	// 종료하려는 소켓이 열려 있을때만 닫는다.
	if (_socket->is_open())
	{
		_socket->close();
		isDisconnected = true;
		OnDisConnected();
	}

}
void Session::OnDisConnected()
{
	std::cout << _session_ID << " Disconnected" << '\n';

	//TODO  need to make session Manager vector 
}
unsigned int Session::getDataLength(const char* buffer)
{
	int _lengthIdx = 0;
	while (buffer[_lengthIdx] != '\0')
	{
		_lengthIdx++;
	}

	return _lengthIdx;
}

