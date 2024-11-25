#include <iostream>
#include "echo_server.cpp"
#include <memory>

int main(int argc, const char *argv[]) {
    EchoSever echo_server(argc, argv); 
    echo_server.startServer();
    echo_server.acceptConnection();
    echo_server.echo();
    echo_server.closeServer();

    return 0;
}