#pragma once
#include <iostream>
#include <fstream>
#include "Logger.h"
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <map>
#include <functional>
#include <algorithm>

namespace 
{
    class RandomGenerator
    {
    private:
        std::ifstream urandom;
        
    public:
        RandomGenerator();
        unsigned int random_num();
        std::string random_ipv4();
    };

    struct LogEntity
    {
        severity_level sev_lvl;
        std::string message;
    };
}

class Logs_Composition
{
public:
    Logs_Composition(const std::string& log_path, severity_level def_sev_lvl);
    
private:
    LogEntity GenerateLog(std::string&& str_sev_lvl = "");
    void Composer(Logger* logger);
    void AddTask(const LogEntity& log_entity);
    void LogWriter(Logger* logger);

    std::atomic<bool> app_running = true;
    std::queue<LogEntity> taskQueue;
    severity_level def_sev_lvl;
    std::condition_variable cv;
    std::mutex mtx;

    std::string log_path;
    RandomGenerator randgen;  
    std::vector<std::string> log_levels = {"INFO", "WARN", "CRIT"};
    std::map<std::string, std::vector<std::function<std::string()>>> message_generator = 
    {
        {"INFO", {
            [this]() { return "User logged in successfully from " + randgen.random_ipv4(); },
            [this]() { return "Cache updated for key: " + std::to_string(randgen.random_num()); },
            [this]() { return "API request processed from " + randgen.random_ipv4(); },
            [this]() { return "Session started from IP: " + randgen.random_ipv4(); },
            [this]() { return "Database connection pool initialized with " + std::to_string(5 + randgen.random_num() % 46) + " connections"; },
            [this]() { return "External API call from " + randgen.random_ipv4() + " successful"; },
            [this]() { return "Batch processing completed: " + std::to_string(100 + randgen.random_num() % 10001) + " records"; },
            [this]() { return "Memory cache flushed: " + std::to_string(100 + randgen.random_num() % 5001) + " items removed"; },
            [this]() { return "User registration completed from " + randgen.random_ipv4(); },
            [this]() { return "Backup completed successfully: " + std::to_string(randgen.random_num() % 100 + 1) + "GB processed"; },
            [this]() { return "SSL handshake successful with " + randgen.random_ipv4(); },
            [this]() { return "Load balancer added new node: " + randgen.random_ipv4(); },
            [this]() { return "CDN cache hit rate: " + std::to_string(85 + randgen.random_num() % 11) + "%"; },
            [this]() { return "Database index optimized: " + std::to_string(randgen.random_num() % 1000) + "ms improvement"; },
            [this]() { return "Message queue processed " + std::to_string(1000 + randgen.random_num() % 9001) + " messages"; },
            [this]() { return "User profile updated from " + randgen.random_ipv4(); },
            [this]() { return "API version " + std::to_string(1 + randgen.random_num() % 6) + "." + std::to_string(1 + randgen.random_num() % 11) + " deployed successfully"; },
            [this]() { return "Websocket connection established from " + randgen.random_ipv4(); },
            [this]() { return "Image processing completed: " + std::to_string(randgen.random_num() % 500) + "ms"; },
            [this]() { return "Search index updated with " + std::to_string(1000 + randgen.random_num() % 10001) + " new documents"; }
        }},
        {"WARN", {
            [this]() { return "Slow response from " + randgen.random_ipv4() + ": " + std::to_string(1000 + randgen.random_num() % 4001) + "ms"; },
            [this]() { return "Memory usage high: " + std::to_string(80 + randgen.random_num() % 16) + "%"; },
            [this]() { return "Database query took " + std::to_string(2000 + randgen.random_num() % 8001) + "ms"; },
            [this]() { return "SSL certificate expiring in " + std::to_string(1 + randgen.random_num() % 30) + " days"; },
            [this]() { return "Disk space low: " + std::to_string(1 + randgen.random_num() % 15) + "% free"; },
            [this]() { return "Rate limit approaching: " + std::to_string(80 + randgen.random_num() % 16) + "% of quota used"; },
            [this]() { return "Unusual activity detected from IP: " + randgen.random_ipv4(); },
            [this]() { return "Database connection timeout after " + std::to_string(5 + randgen.random_num() % 26) + " seconds"; },
            [this]() { return "User failed login " + std::to_string(5 + randgen.random_num() % 8) + " times from " + randgen.random_ipv4(); },
            [this]() { return "CPU usage spike: " + std::to_string(85 + randgen.random_num() % 15) + "% for " + std::to_string(30 + randgen.random_num() % 271) + " seconds"; },
            [this]() { return "Network latency increased to " + std::to_string(200 + randgen.random_num() % 800) + "ms"; },
            [this]() { return "Cache miss rate elevated: " + std::to_string(25 + randgen.random_num() % 51) + "%"; },
            [this]() { return "Database deadlock detected, transaction rolled back"; },
            [this]() { return "Third-party API rate limited from " + randgen.random_ipv4(); },
            [this]() { return "SSL certificate validation warning for " + randgen.random_ipv4(); },
            [this]() { return "Session timeout: user from " + randgen.random_ipv4() + " inactive for " + std::to_string(30 + randgen.random_num() % 91) + " minutes"; },
            [this]() { return "Disk I/O queue depth: " + std::to_string(10 + randgen.random_num() % 41); },
            [this]() { return "Memory fragmentation: " + std::to_string(15 + randgen.random_num() % 36) + "%"; },
            [this]() { return "DNS resolution slow: " + std::to_string(500 + randgen.random_num() % 1501) + "ms"; },
            [this]() { return "Websocket ping timeout from " + randgen.random_ipv4(); }
        }},
        {"CRIT", {
            [this]() { return "Security breach attempt from IP: " + randgen.random_ipv4(); },
            [this]() { return "Network partition: " + randgen.random_ipv4() + " unreachable"; },
            [this]() { return "Critical security vulnerability detected: CVE-" + std::to_string(2020 + randgen.random_num() % 5) + "-" + std::to_string(1000 + randgen.random_num() % 9000); },
            [this]() { return "Memory leak: " + std::to_string(500 + randgen.random_num() % 1501) + "MB allocated without release"; },
            [this]() { return "System outage detected from " + randgen.random_ipv4(); },
            [this]() { return "Database corruption with error code: " + std::to_string(randgen.random_num()); },
            [this]() { return "Hard disk failure, sector: " + std::to_string(randgen.random_num()); },
            [this]() { return "Authentication failure from IP: " + randgen.random_ipv4(); },
            [this]() { return "Payment system down, transaction ID: " + std::to_string(randgen.random_num()); },
            [this]() { return "System shutdown due to error " + std::to_string(randgen.random_num()); },
            [this]() { return "Data breach detected: " + std::to_string(1000 + randgen.random_num() % 10000) + " records compromised"; },
            [this]() { return "RAID array degraded: disk " + std::to_string(randgen.random_num() % 8) + " failed"; },
            [this]() { return "Firewall compromised from " + randgen.random_ipv4(); },
            [this]() { return "Database replication broken: " + std::to_string(randgen.random_num() % 1000) + " transactions behind"; },
            [this]() { return "SSL private key exposed in logs"; },
            [this]() { return "Zero-day exploit detected from " + randgen.random_ipv4(); },
            [this]() { return "System kernel panic: error code " + std::to_string(randgen.random_num() % 101); },
            [this]() { return "Financial transaction discrepancy: $" + std::to_string(1000 + randgen.random_num() % 100000); },
            [this]() { return "DDoS attack detected from " + std::to_string(1000 + randgen.random_num() % 10000) + " IP addresses"; }
        }}
    };
};