#include "cplus_server.h"
#include <iostream>
#include <functional>
#include <memory>
#include <map>

using namespace std;

CplusServer::CplusServer(std::string addr):_socket(_ctx, ZMQ_REP), _addr(addr){
    _socket.bind (addr);
}

CplusServer::~CplusServer(){
    _ctx.close();
}

void CplusServer::run(){
    while (true) {
        zmq::message_t request;
        // 等待客户端请求
        _socket.recv (&request);
        // std::cout << "收到 " << (*(request.data())) << std::endl;
        printf("recv %s \r\n", request.data());
        // 应答World
        zmq::message_t reply (5);
        memcpy ((void *) reply.data (), "World", 5);
        _socket.send (reply);
    }
}

template<typename F>
inline void CplusServer::bind( std::string name, F func )
{
    _handlers[name] = std::bind(&CplusServer::callproxy<F>, this, func, std::placeholders::_1, std::placeholders::_2);
}

template<typename F, typename S>
inline void CplusServer::bind(std::string name, F func, S* s)
{
    _handlers[name] = std::bind(&CplusServer::callproxy<F, S>, this, func, s, std::placeholders::_1, std::placeholders::_2);
}

inline json* CplusServer::call(std::string name, json& method_args){
	json* ret = new json();
	if (_handlers.find(name) == _handlers.end()) {
		(*ret)["res"] = 0;
		return ret;
	}
	auto fun = _handlers[name];
	fun(ret, method_args);
	return ret;
}