/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_WALLET_KEYS_HD_PUBLIC_HPP
#define LIBBITCOIN_SYSTEM_WALLET_KEYS_HD_PUBLIC_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/wallet/keys/ec_public.hpp>

namespace libbitcoin
{
namespace system
{
namespace wallet
{

/// A constant used in key derivation.
static constexpr uint32_t hd_first_hardened_key = 1 << 31;

/// An hd key chain code.
static constexpr size_t hd_chain_code_size = 32;
typedef data_array<hd_chain_code_size> hd_chain_code;

/// A decoded hd public or private key.
static constexpr size_t hd_key_size = 82;
typedef data_array<hd_key_size> hd_key;

/// Key derivation information used in the serialization format.
struct BC_API hd_lineage
{
    uint64_t prefixes;
    uint8_t depth;
    uint32_t parent_fingerprint;
    uint32_t child_number;

    bool operator==(const hd_lineage& other) const noexcept;
    bool operator!=(const hd_lineage& other) const noexcept;
};

class hd_private;

/// An extended public key, as defined by BIP32.
class BC_API hd_public
{
public:
    static const uint32_t mainnet;
    static const uint32_t testnet;

    static uint32_t to_prefix(uint64_t prefixes) noexcept
    {
        return prefixes & 0x00000000ffffffff;
    }

    /// Constructors.
    hd_public() noexcept;
    hd_public(const hd_public& other) noexcept;
    hd_public(const hd_key& public_key) noexcept;
    hd_public(const hd_key& public_key, uint32_t prefix) noexcept;
    hd_public(const std::string& encoded) noexcept;
    hd_public(const std::string& encoded, uint32_t prefix) noexcept;

    /// Operators.
    bool operator<(const hd_public& other) const noexcept;
    bool operator==(const hd_public& other) const noexcept;
    bool operator!=(const hd_public& other) const noexcept;
    hd_public& operator=(const hd_public& other) noexcept;
    friend std::istream& operator>>(std::istream& in, hd_public& to);
    friend std::ostream& operator<<(
        std::ostream& out, const hd_public& of) noexcept;

    /// Cast operators.
    operator bool() const noexcept;
    operator const ec_compressed&() const noexcept;

    /// Serializer.
    std::string encoded() const noexcept;

    /// Accessors.
    const hd_chain_code& chain_code() const noexcept;
    const hd_lineage& lineage() const noexcept;
    const ec_compressed& point() const noexcept;

    /// Methods.
    hd_key to_hd_key() const noexcept;
    hd_public derive_public(uint32_t index) const noexcept;

protected:
    /// Factories.
    static hd_public from_secret(
        const ec_secret& secret, const hd_chain_code& chain_code,
        const hd_lineage& lineage) noexcept;

    /// Helpers.
    uint32_t fingerprint() const noexcept;

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_;
    hd_chain_code chain_;
    hd_lineage lineage_;
    ec_compressed point_;

private:
    static hd_public from_key(const hd_key& public_key) noexcept;
    static hd_public from_string(const std::string& encoded) noexcept;
    static hd_public from_key(
        const hd_key& public_key, uint32_t prefix) noexcept;
    static hd_public from_string(
        const std::string& encoded, uint32_t prefix) noexcept;

    hd_public(
        const ec_compressed& point, const hd_chain_code& chain_code,
        const hd_lineage& lineage) noexcept;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
