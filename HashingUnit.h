#ifndef HashingUnit_H
#define HashingUnit_H

#include <string>
#include <openssl/evp.h>
#include <openssl/sha.h>

class HashingUnit {
public:
     std::string sha256(const std::string& data);
};

#endif // HashingUnit_H

