#include "Logger.h"
#include "HashingUnit.h"
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <fstream>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/make_shared.hpp>
#include <iostream>


Logger* Logger::instance = nullptr;



Logger::Logger(const std::string& log_directory, const std::string& graylog_host)
    : graylog_host(graylog_host) {

    boost::filesystem::path log_dir(log_directory);
    if (!boost::filesystem::exists(log_dir)) {
        boost::filesystem::create_directories(log_dir);
    }
 
    UUID = getUserId();


    boost::log::add_file_log(
        boost::log::keywords::file_name = log_directory + "/%Y-%m-%d-" + UUID + ".log",
        boost::log::keywords::open_mode = std::ios_base::app,  // Append to the existing log file
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), 
        boost::log::keywords::max_files = 7,  // Keep only the last 7 days of logs //todo check
        boost::log::keywords::auto_flush = true,
        boost::log::keywords::format = (
            boost::log::expressions::stream
            << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
            << " [" << boost::log::trivial::severity << "] "
            << boost::log::expressions::smessage
            )
    );

    boost::log::add_common_attributes(); // Add common attributes like timestamp
}

Logger::~Logger() {
}


Logger* Logger::GetInstance(const std::string& logDirectory, const std::string& graylogHost) {
    if (instance == nullptr) {
        instance = new Logger(logDirectory, graylogHost);
    }
    return instance;
}





void Logger::log(const std::string& message, boost::log::trivial::severity_level level, bool user_consent) {
    auto start_time = std::chrono::high_resolution_clock::now();

    BOOST_LOG_SEV(boost::log::trivial::logger::get(), level) << message;

    if (level == boost::log::trivial::severity_level::error && user_consent == true) {
        logToGraylog(message, level);
    }

    std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Logging took: " << duration << " msec" << std::endl;
}


void Logger::logToGraylog(const std::string& message, boost::log::trivial::severity_level level) {
    try {
        using namespace boost::beast;
        using namespace boost::asio;
        using namespace boost::asio::ssl;

        std::string target = "/gelf";
        unsigned int version = 11;

        const std::string auth_token = "abc";
        HashingUnit hasher;
        std::string hashedToken = hasher.sha256(auth_token);
       /* std::ifstream token_file("C:\\Users\\BayanSamak\\source\\repos\\LoggerTest1 - Copy\\token.txt");
        if (!token_file.is_open()) {
            throw std::runtime_error("Unable to open token file");
        }

        std::string auth_token;
        std::getline(token_file, auth_token);
        token_file.close();

        if (auth_token.empty()) {
            throw std::runtime_error("Token file is empty");
        }*/

       

        boost::asio::io_context io_context;
        ssl::context ssl_ctx(ssl::context::sslv23_client);

        //ssl_ctx.set_verify_mode(verify_none);

        ssl_ctx.set_default_verify_paths(); // Use system's CA certificates

        //ssl_ctx.load_verify_file("C:\\Users\\BayanSamak\\certs");
        ip::tcp::resolver resolver(io_context);
        auto const results = resolver.resolve(graylog_host, "443");

        ssl::stream<ip::tcp::socket> ssl_socket(io_context, ssl_ctx);

        boost::asio::connect(ssl_socket.next_layer(), results.begin(), results.end());

        ssl_socket.handshake(ssl::stream_base::client);

        http::request<http::string_body> req(http::verb::post, target, version);
        req.set(http::field::host, graylog_host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        //req.set(http::field::authorization, std::string(auth_token));
        
        req.body() = R"({"version": "1.1", "host": ")" + graylog_host + R"(", "level": ")"
            + std::to_string(static_cast<int>(level)) + R"(", "short_message": ")"
            + message + R"(", "uuid": ")" + UUID + R"(", "auth_token": ")" + std::string(hashedToken) + R"("})";


        req.prepare_payload();

        // Send the HTTPS request
        http::write(ssl_socket, req);

        boost::system::error_code ec;
        ssl_socket.shutdown(ec);
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}



//TODO
//user ID,name...etc
std::string Logger::getUserId() {

    //this si a static user id, it generates once and then store it in a file and then keep reading the id from the file
    //ofc, it is possible to generate new id each run..
    //const std::string UserIdentifierFile = "user_id.txt";
    //std::ifstream file(UserIdentifierFile);
    std::string userId;

    //if (file) {
    //    std::getline(file, userId); // Read existing user ID from file
    //}
    //else {
        // Generate a new user ID using _dupenv_s
        char* username_env = nullptr;
       // char* machine_env = nullptr;
        std::string username = "unknown_user";
     //   std::string machineName = "unknown_machine";

        // Get USERNAME
        if (_dupenv_s(&username_env, nullptr, "USERNAME") == 0 && username_env) {
            username = username_env;
            free(username_env);
        }

        // Get COMPUTERNAME
        //if (_dupenv_s(&machine_env, nullptr, "COMPUTERNAME") == 0 && machine_env) {
        //    machineName = machine_env;
        //    free(machine_env); // Free the allocated memory
        //}

        //boost::uuids::random_generator generator;
        //std::string uuid = to_string(generator()); // Generate UUID

        //userId = username + "-" + machineName + "-" + uuid;

        userId = username ;



        // Save user ID to file
        /*std::ofstream outFile(UserIdentifierFile);*/
        //outFile << userId;
   /* }*/

    return userId;
}





















