#pragma once
#include <iostream>
#include <string>
#include <mutex>
#include <optional>
#include <chrono>
#include <iomanip>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

enum class severity_level { INFORMATION, WARNING, CRITICAL }; 

class IBaseLogger
{
public:
    IBaseLogger(severity_level def_sev_lvl);
    static std::string severity_level_to_string(severity_level sev_lvl);
    static std::optional<severity_level> string_to_severity_level(const std::string& str_sev_lvl);
    void set_default_severity_level(severity_level new_def_sev_lvl);
    bool write_log(severity_level sev_lvl, const std::string& message) const;
    ~IBaseLogger();

protected:
    std::string get_log_timestamp() const;
    severity_level def_sev_lvl;
    mutable std::mutex mtx;
    int fd = -1;
};

struct Logger : IBaseLogger
{
    Logger(std::string log_path, severity_level def_sev_lvl);
};

struct NetworkLogger : IBaseLogger
{
    NetworkLogger(std::string log_ip, uint16_t log_port, severity_level def_sev_lvl);
};