#include "Crypto_lib.h"

std::string SHA_256(const char* str)
{
    byte digest[ CryptoPP::SHA256::DIGESTSIZE ];
    std::string message = str;

    CryptoPP::SHA256 hash;
    hash.CalculateDigest( digest, (const byte*)message.c_str(), message.length() );

    CryptoPP::HexEncoder encoder;
    std::string output;

    encoder.Attach( new CryptoPP::StringSink( output ) );
    encoder.Put( digest, sizeof(digest) );
    encoder.MessageEnd();
    return output;
}