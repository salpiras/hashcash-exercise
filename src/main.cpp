#include "hashcash.h"
#include "MD5Hasher.h"
#include <iostream>
#include <thread>
#include <future>

#define ZERO_BITS 8

inline void printConflict(const std::string& currentHash, const std::vector<unsigned char>& input) {
    std::cout << "Hash conflict was found with: ";
    for (const auto& elem : input) {
        std::cout << (int) elem << " ";
    }
    std::cout << " for string " << currentHash;
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {

    // init random number generator
    std::random_device rd;
    std::mt19937 randomGen(rd());

    // init hashing algo, just has to be an IHasher
    MD5Hasher md5hasher;

    // check number of cores to improve parallelisation
    auto numCores = std::thread::hardware_concurrency();

    using FutureConflict = std::future<std::vector<unsigned char>>;
    std::vector<FutureConflict> conflicts;

    for (auto i = 1; i < argc; ++i)  // compute for every hash
    {
        auto conflict = std::async(std::launch::async, [&, i] {
            std::string s(argv[i]);
            return hashcash::findConflict(s, md5hasher, ZERO_BITS, randomGen, numCores);
        });
        conflicts.emplace_back(std::move(conflict));
    }

    auto currentIndex = 1; //print conflicts from first arg hash
    for (auto& elem: conflicts) {
        printConflict(argv[currentIndex], elem.get());
        currentIndex++;
    }

    return 0;
}