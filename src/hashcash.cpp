#include <string>
#include <vector>
#include "hashcash.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <future>

using Nonce = std::vector<unsigned char>;
using NoncePool = std::vector<Nonce>;
using HashNoncePair = std::pair<std::vector<unsigned char>, std::vector<unsigned char>>;
using FutureCharVector = std::vector<std::future<HashNoncePair>>;

inline std::vector<unsigned char>
combineValues(const std::vector<unsigned char>& first, const std::vector<unsigned char>& second);

inline std::vector<unsigned char> strToBytes(const std::string& hashInput);

inline NoncePool generateRandomNoncePool(std::mt19937& randGen, const unsigned long nonceSize, const int poolSize);

inline std::vector<unsigned char> generateRandomNonce(std::mt19937& randGen, unsigned long size);

inline unsigned char generateRandomNonceEntry(std::mt19937& randGen);

inline void incrementNonce(std::vector<unsigned char>& currentNonce, std::mt19937& randGen);

inline void incrementNoncePool(NoncePool& currentNonce, std::mt19937& randGen);

inline std::string calculateBitRepresentation(const std::vector<unsigned char>& input);

inline bool bitRepresentationHasLeadingZeroes(const std::vector<unsigned char>& input, const int leadingZeroes);

inline std::string bytesToHex(std::vector<unsigned char>& bytes);

inline bool hasLeadingZeroes(const int zeroes, const std::string& bitRepresentation);

Nonce hashcash::findConflict(const std::string& hashInput, const IHasher& hasher,
                             const int leadingZeroes, std::mt19937& randomGen, const int cores) {

    // convert into bytes
    auto bytesValue = strToBytes(hashInput);

    auto poolSize = cores + 1; // threads should be number of cpus+1 ideally

    // create initial list of nonces to feed to workers
    auto currentRandNonces = generateRandomNoncePool(randomGen, hashInput.size(), poolSize);

    FutureCharVector combinedHashes;

    while (true) {

        for (const auto& nonceElem : currentRandNonces) {
            auto combinedHashNoncePair = std::async(std::launch::async, [=, &hasher]() -> HashNoncePair {
                auto combined = combineValues(bytesValue, nonceElem);
                return HashNoncePair(hasher.calculateHash(combined), nonceElem);
            });
            combinedHashes.emplace_back(std::move(combinedHashNoncePair));
        }

        for (auto& elem : combinedHashes) {
            auto hashResult = elem.get();
            if (bitRepresentationHasLeadingZeroes(hashResult.first, leadingZeroes)) {
                return hashResult.second;
            }
        }

        // none of the current workers had the solution, retry with different ones
        combinedHashes.clear();
        incrementNoncePool(currentRandNonces, randomGen);
    }
}


std::vector<unsigned char>
combineValues(const std::vector<unsigned char>& first, const std::vector<unsigned char>& second) {
    static std::vector<unsigned char> scratch(first.size());
    for (auto i = 0u; i < scratch.size(); ++i) {
        scratch[i] = first[i] | second[i];
    }
    return scratch;
}

std::vector<unsigned char> strToBytes(const std::string& hashInput) {
    return std::vector<unsigned char>(hashInput.begin(), hashInput.end());
}

std::string bytesToHex(std::vector<unsigned char>& bytes) {
    char buf[33];
    for (int i = 0; i < 16; i++)
        sprintf(buf + i * 2, "%02x", bytes[i]);
    buf[32] = 0;

    return std::string(buf);
}

NoncePool generateRandomNoncePool(std::mt19937& randGen, const unsigned long nonceSize, const int poolSize) {
    NoncePool pool;
    for (int i = 0; i < poolSize; i++) {
        pool.emplace_back(generateRandomNonce(randGen, nonceSize));
    }
    return pool;
}

std::vector<unsigned char> generateRandomNonce(std::mt19937& randomGen, const unsigned long size) {
    auto nonceSeed = std::vector<unsigned char>(size);
    for (auto& elem : nonceSeed) {
        elem = generateRandomNonceEntry(randomGen);
    }
    return nonceSeed;
}

unsigned char generateRandomNonceEntry(std::mt19937& randomGen) {
    std::uniform_int_distribution<int> distribution(0, 256);
    return (unsigned char) distribution(randomGen);
}

void incrementNonce(std::vector<unsigned char>& currentNonce, std::mt19937& randGen) {
    auto size = std::distance(currentNonce.begin(), currentNonce.end());
    std::uniform_int_distribution<long> distribution(0, (size - 1ll));
    auto indexToIncrement = (unsigned long) distribution(randGen);
    auto it = currentNonce.begin() + indexToIncrement;
    (*it)++;
}

void incrementNoncePool(NoncePool& currentPool, std::mt19937& randGen) {
    for (auto& elem : currentPool) {
        incrementNonce(elem, randGen);
    }
}

std::string calculateBitRepresentation(const std::vector<unsigned char>& input) {
    std::ostringstream oss;
    for (auto& elem : input) {
        for (auto bit = 1u; bit <= 256; bit <<= 1) {
            oss << ((elem & bit) == bit);
        }
    }
    return oss.str();
}

bool hasLeadingZeroes(const int zeroes, const std::string& bitRepresentation) {
    return bitRepresentation.find_first_not_of('0') > zeroes;
}


bool bitRepresentationHasLeadingZeroes(const std::vector<unsigned char>& input, const int leadingZeroes) {
    auto counter = 0;
    for (auto& elem : input) {
        for (auto bit = 1u; bit <= 256; bit <<= 1) {
            if ((elem & bit) != bit) {
                counter++;
                if (counter >= leadingZeroes) {
                    return true;
                }
            } else {
                return leadingZeroes == 0;
            }
        }
    }
    return leadingZeroes == 0;
}



