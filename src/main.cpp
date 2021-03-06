#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <iterator>
#include "ssl_aes.hpp"
#include "aes.hpp"

using namespace std;
void test_aes();

int main(int argc, char *argv[])
{
	cout << "2 ^ 12 = " << (1 << 12) << endl;

	string password = "123456";
	string test = "Hello world!";
	vector<uint8_t> output;

	aes_256_cbc_encrypt(password, begin(test), end(test), back_inserter(output));

	string result;

	aes_256_cbc_decrypt(password, begin(output), end(output), back_inserter(result));

	cout << result.size() << ", " << strlen(result.c_str()) << endl;
	result.resize(strlen(result.c_str()) + 1);

	// cout << result << endl;

	using ofs_iterator = ostreambuf_iterator<char>;
	using ifs_iterator = istreambuf_iterator<char>;

	ifstream ftest("test", ios::binary);
	ofstream ofs1("encrypted", ios::binary);
	aes_256_cbc_encrypt(password, ifs_iterator(ftest), ifs_iterator(), ofs_iterator(ofs1));
	ftest.close();
	ofs1.close();

	ifstream ifs("encrypted", ios::binary);
	ofstream ofs2("result", ios::binary);
	aes_256_cbc_decrypt(password, ifs_iterator(ifs), ifs_iterator(), ofs_iterator(ofs2));

	test_aes();

	return 0;
}

void test_aes()
{
	string input("hello world");	
	vector<uint8_t> output;

	ssl_aes::create("123456", 256, ssl_aes::ENCRYPT)
		->encode(begin(input), end(input), back_inserter(output));
}