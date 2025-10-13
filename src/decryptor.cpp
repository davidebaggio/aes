#include "decryptor.hpp"

std::string decrypt(std::string encoded, std::vector<std::string> expanded)
{
	std::string decoded = "";
	size_t blocks = encoded.size() / 16;
	for (size_t k = 0; k < blocks; k++)
	{
		Matrix state, key;
		str_to_matrix(encoded.substr(k * 16, 16), state);
		str_to_matrix(expanded[10], key);
		matrix_xor(state, key);
		size_t i = 9;
		for (i; i > 0; i--)
		{
			matrix_inv_shift_rows(state);
			matrix_inv_sub_bytes(state);
			str_to_matrix(expanded[i], key);
			matrix_xor(state, key);
			matrix_inv_mix_columns(state);
		}

		matrix_inv_shift_rows(state);
		matrix_inv_sub_bytes(state);
		str_to_matrix(expanded[0], key);
		matrix_xor(state, key);

		decoded += matrix_to_str(state);
	}

	decoded = pkcs7_unpad(decoded);
	return decoded;
}