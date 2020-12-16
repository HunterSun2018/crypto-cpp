#include <string_view>
#include <string>
#include <array>
#include <random>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include "aes.hpp"

class ssl_aes_imp : public ssl_aes
{
private:
    /* data */
public:
    ssl_aes_imp(/* args */);
    ~ssl_aes_imp();

    virtual void prepare() override
    {

    }
    
    virtual void encrypt(const aes_buffer &input, aes_buffer &out) override
    {
    }
};

ssl_aes_imp::ssl_aes_imp(/* args */)
{
}

ssl_aes_imp::~ssl_aes_imp()
{
}

ssl_aes_ptr ssl_aes::create(std::string_view password, size_t bits, crypt_type type)
{
    return std::make_shared<ssl_aes_imp>();
}