#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

class Config {
friend class HttpServer;
public:
    static Config &getInstance();    
    Config(const Config &other) = delete; 
    Config &operator=(const Config &other) = delete; 
    ~Config();

    bool parseParms(int argc, const char *argv[]);
    
private:
    Config();

    void initSupportedParams();
    void initDesiredParams();
    void printUsage();
    void printParams();  // debug

    bool checkPortNum();
    bool checkMySQLIP();
    bool checkMySQLConectionCount();
    bool checkThreadCount();

    bool checkNumRange(const std::string &key, int low, int high);
    bool checkIPV4(std::string_view ip);

    std::string usage_str_;
    std::unordered_map<std::string, std::string> supported_params_;
    std::unordered_map<std::string, std::string> desired_params_;
};