#ifndef AES_256_CBC_HPP
#define AES_256_CBC_HPP
#include <string_view>
#include <string>
#include <openssl/aes.h>

// template <typename T>
// concept Uint8Limit = std::is_same_v<uint8_t, std::iterator_traits<T>::value_type> ||
//                      std::is_same_v<int8_t, std::iterator_traits<T>::value_type>;

template <typename InputIterator, typename OutputIterator>
void aes_256_cbc_encrypt(std::string_view password, InputIterator first, InputIterator last, OutputIterator result)
{
    using namespace std;

    //static_assert(std::iterator_traits<InputIterator>::value_type == uint8_t, "");

    AES_KEY aes_key;
    uint8_t iv[AES_BLOCK_SIZE] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

    int ret = AES_set_encrypt_key((const uint8_t *)password.data(), 256, &aes_key);
    if (ret)
        __throw_runtime_error("AES_set_encrypt_key error.");

    while (first != last)
    {
        uint8_t input[AES_BLOCK_SIZE] = {0};
        uint8_t out[AES_BLOCK_SIZE] = {0};

        for (auto i = 0; i < AES_BLOCK_SIZE && first != last; i++)
            input[i] = *first++;

        AES_cbc_encrypt(input, out, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);

        copy(begin(out), end(out), result);
    }
}

template <typename InputIterator, typename OutputIterator>
void aes_256_cbc_decrypt(std::string_view password, InputIterator first, InputIterator last, OutputIterator result)
{
    using namespace std;

    AES_KEY aes_key;
    uint8_t iv[AES_BLOCK_SIZE] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

    int ret = AES_set_decrypt_key((const uint8_t *)password.data(), 256, &aes_key);
    if (ret)
        __throw_runtime_error("AES_set_encrypt_key error.");

    while (first != last)
    {
        uint8_t input[AES_BLOCK_SIZE] = {0};
        uint8_t out[AES_BLOCK_SIZE] = {0};

        for (auto i = 0; i < AES_BLOCK_SIZE && first != last; i++)
            input[i] = *first++;

        AES_cbc_encrypt(input, out, AES_BLOCK_SIZE, &aes_key, iv, AES_DECRYPT);

        copy(begin(out), end(out), result);
    }
}

//using ssl_aes_ptr = std::shared_ptr<Aes>;

#endif