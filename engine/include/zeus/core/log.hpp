/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2021 Tristan F.
 *
 * The Zeus Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * The Zeus Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the Zeus Game Engine. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>
#include <string_view>

/**
 * @file log.hpp
 */

namespace Zeus {

/**
 * A simple logging system.
 */
class Log {
   public:
    /**
     * Log levels.
     *
     * @note The order of the levels is in order of importance.
     */
    enum class Level {
        Error = 4,
        Warning = 3,
        Info = 2,
        Config = 1,
        Debug = 0
    };

    /**
     * Logs the given message using the given logger name and given log level.
     *
     * @param name      The name of the logger
     * @param message   The message to log
     * @param level     The log level of the message
     */
    static void msg(std::string_view name, std::string_view message,
                    Log::Level level);

    /**
     * Logs the given message at the given log level using the default logger
     * name.
     *
     * @param message   The message to log
     * @param level     The log level of the message
     */
    static void msg(std::string_view message, Log::Level level);

    /**
     * Logs the given message using the given logger name at the error log
     * level.
     *
     * @param name      The name of the logger
     * @param message   The message to log
     */
    static void error(std::string_view name, std::string_view message);

    /**
     * Logs the given message at the error log level using the default logger
     * name.
     *
     * @param message The message to log
     */
    static void error(std::string_view message);

    /**
     * Logs the given message using the given logger name at the warning log
     * level.
     *
     * @param name      The name of the logger
     * @param message   The message to log
     */
    static void warning(std::string_view name, std::string_view message);

    /**
     * Logs the given message at the warning log level using the default logger
     * name.
     *
     * @param message The message to log
     */
    static void warning(std::string_view message);

    /**
     * Logs the given message using the given logger name at the info log
     * level.
     *
     * @param name      The name of the logger
     * @param message   The message to log
     */
    static void info(std::string_view name, std::string_view message);

    /**
     * Logs the given message at the info log level using the default logger
     * name.
     *
     * @param message The message to log
     */
    static void info(std::string_view message);

    /**
     * Logs the given message using the given logger name at the config log
     * level.
     *
     * @param name      The name of the logger
     * @param message   The message to log
     */
    static void config(std::string_view name, std::string_view message);

    /**
     * Logs the given message at the config log level using the default logger
     * name.
     *
     * @param message The message to log
     */
    static void config(std::string_view message);

    /**
     * Logs the given message using the given logger name at the debug log
     * level.
     *
     * @param name      The name of the logger
     * @param message   The message to log
     */
    static void debug(std::string_view name, std::string_view message);

    /**
     * Logs the given message at the debug log level using the default logger
     * name.
     *
     * @param message The message to log
     */
    static void debug(std::string_view message);
};

/**
 * Returns a string representation of the given log level.
 *
 * @param level The log level to stringify
 *
 * @return A string representation of the given log level
 */
[[nodiscard]] std::string to_string(Log::Level level) noexcept;

}  // namespace Zeus

// Check if logging is turned on
#ifdef ZEUS_ENABLE_LOGGING

#ifndef ZEUS_LOGGING_LEVEL
// Set to lowest log level (accepts all logs)
#define ZEUS_LOGGING_LEVEL 0
#endif

#if ZEUS_LOGGING_LEVEL <= 4
#define ZEUS_ERROR_LOG(...) Zeus::Log::error(__VA_ARGS__)
#else
#define ZEUS_ERROR_LOG(...)
#endif

#if ZEUS_LOGGING_LEVEL <= 3
#define ZEUS_WARNING_LOG(...) Zeus::Log::warning(__VA_ARGS__)
#else
#define ZEUS_WARNING_LOG(...)
#endif

#if ZEUS_LOGGING_LEVEL <= 2
#define ZEUS_INFO_LOG(...) Zeus::Log::info(__VA_ARGS__)
#else
#define ZEUS_INFO_LOG(...)
#endif

#if ZEUS_LOGGING_LEVEL <= 1
#define ZEUS_CONFIG_LOG(...) Zeus::Log::config(__VA_ARGS__)
#else
#define ZEUS_CONFIG_LOG(...)
#endif

#if ZEUS_LOGGING_LEVEL <= 0
#define ZEUS_DEBUG_LOG(...) Zeus::Log::debug(__VA_ARGS__)
#else
#define ZEUS_DEBUG_LOG(...)
#endif

#else
#define ZEUS_ERROR_LOG(...)
#define ZEUS_WARNING_LOG(...)
#define ZEUS_INFO_LOG(...)
#define ZEUS_CONFIG_LOG(...)
#define ZEUS_DEBUG_LOG(...)
#endif
