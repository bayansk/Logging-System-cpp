
#pragma once

#include <string>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/asio.hpp> // For networking
#include <boost/beast.hpp>
#include <boost/timer/timer.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <cstdlib> 
#include <chrono>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>


class Logger {
public:



    static Logger* GetInstance(const std::string& logDirectory = "C:/Logs", const std::string& graylogHost = "127.0.0.1");


    void log(const std::string& message, boost::log::trivial::severity_level level, bool user_consent = false);
    void logToGraylog(const std::string& message, boost::log::trivial::severity_level level);
    std::string getUserId();

private:

    Logger(const std::string& log_directory, const std::string& graylog_host);
    ~Logger();


    std::string graylog_host;
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> logger;
    boost::asio::io_context io_context;
    std::string UUID;
    static Logger* instance;
    



};
