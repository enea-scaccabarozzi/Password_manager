#pragma once
#ifndef CRYPTO_LIB_FLAG
#define CRYPTO_LIB_FLAG
#include <cryptopp/rijndael.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <iostream>

std::string SHA_256(const char* str);

std::string AES(const char* str, const char* key);
std::string AES(const char* str);


#endif //CRYPTO_LIB_FLAG