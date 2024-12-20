// Copyright (c) 2014-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_SHA256_H
#define BITCOIN_CRYPTO_SHA256_H

#include <stdint.h>
#include <stdlib.h>
#include "../../../test.hpp"

namespace baseline {

class CSHA256
{
private:
    uint32_t s[8];
    unsigned char buf[64];
    uint64_t bytes;

public:
    using digest_t = system::hash_digest;
    static const size_t digest_bytes = 32;

    CSHA256();
    CSHA256& Write(const unsigned char* data, size_t len);
    void Finalize(unsigned char hash[digest_bytes]);
    CSHA256& Reset();
};

void SHA256D64(unsigned char* output, const unsigned char* input, size_t blocks);

} // namespace baseline

#endif // BITCOIN_CRYPTO_SHA256_H
