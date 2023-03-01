#include "rpc_client.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    rpcClient test_client;
    printf("iiiiiiiii1");
    test_client.start(("ipc:///tmp/test1.ipc"));
        printf("iiiiiiiii2");

    test_client.call("setOn");
    
    
    return 0;
}