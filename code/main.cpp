#include "./config/config.hpp"
// #include ".server/HttpServer.hpp"
#include <memory>


int main(int argc, const char *argv[]) {
    // HttpServer server;
    // server.init(argc, argv);
    // server.start();

    Config &config = Config::getInstance();
    config.parseParms(argc, argv);

    return 0;
}