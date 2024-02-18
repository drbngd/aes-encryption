#include <sstream>
#include <iomanip>
#include "aes.h"

std::string AES::plaintext;
std::array<std::string, 2> AES::keys;
std::array<std::array<uint8_t, 4>, 4> AES::state_matrix;
std::array<std::array<uint8_t, 4>, 4> AES::key_matrix;

// can change size to 11 (1+10) when implementing the full AES-128
AES::AES(const std::string& input_text, const std::array<std::string, 2>& keys_list) {
    // TODO: Implement constructor logic here
    AES::plaintext = input_text;
    AES::keys = keys_list;
    state_matrix = StringToAsciiMatrix(plaintext);
    key_matrix = IntStringToMatrix(keys[0]);

}

void AES::Encrypt(const uint8_t num_rounds) {
//    state_matrix = StringToAsciiMatrix(plaintext);
//    key_matrix = IntStringToMatrix(keys[0]);
    AddKey(0);
    for (int i = 1; i <= num_rounds; i++) {
        std::cout << "==== ROUND " << i << " ====" <<std::endl;
        SubBytes();
        ShiftRows();
        MixColumns();
        AddKey(i);
    }
}

void AES::DisplayStateMatrix() {
    std::cout << "State Matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << IntToHex(state_matrix[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}

void AES::DisplayKeyMatrix() {
    std::cout << "Key Matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << IntToHex(key_matrix[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}

//void AES::GenerateSubkeys() {
//    // TODO: Implement GenerateSubkeys logic here
//    // for this project it will get the keys from the list
//    // probably later on we will generate all the 10  keys
//}

void AES::SubBytes() {
    std::cout << "==== SubBytes ====" << std::endl;
    // logic to sub bytes from the SBox
    std::array<std::array<uint8_t, 4>, 4> result{};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = static_cast<uint8_t>(SBox[state_matrix[i][j]]);
        }
    }
    state_matrix = result;

    DisplayStateMatrix();
}

void AES::ShiftRows() {
    std::cout << "==== ShiftRows ====" << std::endl;
    // skip the first [0th] row to save computation power
    for (int i = 1; i < 4; i++) {
        std::array<uint8_t, 4> temp_row = state_matrix[i];
        for (int j = 0; j < 4; j++) {
            state_matrix[i][j] = temp_row[(i + j) % 4]; /* shift by offset = row number */
        }
    }


    DisplayStateMatrix();
}

void AES::MixColumns() {
    std::cout << "==== MixColumns ====" << std::endl;
    state_matrix = MatrixMultiply(mix_column_matrix, state_matrix);

    DisplayStateMatrix();
}

void AES::AddKey(const uint8_t num_round) {
    std::cout << "==== AddKey ====" << std::endl;
    // update key matrix with the next key
    key_matrix = IntStringToMatrix(keys[num_round]);

    // XOR the state matrix with the key matrix
    state_matrix = XOR(state_matrix, key_matrix);


    DisplayStateMatrix();
    //DisplayKeyMatrix();
}

std::string AES::get_subkeys(const uint8_t num_rounds) {
    return keys[num_rounds];
}

std::array<std::array<uint8_t, 4>, 4> AES::get_state_matrix() {
    return state_matrix;
}

std::array<std::array<uint8_t, 4>, 4> AES::get_key_matrix() {
    return key_matrix;
}

void AES::set_state_matrix(const std::array<std::array<uint8_t, 4>, 4>& matrix) {
    state_matrix = matrix;
}

std::array<std::array<uint8_t, 4>, 4> AES::XOR(const std::array<std::array<uint8_t, 4>, 4> &a, const std::array<std::array<uint8_t, 4>, 4> &b) {
    // implement logic to XOR two 4x4 hex arrays and return a new 4x4 array
    std::array<std::array<uint8_t, 4>, 4> result{};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = a[i][j] ^ b[i][j];
        }
    }
    return result;
}

uint8_t AES::GF28Multiply(uint8_t &a, uint8_t &b) {
    // implement logic to multiply two 8-bit numbers in GF(2^8)
    uint8_t result = 0x00;
    for (int i = 0; i < 8; i++) {
        if (b & 0x01) { result ^= a; }
        bool hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set) { a ^= 0x1B; }
        b >>= 1;
    }
    return result;
}

std::array<std::array<uint8_t, 4>, 4> AES::MatrixMultiply(const std::array<std::array<uint8_t, 4>, 4> &a, const std::array<std::array<uint8_t, 4>, 4> &b) {
    // logic to multiply two 4x4 hex arrays
    std::cout << "==== MatrixMultiply ====" << std::endl;
    std::array<std::array<uint8_t, 4>, 4> a_copy = a;
    std::array <std::array<uint8_t, 4>, 4> b_copy = b;
    std::array<std::array<uint8_t, 4>, 4> result{};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = 0x00;
            for (int k = 0; k < 4; k++) {
                result[i][j] ^= GF28Multiply(a_copy[i][k], b_copy[k][j]);
                std::cout << IntToHex(result[i][j]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    return result;
}

std::array<std::array<uint8_t, 4>, 4> AES::StringToAsciiMatrix(const std::string& str) {
    // check if the input string is exactly 16 characters long
    if (str.length() != 16) {
        throw std::invalid_argument("Input string must have exactly 16 characters.");
    }
    // split into an array of 4x4 ascii values
    std::array<std::array<uint8_t, 4>, 4> matrix{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[j][i] = static_cast<uint8_t>(str[i * 4 + j]); // column major form
        }
        std::cout << std::endl;
    }
    return matrix;
}

std::array<std::array<uint8_t , 4>, 4> AES::IntStringToMatrix(const std::string& str) {
    // check if the input string is exactly 32 characters long
    if (str.length() != 32) {
        throw std::invalid_argument("Input string must have exactly 32 characters.");
    }
    // takes in a 128bit hex string and returns a 4x4 uint8_t matrix
    std::array<std::array<uint8_t, 4>, 4> matrix{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[j][i] = static_cast<uint8_t>(std::stoi(str.substr(i * 8 + j * 2, 2), nullptr, 16)); // column major form
        }
    }
    return matrix;
}


std::string AES::IntToHex(const uint8_t& num) {
    std::stringstream stream;
    stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(num);
    return stream.str();
}
