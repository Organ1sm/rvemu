#pragma once

#include <cstdio>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <iostream>
#include <string_view>


constexpr bool ENABLE_DEBUG_PANIC         = true;
constexpr std::string_view levelStrings[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

constexpr fmt::color colors[] = {
    fmt::color::sky_blue,
    fmt::color::green,
    fmt::color::yellow,
    fmt::color::red,
};


enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
};

inline void vlog(LogLevel level, fmt::format_args args, fmt::string_view format = "{}\n")
{
    fmt::vprint(stdout, fg(colors[level]), format, args);
}

template <typename T>
concept Printable = requires(std::ostream & os, T s)
{
    os << s;
};

template <Printable... Args>
void log(LogLevel level, const char *fName, const char *file, int line, Args &&...args)
{
    fmt::print(fmt::emphasis::bold | fg(colors[level]), "[{}] ", levelStrings[level]);
    fmt::print(fg(colors[level]), "In function {} ({}:{}): ", fName, file, line);

    vlog(level, fmt::make_format_args(args...));

    if (level == ERROR && ENABLE_DEBUG_PANIC)
    {
        fmt::print(std::cerr, "Error occurred . Aborting program{}.\n", "sda");
        std::abort();
    }
}

#define LOG(level, ...) log(level, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);
#define LOG_DEBUG(msg)  vlog(DEBUG, fmt::make_format_args(msg));
