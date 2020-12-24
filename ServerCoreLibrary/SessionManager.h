#pragma once
#include<unordered_map>
#include"Session.h"
class SessionManager
{
public:
	static std::mutex _sessionIDLock;
	static SessionManager* GetInstance();

	//  key : session_ID ,  value : 클라이언트 session
	std::unordered_map<int, Session*>* _sessionMap = new std::unordered_map<int, Session*>();
	
	//신규 세션 생성 함수
	Session* GenerateSession(boost::asio::io_context& iosvc);
	int GetSessionID() { return _sessionID; }
private:
	SessionManager() {};
	~SessionManager() {};
	 
	//공유자원(locK) 세션 구분
	int _sessionID=0;
	static SessionManager* _Instance;
};