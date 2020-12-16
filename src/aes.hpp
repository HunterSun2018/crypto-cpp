#ifndef AES_HPP
#define AES_HPP

#include <string>
#include <string_view>
#include <array>
#include <memory>

class ssl_aes
{
public:
    enum crypt_type
    {
        ENCRYPT,
        DECRYPT,
    };

    static std::shared_ptr<ssl_aes>
    create(std::string_view password, size_t bits, crypt_type type);

    static const size_t BUFFER_SIZE = 16;
    using aes_buffer = std::array<uint8_t, BUFFER_SIZE>;

    template <typename InputIterator, typename OutputIterator>
    void encode(InputIterator first, InputIterator last, OutputIterator result)
    {
        prepare();

        while (first != last)
        {
            aes_buffer input = {0};
            aes_buffer out = {0};

            for (auto i = 0; i < BUFFER_SIZE && first != last; i++)
                input[i] = *first++;

            encrypt(input, out);

            copy(begin(out), end(out), result);
        }
    }

private:
    virtual void prepare() = 0;

    virtual void encrypt(const aes_buffer &input, aes_buffer &out) = 0;
};

using ssl_aes_ptr = std::shared_ptr<ssl_aes>;

#endif