#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "constants.hpp"

using namespace std;

uint32_t RotWord(uint32_t word);
uint32_t SubWord(uint32_t word);

void matrix_xor(Matrix &state, Matrix &key);

void matrix_sub_bytes(Matrix &state);
void matrix_inv_sub_bytes(Matrix &state);

void matrix_shift_rows(Matrix &state);
void matrix_inv_shift_rows(Matrix &state);

inline uint8_t xtime(uint8_t x) { return (uint8_t)((x << 1) ^ ((x & 0x80) ? 0x1B : 0x00)); }
inline uint8_t gf_mul(uint8_t x, uint8_t y)
{
	uint8_t r = 0;
	while (y)
	{
		if (y & 1)
			r ^= x;
		x = xtime(x);
		y >>= 1;
	}
	return r;
}
void matrix_mix_columns(Matrix &s);
void matrix_inv_mix_columns(Matrix &s);

std::vector<std::string> AES128KeyExpansion(const std::string &key);

#endif // PIPELINE_HPP