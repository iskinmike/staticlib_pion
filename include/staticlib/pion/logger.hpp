/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ---------------------------------------------------------------------
// pion:  a Boost C++ framework for building lightweight HTTP interfaces
// ---------------------------------------------------------------------
// Copyright (C) 2007-2014 Splunk Inc.  (https://github.com/splunk/pion)
//
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
//

#ifndef STATICLIB_PION_LOGGER_HPP
#define STATICLIB_PION_LOGGER_HPP

#include "staticlib/config.hpp"

#if defined(STATICLIB_PION_USE_LOG4CPLUS)

    // log4cplus headers
    #include "log4cplus/logger.h"
    #include "log4cplus/loggingmacros.h"

    namespace staticlib { 
    namespace pion {
        /**
         * Log4cplus logger, log4cplus must be initialized explicitely before using
         */
        using logger = log4cplus::Logger;
    } // namespace
    }

    #define STATICLIB_PION_LOG_CONFIG_BASIC   log4cplus::BasicConfigurator::doConfigure();
    #define STATICLIB_PION_LOG_CONFIG(FILE)   log4cplus::PropertyConfigurator::doConfigure(FILE);
    #define STATICLIB_PION_GET_LOGGER(NAME)   log4cplus::Logger::getInstance(NAME)
    #define STATICLIB_PION_SHUTDOWN_LOGGER    log4cplus::Logger::shutdown();

    #define STATICLIB_PION_LOG_SETLEVEL_DEBUG(LOG)    LOG.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
    #define STATICLIB_PION_LOG_SETLEVEL_INFO(LOG)     LOG.setLogLevel(log4cplus::INFO_LOG_LEVEL);
    #define STATICLIB_PION_LOG_SETLEVEL_WARN(LOG)     LOG.setLogLevel(log4cplus::WARN_LOG_LEVEL);
    #define STATICLIB_PION_LOG_SETLEVEL_ERROR(LOG)    LOG.setLogLevel(log4cplus::ERROR_LOG_LEVEL);
    #define STATICLIB_PION_LOG_SETLEVEL_FATAL(LOG)    LOG.setLogLevel(log4cplus::FATAL_LOG_LEVEL);
    #define STATICLIB_PION_LOG_SETLEVEL_UP(LOG)       LOG.setLogLevel(LOG.getLogLevel()+1);
    #define STATICLIB_PION_LOG_SETLEVEL_DOWN(LOG)     LOG.setLogLevel(LOG.getLogLevel()-1);

    #define STATICLIB_PION_LOG_DEBUG  LOG4CPLUS_DEBUG
    #define STATICLIB_PION_LOG_INFO   LOG4CPLUS_INFO
    #define STATICLIB_PION_LOG_WARN   LOG4CPLUS_WARN
    #define STATICLIB_PION_LOG_ERROR  LOG4CPLUS_ERROR
    #define STATICLIB_PION_LOG_FATAL  LOG4CPLUS_FATAL


#elif defined(STATICLIB_PION_DISABLE_LOGGING)

    // Logging is disabled -> add do-nothing stubs for logging
    namespace staticlib { 
    namespace pion {
        /**
         * No-op logger implementation
         */
        struct logger {
            /**
             * Constructor
             */
            logger(int /* glog */) { }
            
            /**
             * Overloaded bool operator, always returns false
             * 
             * @return always returns false
             */
            operator bool() const {
                return false;
            }
        };
    } // namespace
    }

    #define STATICLIB_PION_LOG_CONFIG_BASIC   {}
    #define STATICLIB_PION_LOG_CONFIG(FILE)   {}
    #define STATICLIB_PION_GET_LOGGER(NAME)   0
    #define STATICLIB_PION_SHUTDOWN_LOGGER    0

    // use LOG to avoid warnings about LOG not being used
    #define STATICLIB_PION_LOG_SETLEVEL_DEBUG(LOG)    { if (LOG) {} }
    #define STATICLIB_PION_LOG_SETLEVEL_INFO(LOG)     { if (LOG) {} }
    #define STATICLIB_PION_LOG_SETLEVEL_WARN(LOG)     { if (LOG) {} }
    #define STATICLIB_PION_LOG_SETLEVEL_ERROR(LOG)    { if (LOG) {} }
    #define STATICLIB_PION_LOG_SETLEVEL_FATAL(LOG)    { if (LOG) {} }
    #define STATICLIB_PION_LOG_SETLEVEL_UP(LOG)       { if (LOG) {} }
    #define STATICLIB_PION_LOG_SETLEVEL_DOWN(LOG)     { if (LOG) {} }

    #define STATICLIB_PION_LOG_DEBUG(LOG, MSG)    { if (LOG) {} }
    #define STATICLIB_PION_LOG_INFO(LOG, MSG)     { if (LOG) {} }
    #define STATICLIB_PION_LOG_WARN(LOG, MSG)     { if (LOG) {} }
    #define STATICLIB_PION_LOG_ERROR(LOG, MSG)    { if (LOG) {} }
    #define STATICLIB_PION_LOG_FATAL(LOG, MSG)    { if (LOG) {} }
#else

    #define STATICLIB_PION_USE_OSTREAM_LOGGING

    // Logging uses std::cout and std::cerr
    #include <iostream>
    #include <string>
    #include <ctime>

    namespace staticlib { 
    namespace pion {
        /**
         * STDOUT logger implementation
         */
        struct logger {
            
            /**
             * Supported log levels
             */
            enum log_priority_type {
                LOG_LEVEL_DEBUG, 
                LOG_LEVEL_INFO, 
                LOG_LEVEL_WARN,
                LOG_LEVEL_ERROR, 
                LOG_LEVEL_FATAL
            };

            /**
             * Logger name
             */
            std::string m_name;
            
            /**
             * Global priority for all loggers
             */
            log_priority_type& priority() {
                static log_priority_type pr = logger::LOG_LEVEL_INFO;
                return pr;
            } 
            
            /**
             * Destructor
             */
            ~logger() { }
            
            /**
             * Constructor, returns logger with name "pion"
             */
            logger() : m_name("staticlib.pion") { }
            
            /**
             * Constructor, return logger with specified name
             * 
             * @param name logger name
             */
            logger(const std::string& name) : m_name(name) { }
            
            /**
             * Copy constructor
             * 
             * @param p logger instance
             */
            logger(const logger& p) : m_name(p.m_name) { }
        };
    } // namespace
    }

    #define STATICLIB_PION_LOG_CONFIG_BASIC   {}
    #define STATICLIB_PION_LOG_CONFIG(FILE)   {}
    #define STATICLIB_PION_GET_LOGGER(NAME)   sl::pion::logger(NAME)
    #define STATICLIB_PION_SHUTDOWN_LOGGER    {}

    #define STATICLIB_PION_LOG_SETLEVEL_DEBUG(LOG)    { LOG.priority() = sl::pion::logger::LOG_LEVEL_DEBUG; }
    #define STATICLIB_PION_LOG_SETLEVEL_INFO(LOG)     { LOG.priority() = sl::pion::logger::LOG_LEVEL_INFO; }
    #define STATICLIB_PION_LOG_SETLEVEL_WARN(LOG)     { LOG.priority() = sl::pion::pion::logger::LOG_LEVEL_WARN; }
    #define STATICLIB_PION_LOG_SETLEVEL_ERROR(LOG)    { LOG.priority() = sl::pion::logger::LOG_LEVEL_ERROR; }
    #define STATICLIB_PION_LOG_SETLEVEL_FATAL(LOG)    { LOG.priority() = sl::pion::logger::LOG_LEVEL_FATAL; }
    #define STATICLIB_PION_LOG_SETLEVEL_UP(LOG)       { ++LOG.priority(); }
    #define STATICLIB_PION_LOG_SETLEVEL_DOWN(LOG)     { --LOG.priority(); }

    #define STATICLIB_PION_LOG_DEBUG(LOG, MSG)    if (LOG.priority() <= sl::pion::logger::LOG_LEVEL_DEBUG) { std::cout << time(NULL) << " DEBUG " << LOG.m_name << ' ' << MSG << std::endl; }
    #define STATICLIB_PION_LOG_INFO(LOG, MSG)     if (LOG.priority() <= sl::pion::logger::LOG_LEVEL_INFO) { std::cout << time(NULL) << " INFO " << LOG.m_name << ' ' << MSG << std::endl; }
    #define STATICLIB_PION_LOG_WARN(LOG, MSG)     if (LOG.priority() <= sl::pion::logger::LOG_LEVEL_WARN) { std::cerr << time(NULL) << " WARN " << LOG.m_name << ' ' << MSG << std::endl; }
    #define STATICLIB_PION_LOG_ERROR(LOG, MSG)    if (LOG.priority() <= sl::pion::logger::LOG_LEVEL_ERROR) { std::cerr << time(NULL) << " ERROR " << LOG.m_name << ' ' << MSG << std::endl; }
    #define STATICLIB_PION_LOG_FATAL(LOG, MSG)    if (LOG.priority() <= sl::pion::logger::LOG_LEVEL_FATAL) { std::cerr << time(NULL) << " FATAL " << LOG.m_name << ' ' << MSG << std::endl; }

#endif

#endif // STATICLIB_PION_LOGGER_HPP
