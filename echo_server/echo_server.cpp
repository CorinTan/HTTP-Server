#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

class EchoSever {
public:
    EchoSever(int argc, const char *argv[]) : argc_{argc}, argv_{argv}, my_socket_fd_{-1}, client_fd_{-1} { 
        init();
    }

    EchoSever(const EchoSever &) = delete;
    EchoSever &operator=(const EchoSever &) = delete;

    void startServer() {
        createSocket();
        bindSocket();
        listenSocket();
    }

    void acceptConnection() {
        socklen_t len = sizeof(clientAddress_);
        while (client_fd_ == -1) {
            client_fd_ = accept(my_socket_fd_, reinterpret_cast<sockaddr *>(&clientAddress_), &len);
            // std::cout << "client_fd = " << client_fd_ << std::endl;
        }
    }

    void echo() {
        while (true) {
            std::string msg(1000, ' ');
            int recv_len = recv(client_fd_, msg.data(), msg.size(), MSG_NOSIGNAL); 
            if (recv_len == 0) // 客户端断开
                break;
            send(client_fd_, msg.data(), recv_len, MSG_NOSIGNAL);
        }
    }

    ~EchoSever() {
        // close
        closeServer();
    }
    
    void closeServer() {
        close(my_socket_fd_);
    }

private:

    void init() {
        memset(&myAddress_, 0, sizeof(myAddress_));
        memset(&clientAddress_, 0, sizeof(clientAddress_)); 
        parseArguments();
    }

    void parseArguments() {
        if (argc_ <= 3) {
            std::cerr << "usage: " << argv_[0] << " ip_address port_number backlog" << std::endl;
            return ;
        }
        
        std::string ip_str = argv_[1], port_str = argv_[2], backlog_str = argv_[3];
        int port = std::stoi(port_str);
        backlog_ = std::stoi(backlog_str);
        
        myAddress_.sin_family = AF_INET;
        inet_aton(ip_str.data(), &myAddress_.sin_addr);
        myAddress_.sin_port = htons(port);
    }

    int createSocket() {
        my_socket_fd_= socket(AF_INET, SOCK_STREAM, 0);
        // 错误处理
        return my_socket_fd_;
    }
    
    int bindSocket() {
        int ret = bind(my_socket_fd_, reinterpret_cast<sockaddr *>(&myAddress_), sizeof(myAddress_));
        // 错误处理
        return ret;
    }

    int listenSocket() {
        int ret = listen(my_socket_fd_, backlog_);
        // 错误处理
        return ret;
    }

    int argc_;
    const char **argv_;
    int backlog_;
    
    struct sockaddr_in myAddress_;
    int my_socket_fd_;
    struct sockaddr_in clientAddress_; 
    int client_fd_;
};