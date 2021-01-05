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

std::string AES_enc(const char* str, const char* in_key, byte iv[CryptoPP::AES::BLOCKSIZE]){
    CryptoPP::AutoSeededRandomPool prng;
    std::string sKey(in_key);

    if(CryptoPP::AES::DEFAULT_KEYLENGTH < sKey.size())
    {
        sKey = sKey.substr(0, CryptoPP::AES::DEFAULT_KEYLENGTH);
    } 
    else if(CryptoPP::AES::DEFAULT_KEYLENGTH > sKey.size())
    {
        sKey += std::string(CryptoPP::AES::DEFAULT_KEYLENGTH - sKey.size(), '*');
    }
    CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
    memcpy(key, sKey.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);

    prng.GenerateBlock( iv, sizeof(iv));

    std::string plain(str);
    std::string cipher;
    std::string encoded;
    CryptoPP::CFB_Mode< CryptoPP::AES >::Encryption enc;
    enc.SetKeyWithIV( key, key.size(), iv );
    

    CryptoPP::StringSource ss1(plain , true, new CryptoPP::StreamTransformationFilter(enc, new CryptoPP::StringSink(cipher )));
    CryptoPP::StringSource ss2(cipher, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(encoded )));

    return encoded;
}

std::string AES_dec(const char* str, const char* in_key, byte iv[CryptoPP::AES::BLOCKSIZE]){
    std::string cipher;
    CryptoPP::StringSource ss1(str, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(cipher)));

    std::string sKey(in_key);

    if(CryptoPP::AES::DEFAULT_KEYLENGTH < sKey.size())
    {
        sKey = sKey.substr(0, CryptoPP::AES::DEFAULT_KEYLENGTH);
    } 
    else if(CryptoPP::AES::DEFAULT_KEYLENGTH > sKey.size())
    {
        sKey += std::string(CryptoPP::AES::DEFAULT_KEYLENGTH - sKey.size(), '*');
    }
    CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
    memcpy(key, sKey.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);

    CryptoPP::CFB_Mode< CryptoPP::AES >::Decryption dec;
    dec.SetKeyWithIV( key, key.size(), iv );

    std::string recovered;
    
    CryptoPP::StringSource ss3(cipher, true, new CryptoPP::StreamTransformationFilter(dec, new CryptoPP::StringSink(recovered )));

    
    return recovered;
}
std::string random_str(const int len) {
    
    std::string ran_str;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    srand((unsigned(time(NULL))));
    ran_str.reserve(len);
    for (int i = 0; i < len; ++i){
        ran_str += alphanum[rand() % (sizeof(alphanum) - 1)];
    }     
    return ran_str;
}
std::string AES_enc(const char* str, byte iv[CryptoPP::AES::BLOCKSIZE]){
    std::string key = random_str(CryptoPP::AES::DEFAULT_KEYLENGTH);
    std::string encoded (AES_enc(str, key.c_str(), iv));
    encoded += key;
    return encoded;
}

std::string AES_dec(const char* str, byte iv[CryptoPP::AES::BLOCKSIZE]){
    std::string stringa(str);
    int len = stringa.length() - CryptoPP::AES::DEFAULT_KEYLENGTH;
    std::string encoded(stringa.substr(0, len));
    std::string key(stringa.substr(len, 16));
    std::string recovered(AES_dec(encoded.c_str(), key.c_str(), iv));
    return recovered;
}