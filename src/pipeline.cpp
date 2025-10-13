#include "pipeline.hpp"

uint32_t RotWord(uint32_t word)
{
	return (word << 8) | (word >> 24);
}

// SubWord: apply S-box to each byte of a word
uint32_t SubWord(uint32_t word)
{
	return (sbox[(word >> 24) & 0xFF] << 24) |
		   (sbox[(word >> 16) & 0xFF] << 16) |
		   (sbox[(word >> 8) & 0xFF] << 8) |
		   (sbox[word & 0xFF]);
}

void matrix_xor(Matrix &state, Matrix &key)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			state.m[i][j] = state.m[i][j] ^ key.m[i][j];
		}
	}
}

void matrix_sub_bytes(Matrix &state)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			state.m[i][j] = sbox[state.m[i][j]];
		}
	}
}

void matrix_inv_sub_bytes(Matrix &state)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			state.m[i][j] = inv_sbox[state.m[i][j]];
		}
	}
}

void matrix_shift_rows(Matrix &state)
{
	for (int i = 1; i < 4; i++)
	{
		for (int j = 0; j < i; j++)
		{
			char temp = state.m[i][0];
			for (int k = 0; k < 3; k++)
			{
				state.m[i][k] = state.m[i][k + 1];
			}
			state.m[i][3] = temp;
		}
	}
}

void matrix_inv_shift_rows(Matrix &state)
{
	for (int i = 1; i < 4; i++)
	{
		for (int j = 0; j < i; j++)
		{
			char temp = state.m[i][3];
			for (int k = 3; k > 0; k--)
			{
				state.m[i][k] = state.m[i][k - 1];
			}
			state.m[i][0] = temp;
		}
	}
}

void matrix_mix_columns(Matrix &s)
{
	for (int c = 0; c < 4; ++c)
	{
		uint8_t a0 = s.m[0][c], a1 = s.m[1][c], a2 = s.m[2][c], a3 = s.m[3][c];
		s.m[0][c] = gf_mul(a0, 0x02) ^ gf_mul(a1, 0x03) ^ a2 ^ a3;
		s.m[1][c] = a0 ^ gf_mul(a1, 0x02) ^ gf_mul(a2, 0x03) ^ a3;
		s.m[2][c] = a0 ^ a1 ^ gf_mul(a2, 0x02) ^ gf_mul(a3, 0x03);
		s.m[3][c] = gf_mul(a0, 0x03) ^ a1 ^ a2 ^ gf_mul(a3, 0x02);
	}
}

void matrix_inv_mix_columns(Matrix &s)
{
	for (int c = 0; c < 4; ++c)
	{
		uint8_t a0 = s.m[0][c], a1 = s.m[1][c], a2 = s.m[2][c], a3 = s.m[3][c];
		s.m[0][c] = gf_mul(a0, 0x0e) ^ gf_mul(a1, 0x0b) ^ gf_mul(a2, 0x0d) ^ gf_mul(a3, 0x09);
		s.m[1][c] = gf_mul(a0, 0x09) ^ gf_mul(a1, 0x0e) ^ gf_mul(a2, 0x0b) ^ gf_mul(a3, 0x0d);
		s.m[2][c] = gf_mul(a0, 0x0d) ^ gf_mul(a1, 0x09) ^ gf_mul(a2, 0x0e) ^ gf_mul(a3, 0x0b);
		s.m[3][c] = gf_mul(a0, 0x0b) ^ gf_mul(a1, 0x0d) ^ gf_mul(a2, 0x09) ^ gf_mul(a3, 0x0e);
	}
}

std::vector<std::string> AES128KeyExpansion(const std::string &key)
{
	if (key.size() != 16)
	{
		throw std::runtime_error("AES-128 requires 16-byte key");
	}

	const int Nk = 4;  // words in key
	const int Nb = 4;  // words in block
	const int Nr = 10; // rounds

	std::vector<uint32_t> w(Nb * (Nr + 1));
	std::vector<std::string> roundKeys;

	// copy initial key
	for (int i = 0; i < Nk; i++)
	{
		w[i] = (static_cast<uint8_t>(key[4 * i]) << 24) |
			   (static_cast<uint8_t>(key[4 * i + 1]) << 16) |
			   (static_cast<uint8_t>(key[4 * i + 2]) << 8) |
			   (static_cast<uint8_t>(key[4 * i + 3]));
	}

	// key expansion
	for (int i = Nk; i < Nb * (Nr + 1); i++)
	{
		uint32_t temp = w[i - 1];
		if (i % Nk == 0)
		{
			temp = SubWord(RotWord(temp)) ^ Rcon[(i / Nk) - 1];
		}
		w[i] = w[i - Nk] ^ temp;
	}

	// collect round keys (16 bytes each)
	for (int round = 0; round <= Nr; round++)
	{
		std::string rk(16, '\0');
		for (int col = 0; col < 4; col++)
		{
			uint32_t word = w[round * Nb + col];
			rk[4 * col] = static_cast<char>((word >> 24) & 0xFF);
			rk[4 * col + 1] = static_cast<char>((word >> 16) & 0xFF);
			rk[4 * col + 2] = static_cast<char>((word >> 8) & 0xFF);
			rk[4 * col + 3] = static_cast<char>(word & 0xFF);
		}
		roundKeys.push_back(rk);
	}

	return roundKeys;
}