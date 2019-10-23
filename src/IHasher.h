
#include "vector"

#ifndef HASHCASH_IHASHER_H
#define HASHCASH_IHASHER_H

    class IHasher {
    public:
        virtual std::vector<unsigned char> calculateHash(const std::vector<unsigned char> &vec) const = 0;
        virtual ~IHasher() = default;
    };

#endif //HASHCASH_IHASHER_H
