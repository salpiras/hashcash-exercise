#include "IHasher.h"

#ifndef HASHCASH_MD5HASHER_H
#define HASHCASH_MD5HASHER_H

class MD5Hasher : public IHasher {

public:
    std::vector<unsigned char> calculateHash(const std::vector<unsigned char> &vec) const override;
};


#endif //HASHCASH_MD5HASHER_H
