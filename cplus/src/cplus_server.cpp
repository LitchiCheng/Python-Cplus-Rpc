#include "cplus_server.h"
#include <iostream>
#include <functional>
#include <memory>
#include <map>

using namespace std;

class Test
{
public:
    int add(int a, int b){
        return a+b;
    }
};

CplusServer::CplusServer(std::string addr):_socket(_ctx, ZMQ_REP), _addr(addr){
    _socket.bind (addr);
}

CplusServer::~CplusServer(){
    _ctx.close();
}

void CplusServer::run(){
    Test a;
    bind("add", &Test::add, &a);
    while (true) {
        zmq::message_t request;
        // 等待客户端请求
        _socket.recv (&request);

        std::string json_str = (char*)request.data();
        try
        {
            json recv_data = json::parse(json_str);
            // cout << " " << recv_data["method_name"] << endl;
            json* ret = call(recv_data["method_name"], recv_data);
            cout << "ret is " << (*ret)["res"] << endl;
            // 应答World
            zmq::message_t reply;
            memcpy ((void *) reply.data (), ret->dump().c_str(), sizeof((char*)(ret)));
            cout << " reply data is " << (char*)reply.data() << endl;
            delete ret;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        
        json dd;
        dd["res"] = 3;
        _socket.send (dd.dump().c_str(), strlen(dd.dump().c_str()));
        
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
	}else{
        cout << "find function " << endl;
    }
	auto fun = _handlers[name];
	fun(ret, method_args);
	return ret;
}

template<typename F>
inline void CplusServer::callproxy( F fun, json* ret, json& rec ){
	callproxy_(fun, ret, rec);
}

template<typename F, typename S>
inline void CplusServer::callproxy(F fun, S * s, json* ret, json& rec ){
	callproxy_(fun, s, ret, rec);
}