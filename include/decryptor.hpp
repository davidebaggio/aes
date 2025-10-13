#ifndef DECRYPTOR_HPP
#define DECRYPTOR_HPP

#include <string>
#include <vector>

#include "utils.hpp"
#include "pipeline.hpp"
#include "constants.hpp"

std::string decrypt(std::string encoded, std::vector<std::string> expanded);

#endif // DECRYPTOR_HPP