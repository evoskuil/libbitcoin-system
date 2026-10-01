/**
 * Copyright (c) 2011-2026 libbitcoin developers
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/system/crypto/secp256k1/algorithm.hpp>

namespace libbitcoin {
namespace system {
namespace secp256k1 {

class verifier
  : public algorithm
{
public:
    using algorithm::slice_count;
    using algorithm::table_words;
    using ecdsa_t = std_array<scalar_t, two>;
    using schnorr_t = std_array<bytes_t, two>;

    static __device__ uint32_t row() NOEXCEPT
    {
        return __nvvm_read_ptx_sreg_ctaid_x() * __nvvm_read_ptx_sreg_ntid_x() +
            __nvvm_read_ptx_sreg_tid_x();
    }

    static __device__ uint8_t verify(const hash_digest& digest,
        const ec_compressed& key, const ecdsa_t& signature) NOEXCEPT
    {
        affine_t<uint64_t> point{};
        if (!from_bytes(point, key))
            return 0;

        scalar_t z{};
        /* bool */ from_bytes(z, digest);
        return verify_ecdsa(point, z, signature.front(), signature.back()) ?
            1 : 0;
    }

    static __device__ uint8_t verify(const hash_digest& message,
        const ec_xonly& key, const schnorr_t& signature) NOEXCEPT
    {
        const auto challenge = sha256::hash(
            tagged_midstate<"BIP0340/challenge">, signature.front(), key,
            message);

        return verify_schnorr(key, challenge, signature.front(),
            signature.back()) ? 1 : 0;
    }
};

__device__ uint64_t generator_table[verifier::slice_count]
    [verifier::table_words];

__device__ const std_array<const uint64_t*, 16> generator_slices
{
    generator_table[0], generator_table[1], generator_table[2],
    generator_table[3], generator_table[4], generator_table[5],
    generator_table[6], generator_table[7], generator_table[8],
    generator_table[9], generator_table[10], generator_table[11],
    generator_table[12], generator_table[13], generator_table[14],
    generator_table[15]
};

} // namespace secp256k1
} // namespace system
} // namespace libbitcoin

using namespace libbitcoin::system;
using namespace libbitcoin::system::secp256k1;

extern "C" __global__ void verify_ecdsa(
    const hash_digest* digests, const ec_compressed* keys,
    const verifier::ecdsa_t* signatures, uint8_t* results, uint32_t count)
{
    const auto row = verifier::row();
    if (row < count)
        results[row] = verifier::verify(digests[row], keys[row],
            signatures[row]);
}

extern "C" __global__ void verify_schnorr(
    const hash_digest* messages, const ec_xonly* keys,
    const verifier::schnorr_t* signatures, uint8_t* results, uint32_t count)
{
    const auto row = verifier::row();
    if (row < count)
        results[row] = verifier::verify(messages[row], keys[row],
            signatures[row]);
}
