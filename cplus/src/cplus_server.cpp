#include "cplus_server.h"
#include <iostream>

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
        std::cout << "收到 Hello" << std::endl;
        // 应答World
        zmq::message_t reply (5);
        memcpy ((void *) reply.data (), "World", 5);
        _socket.send (reply);
    }
}