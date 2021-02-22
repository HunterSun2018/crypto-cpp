#ifndef AES_HPP
#define AES_HPP

#include <string>
#include <string_view>
#include <array>
#include <memory>
#include <type_traits>
#include <iterator>
#include <concepts>

namespace requirement
{
    template <typename InputIterator, typename OutputIterator>
    concept IteratorRequirement = requires(InputIterator input, OutputIterator output)
    {

        requires std::forward_iterator<InputIterator>;
        requires(sizeof(typename std::iterator_traits<InputIterator>::value_type) == 1);

        ++input;

        //std::output_iterator<OutputIterator>;

        std::same_as<std::output_iterator_tag, typename std::iterator_traits<OutputIterator>::iterator_category>;
        //requires(sizeof(typename std::iterator_traits<OutputIterator>::value_type) == 1);

        ++output;
    };
};

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

    //requires requirement::IteratorRequirement<InputIterator, OutputIterator>
    template <typename InputIterator, typename OutputIterator>
        requires std::forward_iterator<InputIterator> &&
        (sizeof(typename std::iterator_traits<InputIterator>::value_type) == 1) &&
        std::output_iterator<OutputIterator, uint8_t> //
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

class ace_factory
{
public:
    ssl_aes create();
};

#endif