#pragma once

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

namespace rdh {
    namespace log {

        namespace attrs = boost::log::attributes;
        namespace expr = boost::log::expressions;
        namespace logging = boost::log;

        void InitLogger(boost::log::trivial::severity_level t_LogLevel) {
            logging::add_console_log(
                std::cout,
                logging::keywords::format = "[%TimeStamp%] [%Severity%] %Message%"
            );

            logging::core::get()->set_filter
            (
                logging::trivial::severity >= t_LogLevel
            );

            logging::add_common_attributes();
        }
    }
}