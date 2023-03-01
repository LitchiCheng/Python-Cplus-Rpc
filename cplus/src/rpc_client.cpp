/*
 * @Date: 2023-02-28 23:45:45
 * @LastEditTime: 2023-03-01 07:49:39
 * @FilePath: \Python-Cplus-Rpc\cplus\src\rpc_client.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "rpc_client.h"

rpcClient::rpcClient():_socket(_ctx, ZMQ_REQ){
    setTimeout(5);
}

rpcClient::~rpcClient(){
    _ctx.close();
}

void rpcClient::start(std::string addr){
    _socket.connect(addr);
}

void rpcClient::setTimeout(uint32_t ms){
    _socket.setsockopt(ZMQ_SNDTIMEO, ms);
	_socket.setsockopt(ZMQ_RCVTIMEO, ms);
}

void rpcClient::close(std::string addr){
    _socket.disconnect(addr);
}

void rpcClient::call(std::string func_name){
    json call_json;
    call_json["method_name"] = func_name;
    std::string s = call_json.dump();
    zmq::message_t reply (s.size());
    memcpy(reply.data(), (s.c_str()), (s.size()));
    _socket.send(reply);
    zmq::message_t request;
    _socket.recv (&request);
}
