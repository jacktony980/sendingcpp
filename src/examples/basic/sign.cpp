
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <secp256k1.h>
#include <cryptopp/sha3.h>
#include <sstream>

class sign {

public:
    std::string sign_message_with_private_key(const std::string& private_key_hex, const std::string& message) {
        // Hash the message using SHA-3 (Keccak-256)
        CryptoPP::SHA3_256 sha3;
        std::vector<unsigned char> hash(sha3.DigestSize());
        sha3.Update(reinterpret_cast<const unsigned char*>(message.c_str()), message.length());
        sha3.Final(hash.data());

        // Convert the private key from hex to binary
        std::vector<unsigned char> private_key_bytes;
        for (size_t i = 0; i < private_key_hex.length(); i += 2) {
            std::string byteString = private_key_hex.substr(i, 2);
            unsigned char byte = (unsigned char) strtol(byteString.c_str(), nullptr, 16);
            private_key_bytes.push_back(byte);
        }

        // Create a secp256k1 context object
        secp256k1_context* context = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

        // Create a secp256k1 secret key object from the private key bytes
        secp256k1_ecdsa_signature signature;
        secp256k1_ecdsa_sign(context, &signature, hash.data(), private_key_bytes.data(), nullptr, nullptr);

        // Serialize the signature
        unsigned char serialized_signature[64];
        size_t signature_length = 64;
        secp256k1_ecdsa_signature_serialize_compact(context, serialized_signature, &signature);

        // Convert the serialized signature to hex
        std::ostringstream signature_hex;
        for (size_t i = 0; i < signature_length; ++i) {
            signature_hex << std::hex << std::setw(2) << std::setfill('0') << (int)serialized_signature[i];
        }

        // Destroy the secp256k1 context object
        secp256k1_context_destroy(context);

        // Return the signature
        return signature_hex.str();
    }
};