#include <random>
#include "vector"
#include "IHasher.h"

#ifndef HASHCASH_HASHCASH_H
#define HASHCASH_HASHCASH_H

namespace hashcash {
    std::vector<unsigned char> findConflict(const std::string& hashInput, const IHasher& algorithm,
                                            const int leadingZeroes, std::mt19937& randomGen,
                                            const int cores = 1);
}

#endif //HASHCASH_HASHCASH_H
