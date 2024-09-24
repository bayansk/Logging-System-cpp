
#include <iostream>
#include "Logger.h"

//int main() {
//    try {
//        
//        Logger logger("C:\\Users\\BayanSamak\\source\\repos\\LoggerTest1", "127.0.0.1", 12201);
//
//       // Logger logger("C:\\Users\\BayanSamak\\source\\repos\\LoggerTest1\\logs.txt", "127.0.0.1", 12201, 1024 * 1024, 5); // 1 MB max file size and keep 5 rotated files
//
//        // Log msgs
//       /* logger.log("log from c++");
//        logger.log("CIAO!!.");*/
//
//        // Log messages with different severity levels
//
//        logger.log("This is an info log message", boost::log::trivial::info);
//        logger.log("This is a warning log message", boost::log::trivial::warning);
//        logger.log("This is an error log message", boost::log::trivial::error);
//        
//
//        //logger.log("This is a warning log message", boost::log::trivial::warning);
//        //logger.log("This is an error log message", boost::log::trivial::error);
//        //logger.log("Ciao");
//
//        std::cerr << "donneeeee: " << std::endl;
//
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//
//    return 0;
//}



//int main() {
//
//
//   /* Logger* logger = Logger::GetInstance("C:/Logs", "127.0.0.1");
//
//    logger->log("This is a log message from C++", boost::log::trivial::info, true);*/
//
//}

int main() {
    Logger* logger = Logger::GetInstance("C:/Logs", "127.0.0.1");
    logger->log("Test log from C++", boost::log::trivial::info);

    logger->log("Test error log from C++", boost::log::trivial::error,true);

    return 0;
}



