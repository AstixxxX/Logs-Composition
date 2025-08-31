#include <iostream>
#include "Logger.h"
#include "Logs_Composition.h"

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        auto res = Logger::string_to_severity_level(argv[2]);

        if (res.has_value())
            Logs_Composition app(argv[1], res.value());
        else
            std::cerr << "Incorrect severity level of log [" << argv[2] << "]" << std::endl;
    }
    else
    {
        std::cerr << "Usage: logs_composition [Log file path] [Log severity level]" << std::endl;
        std::cerr << "Where log severity level is INFO, WARN, CRIT + the same in lowercase" << std::endl;
    }

    return 0;
}