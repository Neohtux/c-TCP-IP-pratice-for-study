#pragma once
#include "SessionManager.h"

SessionManager* SessionManager::_Instance = nullptr;

SessionManager* SessionManager::GetInstance()
{
	if (_Instance == nullptr)
		_Instance = new SessionManager();

	return _Instance;
}
Session* SessionManager::GenerateSession(boost::asio::io_context& iosvc)
{
	//lock on
	std::scoped_lock(_sessionIDLock);
	/************ lock ************/
	int _session_id = ++_sessionID;
	Session* _session = new Session(iosvc, _session_id);
	_sessionMap->insert(std::pair<int, Session*>(_session_id, _session));

	//Session& s_ptr = *_sessionMap->at(_session_id);
	return _session;
	/****************************/
}
