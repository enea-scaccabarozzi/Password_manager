#pragma once
#ifndef CRYPTO_LIB_FLAG
#define CRYPTO_LIB_FLAG
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>

std::string SHA_256(const char* str);

const char* AES(const char* str, const char* key);
const char* AES(const char* str);


#endif //CRYPTO_LIB_FLAG