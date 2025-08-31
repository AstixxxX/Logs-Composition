#include "Logger.h"

IBaseLogger::IBaseLogger(severity_level def_sev_lvl) : def_sev_lvl(def_sev_lvl) {}

std::string IBaseLogger::severity_level_to_string(severity_level sev_lvl)
{
    switch (sev_lvl)
    {
        case severity_level::INFORMATION : return "INFO";
        case severity_level::WARNING     : return "WARN";
        case severity_level::CRITICAL    : return "CRIT";
        default : return "UNKN";
    }
}

std::optional<severity_level> IBaseLogger::string_to_severity_level(const std::string& str_sev_lvl)
{
    if (str_sev_lvl == "INFO" || str_sev_lvl == "info")
        return severity_level::INFORMATION;
    else if (str_sev_lvl == "WARN" || str_sev_lvl == "warn")
        return severity_level::WARNING;
    else if (str_sev_lvl == "CRIT" || str_sev_lvl == "crit")
        return severity_level::CRITICAL;
    else 
        return std::nullopt;         
}

void IBaseLogger::set_default_severity_level(severity_level new_def_sev_lvl)
{
    std::lock_guard<std::mutex> lock(mtx);
    def_sev_lvl = new_def_sev_lvl;
}

bool IBaseLogger::write_log(severity_level sev_lvl, const std::string& message) const
{
    std::lock_guard<std::mutex> lock(mtx);

    if (sev_lvl < def_sev_lvl)
        return false;
             
    std::string log_entity = "[" + get_log_timestamp() + "] [" + severity_level_to_string(sev_lvl) + "] " + message + "\n";
    
    if (fd > 0)
    {
        ssize_t n = write(fd, log_entity.c_str(), log_entity.size());

        if (n == -1)
            return false;

        return n == log_entity.size();
    }
    else
        return false;
}

IBaseLogger::~IBaseLogger() 
{ 
    if (fd > -1)
        close(fd); 
}

std::string IBaseLogger::get_log_timestamp() const        
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;

    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

Logger::Logger(std::string log_path, severity_level def_sev_lvl) : IBaseLogger(def_sev_lvl)
{
    fd = open(log_path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644); 

    if (fd < 0)
        throw std::runtime_error("Failed to open log file: " + log_path);
}

NetworkLogger::NetworkLogger(std::string log_ip, uint16_t log_port, severity_level def_sev_lvl) : IBaseLogger(def_sev_lvl)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0)
        throw std::runtime_error("Socket creation failed");
    
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(log_port);
    
    if (inet_pton(AF_INET, log_ip.c_str(), &serv_addr.sin_addr) < 0) 
        throw std::invalid_argument("Invalid IP address");
    
    if (connect(fd, (sockaddr*)& serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("Coudn\'t connect to the log server");
}