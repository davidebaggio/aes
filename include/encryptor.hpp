#ifndef ENCRYPTOR_HPP
#define ENCRYPTOR_HPP

#include <string>
#include <vector>

#include "utils.hpp"
#include "pipeline.hpp"
#include "constants.hpp"

std::string encrypt(std::string plain_text, std::vector<std::string> expanded);

#endif // ENCRYPTOR_HPP