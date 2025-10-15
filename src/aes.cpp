#include "encryptor.hpp"
#include "decryptor.hpp"

#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "[ERROR] Incorrect arguments.\nUsage:\t./aes -e <file>\n\t./aes -d <file>\n";
		return 1;
	}

	std::string mode = argv[1];
	if (mode != "-e" && mode != "-d")
	{
		cout << "[ERROR] Incorrect mode.\nUsage:\t./aes -e <file>\n\t./aes -d <file>\n";
		return 1;
	}

	std::string aes_key = manage_key("AES_KEY");

	std::string file_name = argv[2];
	std::ifstream file(file_name);
	if (!file)
	{
		cout << "[ERROR]: Could not open file!\n";
		return 1;
	}

	file.seekg(0, std::ios::end);
	size_t length = file.tellg();
	file.seekg(0, std::ios::beg);

	char buffer[length];
	file.read(buffer, length);
	std::string f(buffer, length);

	cout << "[INFO]: File read\n";

	auto expanded = AES128KeyExpansion(aes_key);

	if (mode == "-e")
	{
		std::string encrypted = encrypt(f, expanded);

		std::string ofile_name = file_name + ".enc";
		std::ofstream outfile(ofile_name);
		if (!outfile)
		{
			cout << "[ERROR]: Could not open output file!\n";
			return 1;
		}
		outfile << encrypted;
		outfile.close();

		cout << "[INFO]: File encrypted to " << ofile_name << "\n";
	}
	if (mode == "-d")
	{
		std::string decrypted = decrypt(f, expanded);
		std::string ofile_name = file_name.substr(0, file_name.length() - 4);
		std::ofstream outfile(ofile_name);
		if (!outfile)
		{
			cout << "[ERROR]: Could not open output file!\n";
			return 1;
		}
		outfile << decrypted;
		outfile.close();

		cout << "[INFO]: File decrypted to " << ofile_name << "\n";
	}

	return 0;
}