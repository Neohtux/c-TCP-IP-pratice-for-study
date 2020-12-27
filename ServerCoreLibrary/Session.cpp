#pragma once
#include<iostream>
#include "SessionManager.h"
#define SEND_LENGTH	256
#pragma region  ServerSession 


//used for client (ServerSession)
ServerSession::ServerSession(boost::asio::io_context& io_context, const int& session_ID)
	:_socket(io_context), _session_ID(session_ID) {}

unsigned int ServerSession::getDataLength(const char* buffer)
{
	int _lengthIdx = 0;
	while (buffer[_lengthIdx] != '\0')
	{
		_lengthIdx++;
	}

	return _lengthIdx;
}
void ServerSession::Disconnect()
{
	// �����Ϸ��� ������ ���� �������� �ݴ´�.
	if (_socket.is_open())
	{
		_socket.close();
		isDisconnected = true;
		//OnDisConnected();
	}
}
void ServerSession::Send(const char* buffer) 
{
	std::scoped_lock(_sSessionMsgLock);
	_sendMessageQueue.push(buffer);

	if (!_isSendWait) //���� Send���� �ƴ϶�� StartSend() 
	{
		ServerSession::StartSend();
	}
}
void ServerSession::OnSendCompleted(const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "Client Send completed Message : " << _buffer << '\n';
		_isSendWait = false;
	}

	else
	{
		std::cout << error.message() << '\n';
	}
}
void ServerSession::StartSend()
{
	//_buffer�� char buffer[255];
	if (isDisconnected) return;

	_isSendWait = true;

	const char* buffer = _sendMessageQueue.front();
	_sendMessageQueue.pop();

	unsigned int _dataLength = getDataLength(buffer);

	boost::asio::async_write(_socket,
		boost::asio::buffer(buffer, _dataLength),
		boost::bind(&ServerSession::OnSendCompleted, this, boost::asio::placeholders::error));
}
void ServerSession::Read()
{
	if (isDisconnected)
	{
		Disconnect();
		return;
	}
	memset(&_buffer, '\0', sizeof(_buffer));

	_socket.async_read_some(boost::asio::buffer(_buffer, 14),
		boost::bind(&ServerSession::OnReadCompleted, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void ServerSession::OnReadCompleted(const boost::system::error_code& error, const size_t& byte_transffered)
{
	if (!error)
	{
		std::cout << "byte_transferred  : " << byte_transffered << '\n';
		std::cout << "[From Server] " << _session_ID << " : " << _buffer << '\n';

		Read();

	}
	else
	{
		std::cout << error.message() << '\n';
	}
}
void ServerSession::StartSession()
{
	isDisconnected = false;
	std::cout << "Client thread ID : " << std::this_thread::get_id << '\n';
	std::cout << _socket.remote_endpoint() << '\n';
	Read();
}
#pragma endregion
Session::Session(boost::asio::io_context& io_context, const int& session_ID)
	:_socket(io_context), _session_ID(session_ID) {}

void Session::StartSession()
{
	isDisconnected = false;
	//std::cout << "thread ID : " << std::this_thread::get_id << '\n';
	std::cout << _socket.remote_endpoint() << '\n';
	Read();
}
#pragma region Send
void Session::Send(const char* buffer)
{
	//Danger zone �� ���ǰ� ť�� ���� ���̽� ����� �߻�
	std::scoped_lock(_cSessionMsgLock);
	_sendMessageQueue.push(buffer);

	if (!isSendWait) //���� Send���� �ƴ϶�� StartSend() 
	{
		StartSend();
	}
}
void Session::StartSend()
{
	//_buffer�� char buffer[255];
	if (isDisconnected) return;

	isSendWait = true;

	const char* buffer = _sendMessageQueue.front();
	_sendMessageQueue.pop();

	unsigned int _dataLength = getDataLength(buffer);


	boost::asio::async_write(_socket,
		boost::asio::buffer(buffer, _dataLength),
		boost::bind(&Session::OnSendCompleted, this, boost::asio::placeholders::error));

}
void Session::OnSendCompleted(const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "Send completed Message : " << _buffer << '\n';
		isSendWait = false;
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

}
void Session::Read()
{
	if (isDisconnected)
	{
		DisConnect();
		return;
	}
	memset(&_buffer, '\0', sizeof(_buffer));

	_socket.async_read_some(boost::asio::buffer(_buffer,14),
		boost::bind(&Session::OnReadCompleted, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}
void Session::OnReadCompleted(const boost::system::error_code& error, const std::size_t& byte_transferred)
{

	//
	// TODO. ���� (���빰�� �ִ� ��� �޴°ɷ� �߰�) , �װԾƴϸ� Disconnect()
	//
	// TODO. �������� �Ľ��ؾ���. �Ľ� �߰�.

	if (!error)
	{
		std::cout << "byte_transferred  : " << byte_transferred << '\n';
		std::cout << "[From Client] " << _session_ID << " : " << _buffer << '\n';
	
		Read();
		
	}
	else
	{
		std::cout << error.message() << '\n';
	}

	// TODO. �Ľ��� ���� �����͸� Ŭ���̾�Ʈ ���Ͽ� �����ؾ���.
	


}
#pragma endregion
void Session::DisConnect()
{
	// �����Ϸ��� ������ ���� �������� �ݴ´�.
	if (_socket.is_open())
	{
		_socket.close();
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

