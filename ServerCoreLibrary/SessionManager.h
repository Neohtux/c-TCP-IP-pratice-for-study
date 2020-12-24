#pragma once
#include<unordered_map>
#include"Session.h"
class SessionManager
{
public:
	static std::mutex _sessionIDLock;
	static SessionManager* GetInstance();

	//  key : session_ID ,  value : Ŭ���̾�Ʈ session
	std::unordered_map<int, Session*>* _sessionMap = new std::unordered_map<int, Session*>();
	
	//�ű� ���� ���� �Լ�
	Session* GenerateSession(boost::asio::io_context& iosvc);
	int GetSessionID() { return _sessionID; }
private:
	SessionManager() {};
	~SessionManager() {};
	 
	//�����ڿ�(locK) ���� ����
	int _sessionID=0;
	static SessionManager* _Instance;
};