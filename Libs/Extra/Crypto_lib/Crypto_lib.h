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

std::string AES_enc(const char* str, const char* in_key, std::string &in_iv);
std::string AES_dec(const char* str, const char* in_key, std::string &in_iv);
std::string AES_enc(const char* str, std::string &in_iv);
std::string AES_dec(const char* str, std::string &in_iv);

std::string random_str(const int len, int seed);


#endif //CRYPTO_LIB_FLAG