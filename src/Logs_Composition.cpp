#include "Logs_Composition.h"

RandomGenerator::RandomGenerator() : urandom("/dev/urandom", std::ios::binary) 
{
    if (!urandom.is_open()) 
        throw std::runtime_error("Cannot open /dev/urandom generator file");
}
    
unsigned int RandomGenerator::random_num() 
{
    unsigned int seed;

    if (!urandom.is_open())
        std::cerr << "/dev/urandom generator file unavaliable" << std::endl;
    else
        urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));

    return seed;
}

std::string RandomGenerator::random_ipv4()
{
    return std::to_string(random_num() % 256) + "." +
        std::to_string(random_num() % 256) + "." +
        std::to_string(random_num() % 256) + "." +
        std::to_string(random_num() % 256);
};

Logs_Composition::Logs_Composition(const std::string& log_path, severity_level def_sev_lvl) : log_path(log_path), def_sev_lvl(def_sev_lvl)
{
    Logger logger(log_path, def_sev_lvl);

    std::thread producer([this, &logger] { Composer(&logger); });
    std::thread consumer([this, &logger] { LogWriter(&logger); });

    producer.join();
    consumer.join();
}

LogEntity Logs_Composition::GenerateLog(std::string&& str_sev_lvl)
{
    LogEntity log_entity;

    if (!str_sev_lvl.empty())
    {
        auto severity_result = Logger::string_to_severity_level(str_sev_lvl);

        if (!severity_result.has_value())
            log_entity.sev_lvl = def_sev_lvl;
        else
            log_entity.sev_lvl = severity_result.value();
    }
    else
    {
        str_sev_lvl = log_levels[randgen.random_num() % log_levels.size()];
        log_entity.sev_lvl = Logger::string_to_severity_level(str_sev_lvl).value();
    }

    log_entity.message = message_generator[str_sev_lvl][randgen.random_num() % message_generator[str_sev_lvl].size()]();
    return log_entity;
}

void Logs_Composition::Composer(Logger* logger)
{
    std::system("clear");

    LogEntity log_entity;
    std::string str_sev_lvl;
    std::string input;

    while (true)
    {
        bool valid_input = false;
        str_sev_lvl = Logger::severity_level_to_string(def_sev_lvl);

        std::cout << "       MANUAL WRITING IN THE LOG FILE" << std::endl << std::endl;

        std::cout << "                  SETTINGS:" << std::endl;
        std::cout << "All severity levels of log: [";

        for (const auto& lvl : log_levels)
            std::cout << lvl << ", ";

        std::cout << "\b\b]" << std::endl;

        std::cout << "Default severity level of log: [" << str_sev_lvl << "]" << std::endl;
        std::cout << "Log file: " << log_path << std::endl << std::endl;

        std::cout << "                LOG EXAMPLES:" << std::endl;
        
        for (auto&& lvl = std::find(log_levels.begin(), log_levels.end(), str_sev_lvl); lvl != log_levels.end(); ++lvl)
        {
            log_entity = GenerateLog(std::move(*lvl));
            std::cout << "[" + Logger::severity_level_to_string(log_entity.sev_lvl) + "] " + log_entity.message << std::endl;
        }

        log_entity = GenerateLog(std::move(str_sev_lvl));
        std::cout << log_entity.message << std::endl << std::endl;

        std::cout << "                   USAGE:" << std::endl;
        std::cout << "[LEVEL] MESSAGE -> Regular log. If severity level will be less" << std::endl;
        std::cout << "                -> than [" + str_sev_lvl + "], log don\'t write in the log file!" << std::endl;
        std::cout << "MESSAGE         -> Log which will be marked as [" + str_sev_lvl << "] " << std::endl;
        std::cout << "[LEVEL]         -> Change default severity level of log for LEVEL" << std::endl;
        std::cout << "exit | quit     -> Exit" << std::endl << std::endl << "> ";

        std::getline(std::cin, input);

        if (input.size() == 0)
            std::cout << "Incorrect input. Empty log" << std::endl;
        else if (input[0] == '[')
        {
            auto it = input.find(']');

            if (it != std::string::npos)
            {
                str_sev_lvl = input.substr(1, it - 1);  
                auto severity_result = Logger::string_to_severity_level(str_sev_lvl);

                if (!severity_result.has_value())
                    std::cout << "Incorrect input. Undefined sevenity level" << std::endl;
                else
                    log_entity.sev_lvl = severity_result.value();

                if (input.size() > it + 2)
                {
                    log_entity.message = input.substr(it + 2);
                    valid_input = true;
                }
                else if (severity_result.has_value())
                {
                    std::lock_guard<std::mutex> lock(mtx); 
                    def_sev_lvl = log_entity.sev_lvl;
                    logger->set_default_severity_level(log_entity.sev_lvl);
                    std::cout << "Default severity level was changed successfully" << std::endl;
                }
            }
            else
                std::cout << "Format violation. Close square bracket \']\' don't find" << std::endl;
        }
        else if (input != "exit" && input != "quit")
        {
            log_entity.message = input;
            log_entity.sev_lvl = def_sev_lvl;
            valid_input = true;
        }
        else
        {
            app_running = false;
            cv.notify_all();
            break;
        }

        if (valid_input)
        {
            if (log_entity.sev_lvl >= def_sev_lvl)
                std::cout << "Log will be added soon!" << std::endl;
            else
                std::cout << "Log won\'t added to log file due to low severity level [" << Logger::severity_level_to_string(log_entity.sev_lvl) << "] < [" << Logger::severity_level_to_string(def_sev_lvl) << "]" << std::endl;

            AddTask(log_entity);
        }

        std::system("sleep 1 && clear");
    }
}

void Logs_Composition::AddTask(const LogEntity& log_entity)
{
    std::lock_guard<std::mutex> lock(mtx);
    taskQueue.push(std::move(log_entity));
    cv.notify_one();
}

void Logs_Composition::LogWriter(Logger* logger)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !taskQueue.empty() || !app_running; });

        if (!app_running && taskQueue.empty()) 
            break;

        logger->write_log(taskQueue.front().sev_lvl, taskQueue.front().message);
        taskQueue.pop();
    }
}