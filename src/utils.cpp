#include "utils.hpp"

void print_matrix(Matrix m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << m.m[i][j] << " ";
		}
		cout << "\n";
	}
}

std::string gen_key_128()
{
	return "1234567890123456";
}

void print_hex(const std::string &s)
{
	for (unsigned char c : s)
	{
		std::cout << std::hex << std::setw(2) << std::setfill('0')
				  << static_cast<int>(c) << " ";
	}
	std::cout << std::dec << "\n";
}

std::string pkcs7_pad(const std::string &s)
{
	size_t pad = 16 - (s.size() % 16);
	if (pad == 0)
		pad = 16;
	std::string out = s;
	out.append(pad, static_cast<char>(pad));
	return out;
}

std::string pkcs7_unpad(const std::string &s)
{
	if (s.empty() || s.size() % 16)
		throw std::runtime_error("bad padding");
	uint8_t pad = static_cast<uint8_t>(s.back());
	if (pad == 0 || pad > 16)
		throw std::runtime_error("bad padding");
	for (size_t i = 0; i < pad; ++i)
		if ((uint8_t)s[s.size() - 1 - i] != pad)
			throw std::runtime_error("bad padding");
	return s.substr(0, s.size() - pad);
}

void str_to_matrix(const std::string &str, Matrix &m)
{
	if (str.size() != 16)
		throw std::runtime_error("block must be 16 bytes");
	for (int j = 0; j < 4; ++j)
	{
		for (int i = 0; i < 4; ++i)
			m.m[i][j] = static_cast<uint8_t>(str[i + 4 * j]); // column-major}
	}
}

std::string matrix_to_str(Matrix &m)
{
	std::string str;
	str.reserve(16);
	for (int j = 0; j < 4; ++j)
		for (int i = 0; i < 4; ++i)
			str.push_back(static_cast<char>(m.m[i][j]));
	return str;
}