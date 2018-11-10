#ifndef _LOGGER_HH
#define _LOGGER_HH

#include <iostream>

#define LOG_TRACE std::cout << '\n'
#define LOG_DEBUG std::cout << '\n'
#define LOG_INFO std::cout << '\n'
#define LOG_WARN std::cout << '\n'
#define LOG_ERROR std::cout << '\n'
#define LOG_FATAL abort(); std::cout << '\n'
#define LOG_SYSERR std::cout << '\n'
#define LOG_SYSFATAL abort(); std::cout << '\n'

#endif