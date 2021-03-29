#ifndef _CPLUS_SERVER_H_
#define _CPLUS_SERVER_H_

#include <thread>
#include <memory>
#include <string>
#include "zmq.hpp"

class CplusServer
{
public:
    CplusServer(std::string addr);
    ~CplusServer();
    void run();
private:
    zmq::context_t _ctx{(1)};
    zmq::socket_t _socket;
    std::string _addr{"tcp://*:5555"};
};

#endif //_CPLUS_SERVER_H_
