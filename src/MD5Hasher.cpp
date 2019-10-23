#include "MD5Hasher.h"
#include "md5.h"

std::vector<unsigned char> MD5Hasher::calculateHash(const std::vector<unsigned char>& vec) const {
    MD5 md5;
    md5.update(vec.data(), (MD5::size_type) vec.size());
    md5.finalize();
    return md5.hexbytes();
}
