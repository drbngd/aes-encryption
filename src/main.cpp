#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "aes.h"

int main() {
    std::string plaintext;
    std::array<std::string, 2> keys;

    // retrieve plaintext from ../data/plaintext.txt
    std::ifstream plaintext_file("../data/plaintext.txt");
    if (!plaintext_file) {
        throw std::runtime_error("Could not open file: \"../data/plaintext.txt\"");
    } else {
        std::getline(plaintext_file, plaintext); /* reads the first line */
    }
    plaintext_file.close();

    // retrieve keys from ../data/keys.txt
    std::ifstream keys_file("../data/subkey_example.txt");
    if (!keys_file) {
        throw std::runtime_error("Could not open file: \"../data/keys.txt\"");
    } else {
        std::string line;
        for (int i = 0; i < 2; i++) {
            std::getline(keys_file, line);
            keys[i] = line;
        }
    }
    keys_file.close();

    // create an AES object with the plaintext and keys
    AES project1(plaintext, keys);
    AES::DisplayStateMatrix();

    // call the encrypt method with the number of rounds
    AES::Encrypt(1);
    // display the state matrix
//    AES::DisplayStateMatrix();

    return 0;
}

