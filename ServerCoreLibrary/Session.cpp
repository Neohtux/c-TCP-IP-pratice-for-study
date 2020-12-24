#pragma once
#include"Session.h"
#include<iostream>
#define SEND_LENGTH	256
Session::Session(boost::asio::io_context& io_context, const int& session_ID)
	:_socket(io_context), _session_ID(session_ID) {}

void Session::StartSession()
{
	isDisconnected = false;
	this->Read();

}
#pragma region Send
void Session::Send()
{
	//_buffer는 char buffer[255];
	_buffer[0] = 'H';
	_buffer[1] = 'E';
	_buffer[2] = 'L';
	_buffer[3] = 'L';
	_buffer[4] = 'O';
	//
	// TODO . Buffer 관리자 필요
	//
	boost::asio::async_write(_socket,
		boost::asio::buffer(_buffer, 5),
		boost::bind(&Session::OnSendCompleted, this, boost::asio::placeholders::error));

}
void Session::OnSendCompleted(const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "Send completed Message : " << _buffer << '\n';
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

	_socket.async_read_some(boost::asio::buffer(_buffer, 5),
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

	if (!error)
	{
		std::cout << "byte_transferred  : " << byte_transferred << '\n';
		std::cout << "From " << _session_ID << " : " << _buffer << '\n';
		Read();
	}
	else std::cout << error.message() << '\n';

	// TODO. 파싱이 끝난 데이터를 클라이언트 소켓에 전달해야함.
	


}
#pragma endregion
void Session::DisConnect()
{
	// 종료하려는 소켓이 열려 있을때만 닫는다.
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

