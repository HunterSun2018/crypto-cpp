#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <iterator>
#include <openssl/aes.h>
// #include <openssl/rsa.h>
// #include <openssl/pem.h>
// #include <openssl/err.h>
#include "ssl_aes.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	//cout << "Hello world!" << endl;

	string password = "123456";
	string test = "Hello world!";
	vector<uint8_t> output;

	aes_256_cbc_encrypt(password, begin(test), end(test), back_inserter(output));

	string result;

	aes_256_cbc_decrypt(password, begin(output), end(output), back_inserter(result));
	
	cout << result.size() << ", " << strlen(result.c_str()) << endl;
	result.resize(strlen(result.c_str())+1);

	cout << result << endl;

	using ofs_iterator = ostreambuf_iterator<char>;
	using ifs_iterator = istreambuf_iterator<char>;

	ofstream ofs("test", ios::binary);
	aes_256_cbc_encrypt(password, begin(test), end(test), ofs_iterator(ofs));
	ofs.close();

	ifstream ifs("test", ios::binary);
	ofstream ofs1("result", ios::binary);
	aes_256_cbc_decrypt(password, ifs_iterator(ifs), ifs_iterator(), ostreambuf_iterator<char>(ofs1));

	return 0;
}

// class AesImp : public Aes
// {
// public:
// 	AesImp(std::string_view password, size_t bits)
// 	{
// 		int ret = AES_set_encrypt_key((const unsigned char *)password.data(), bits, &_aes_key);
// 		if (ret)
// 			__throw_runtime_error("AES_set_encrypt_key error.");
// 	}

// 	virtual void encrypt(input_iterator input_beg, input_iterator input_end, output_iterator out_beg) override
// 	{
// 		auto length = distance(input_beg, input_end) / AES_BLOCK_SIZE;
// 		uint8_t iv[AES_BLOCK_SIZE] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

// 		while (input_beg < input_end)
// 		{
// 			uint8_t input[AES_BLOCK_SIZE] = {0};
// 			uint8_t out[AES_BLOCK_SIZE] = {0};

// 			for (auto i = 0; i < AES_BLOCK_SIZE && input_beg < input_end; i++)
// 				input[i] = *input_beg;

// 			AES_cbc_encrypt(input, out, AES_BLOCK_SIZE, &_aes_key, iv, AES_ENCRYPT);
// 		}
// 	}

// private:
// 	AES_KEY _aes_key;
// };

// std::shared_ptr<Aes> Aes::create(std::string_view password, size_t bits)
// {
// 	return make_shared<AesImp>(password, bits);
// }

std::string aes_256_cbc_encode(const std::string &password, const std::string &data)
{
	// 这里默认将iv全置为字符0
	unsigned char iv[AES_BLOCK_SIZE] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

	AES_KEY aes_key;
	if (AES_set_encrypt_key((const unsigned char *)password.c_str(), 256, &aes_key) < 0)
	{
		//assert(false);
		return "";
	}

	std::string strRet;
	std::string data_bak = data;
	unsigned int data_length = data_bak.length();

	// ZeroPadding
	int padding = 0;
	if (data_bak.length() % (AES_BLOCK_SIZE) > 0)
	{
		padding = AES_BLOCK_SIZE - data_bak.length() % (AES_BLOCK_SIZE);
	}
	// 在一些软件实现中，即使是16的倍数也进行了16长度的补齐
	/*else
	{
		padding = AES_BLOCK_SIZE;
	}*/

	data_length += padding;
	while (padding > 0)
	{
		data_bak += '\0';
		padding--;
	}

	for (unsigned int i = 0; i < data_length / (AES_BLOCK_SIZE); i++)
	{
		std::string str16 = data_bak.substr(i * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		::memset(out, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt((const unsigned char *)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);
		strRet += std::string((const char *)out, AES_BLOCK_SIZE);
	}
	return strRet;
}

std::string aes_256_cbc_decode(const std::string &password, const std::string &strData)
{
	// 这里默认将iv全置为字符0
	unsigned char iv[AES_BLOCK_SIZE] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

	AES_KEY aes_key;
	if (AES_set_decrypt_key((const unsigned char *)password.c_str(), password.length() * 8, &aes_key) < 0)
	{
		//assert(false);
		return "";
	}
	std::string strRet;
	for (unsigned int i = 0; i < strData.length() / AES_BLOCK_SIZE; i++)
	{
		std::string str16 = strData.substr(i * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		::memset(out, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt((const unsigned char *)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_DECRYPT);
		strRet += std::string((const char *)out, AES_BLOCK_SIZE);
	}
	return strRet;
}