#include "config.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <arpa/inet.h>
#include <thread>

Config &Config::getInstance() {
    static Config config;
    return config;
}

Config::Config() {
    initSupportedParams();
    initDesiredParams();
    usage_str_ = "./server [-p port] [-h mysql_server_ip] [-u mysql_server_user] [-w mysql_server_password] [-s mysql_conection_count] [-t thread_count]";
}

Config::~Config() {
}

bool Config::parseParms(int argc, const char *argv[]) {
    bool parse_successed = true;
    for (int i = 1; i < argc && parse_successed; ++i) {
        std::string cur_str = argv[i];
        if (cur_str.find("-") != std::string::npos && supported_params_.find(cur_str) != supported_params_.end() && i < argc - 1) {
            std::string next_str = argv[i+1];
            if (desired_params_.find(supported_params_[cur_str]) != desired_params_.end()) {
                desired_params_[supported_params_[cur_str]] = next_str; 
                ++i;
            }else {
                parse_successed = false;
            }
        }else {
            parse_successed = false;
        }
    }

    if (parse_successed) {
        // check port_num、mysql_server_ip、mysql_conection_count、thread_count
        parse_successed = checkPortNum() && checkMySQLIP() && checkMySQLConectionCount() && checkThreadCount();
    }else {
        printUsage();
    }
    // Debug
    printParams(); 
    return parse_successed;
}

void Config::initSupportedParams() {
    supported_params_ = {
        {"-p", "port_num"},
        {"-h", "mysql_server_ip"}, 
        {"-u", "mysql_server_user"},
        {"-w", "mysql_server_password"},
        {"-s", "mysql_conection_count"},  
        {"-t", "thread_count"}, 
    };
}

void Config::initDesiredParams() {
    desired_params_ = {
        {"port_num", "9999"},
        {"mysql_server_ip", "localhost"}, 
        {"mysql_server_user", "root"},
        {"mysql_server_password", "123456"},
        {"mysql_connection_count", "10"},  
        {"thread_count", "10"},
    };
}

void Config::printUsage() {
    std::cout << usage_str_ << std::endl;
}

void Config::printParams() {
    std::cout << "==========Server Params==========" << std::endl;
    for (auto &[key, value] : desired_params_) {
        std::cout << key + ": " << value << std::endl;
    }
}

bool Config::checkPortNum() {
    return checkNumRange("port_num", 0, UINT16_MAX);
}

bool Config::checkMySQLIP() {
    std::string_view ip_maybe = desired_params_["mysql_server_ip"];
    if (ip_maybe == "localhost")
        return true;
    return checkIPV4(ip_maybe);
}

bool Config::checkMySQLConectionCount() {
   return checkNumRange("mysql_connection_count", 0, UINT16_MAX); 
}

bool Config::checkThreadCount() {
    return checkNumRange("thread_count", 0, std::thread::hardware_concurrency()); 
}

bool Config::checkNumRange(const std::string &key, int low, int high) {
    bool check_result = true;
    try {
        int num = std::stoi(desired_params_.at(key));
        if (num < low || num > high)
            check_result = false;
    }catch(...) {
        std::cerr << "Input Parma: " << key << " is illegal." << std::endl;
        check_result = false;
    }
    return check_result; 
}

bool Config::checkIPV4(std::string_view ip) {
    // 4个数字、3个'.' 且数字满足 0～255
    int num_count = 0;
    int p = 0;
    auto dot_counter = 0;
    std::array<std::string, 4> ip_nums;

    bool check_result = true;
    while (p < ip.size()) {
        if (ip[p] == '.') {
            ++dot_counter;
        }else {
            try {
                ip_nums.at(dot_counter) += ip[p];
            }catch(std::out_of_range &e) {
                // dot_counter > 3 arrive here:
                check_result = false;
                break;
            }
        }
        ++p;
    }

    if (check_result) {
        for (auto &ip_num : ip_nums) {
            try {
                auto cur_num = std::stoi(ip_num);
                if (cur_num < 0 || cur_num > 255) {
                    check_result = false;
                    break;
                }
            }catch(...) {
                check_result = false;
                break;
            }
        }
    }
    if (!check_result)
        std::cerr << "Input MySQL Server ip address is illegal." << std::endl;

    return check_result;
}



