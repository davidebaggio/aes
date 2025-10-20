# AES-128 Implementation in C++

This project is a simple **AES-128 encryption/decryption implementation** written in C++ for educational purposes.  
It demonstrates how AES works internally (SubBytes, ShiftRows, MixColumns, AddRoundKey, and Key Expansion) without relying on external crypto libraries.

---

## 🔧 Features

- AES-128 key expansion (16-byte key, 10 rounds)
- Encryption & decryption of arbitrary-length plaintext
- PKCS#7 padding for non-16-byte inputs
- Matrix-based operations (state representation in 4×4 bytes)
- Debug output in **hexadecimal** for ciphertext and plaintext
- Web app to process files

---

## 📂 Project Structure

- `eas.cpp` – entry point, encryption/decryption demo
- `server.cpp` – server, encryption/decryption web app
- AES core functions:
  - `SubBytes`, `ShiftRows`, `MixColumns`
  - `AddRoundKey`
  - `KeyExpansion` for round keys
  - PKCS#7 padding/unpadding
- Utility functions to:
  - Read/write blocks as 4×4 matrices

---

## 🛠️ Build

### Linux / macOS

```bash
./compile.sh

# encrypt demo
./build/aes -e <file>

# decrypt demo
./build/aes -d <file>

# server listening at localhost:9090
./build/server
```
