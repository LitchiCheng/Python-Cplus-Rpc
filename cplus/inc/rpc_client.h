#ifndef _RPC_CLIENT_H_
#define _RPC_CLIENT_H_

#include <string>
#include "zmq.hpp"
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

class rpcClient
{
public:
    rpcClient();
    ~rpcClient();
    void start(std::string addr);
    void close(std::string addr);
    void call(std::string func_name);
    void setTimeout(uint32_t ms);
private:
    zmq::context_t _ctx{(1)};
    zmq::socket_t _socket;
    std::string _addr{"tcp://*:5555"};
};

#endif //
