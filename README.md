# C++ Logger

A simple powerful C++ logger with file and network output support, plus a test log generator. Inspired by Syslog

## Features

- **Multithreading**: Thread-safe log writing
- **Multiple log levels**: INFORMATION, WARNING, CRITICAL
- **Multiple outputs**: File logger and network logger
- **Test log generator**: Automatic generation of realistic logs


## Components

- **Logger** : File logger that writes messages to a file with thread-safe operations.
- **NetworkLogger** : Network logger that sends messages over the network.
- **Logs_Composition** : Test log generator with automatic generation of realistic log messages for testing purposes .

## Log Levels

- **INFORMATION**: Informational messages (logins, API requests, cache)
- **WARNING**: Warnings (slow responses, high memory usage)
- **CRITICAL**: Critical errors (security breaches, system failures)

## Building

### Requirements
- C++17 compiler
- CMake 3.12+
- Linux/Unix system

### Building the Project

```bash
# Clone repository
git clone <repository-url>

# Create build directory
mkdir build
cd build

# Configuration (choose one option)
cmake .. -DBUILD_SHARED_LIBS=ON    # Shared library
cmake .. -DBUILD_STATIC_LIBS=ON    # Static library
cmake .. -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON  # Both versions

# Build
make 
