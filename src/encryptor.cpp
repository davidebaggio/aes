#include "encryptor.hpp"

std::string encrypt(std::string plain_text, std::vector<std::string> expanded)
{
	std::string encoded = "";
	std::string data = pkcs7_pad(plain_text);
	size_t blocks = data.size() / 16;

	for (size_t k = 0; k < blocks; k++)
	{
		Matrix state, key;
		str_to_matrix(data.substr(k * 16, 16), state);
		str_to_matrix(expanded[0], key);
		matrix_xor(state, key);
		size_t i = 1;
		for (i; i < round_count; i++)
		{
			matrix_sub_bytes(state);
			matrix_shift_rows(state);
			matrix_mix_columns(state);
			str_to_matrix(expanded[i], key);
			matrix_xor(state, key);
		}

		matrix_sub_bytes(state);
		matrix_shift_rows(state);
		str_to_matrix(expanded[i], key);
		matrix_xor(state, key);

		encoded += matrix_to_str(state);
	}

	return encoded;
}