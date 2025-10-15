#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include "constants.hpp"

using std::cout;

std::string gen_key_128();
std::string manage_key(const char *name);

void print_matrix(Matrix m);
void print_hex(const std::string &s);

std::string pkcs7_pad(const std::string &s);
std::string pkcs7_unpad(const std::string &s);

void str_to_matrix(const std::string &str, Matrix &m);
std::string matrix_to_str(Matrix &m);

#endif // UTILS_HPP