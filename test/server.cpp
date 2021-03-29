#include "cplus_server.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    CplusServer test_server("tcp://*:5555");
    test_server.run();
    return 0;
}
