/**
 * Copyright (c) 2011-2024 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_HASH_SHA_ALGORITHM_SCHEDULE_IPP
#define LIBBITCOIN_SYSTEM_HASH_SHA_ALGORITHM_SCHEDULE_IPP

#include <type_traits>

// Message scheduling.
// ============================================================================

namespace libbitcoin {
namespace system {
namespace sha {

// protected
// ----------------------------------------------------------------------------

TEMPLATE
template<size_t Round>
INLINE constexpr void CLASS::
prepare(auto& buffer) NOEXCEPT
{
    // K-adding is shifted 16 words, with last 16 added after scheduling.
    constexpr auto s = SHA::word_bits;

    if constexpr (SHA::strength == 160)
    {
        constexpr auto r03 = Round - 3;
        constexpr auto r08 = Round - 8;
        constexpr auto r14 = Round - 14;
        constexpr auto r16 = Round - 16;

        buffer[Round] = f::rol<1, s>(f::xor_(
            f::xor_(buffer[r16], buffer[r14]),
            f::xor_(buffer[r08], buffer[r03])));

        buffer[r16] = f::addc<K::get[r16], s>(buffer[r16]);
    }
    else
    {
        constexpr auto r02 = Round - 2;
        constexpr auto r07 = Round - 7;
        constexpr auto r15 = Round - 15;
        constexpr auto r16 = Round - 16;

        buffer[Round] = f::add<s>(
            f::add<s>(buffer[r16], sigma0(buffer[r15])),
            f::add<s>(buffer[r07], sigma1(buffer[r02])));

        buffer[r16] = f::addc<K::get[r16], s>(buffer[r16]);
    }
}

TEMPLATE
INLINE constexpr void CLASS::
add_k(auto& buffer) NOEXCEPT
{
    // Add K to last 16 words.
    // TODO: Consolidated K-adding can be performed in 4/8/16 lanes.
    constexpr auto s = SHA::word_bits;
    constexpr auto r = SHA::rounds - SHA::block_words;
    buffer[r + 0] = f::addc<K::get[r + 0], s>(buffer[r + 0]);
    buffer[r + 1] = f::addc<K::get[r + 1], s>(buffer[r + 1]);
    buffer[r + 2] = f::addc<K::get[r + 2], s>(buffer[r + 2]);
    buffer[r + 3] = f::addc<K::get[r + 3], s>(buffer[r + 3]);
    buffer[r + 4] = f::addc<K::get[r + 4], s>(buffer[r + 4]);
    buffer[r + 5] = f::addc<K::get[r + 5], s>(buffer[r + 5]);
    buffer[r + 6] = f::addc<K::get[r + 6], s>(buffer[r + 6]);
    buffer[r + 7] = f::addc<K::get[r + 7], s>(buffer[r + 7]);
    buffer[r + 8] = f::addc<K::get[r + 8], s>(buffer[r + 8]);
    buffer[r + 9] = f::addc<K::get[r + 9], s>(buffer[r + 9]);
    buffer[r + 10] = f::addc<K::get[r + 10], s>(buffer[r + 10]);
    buffer[r + 11] = f::addc<K::get[r + 11], s>(buffer[r + 11]);
    buffer[r + 12] = f::addc<K::get[r + 12], s>(buffer[r + 12]);
    buffer[r + 13] = f::addc<K::get[r + 13], s>(buffer[r + 13]);
    buffer[r + 14] = f::addc<K::get[r + 14], s>(buffer[r + 14]);
    buffer[r + 15] = f::addc<K::get[r + 15], s>(buffer[r + 15]);
}

TEMPLATE
constexpr void CLASS::
schedule_(auto& buffer) NOEXCEPT
{
    prepare<16>(buffer);
    prepare<17>(buffer);
    prepare<18>(buffer);
    prepare<19>(buffer);
    prepare<20>(buffer);
    prepare<21>(buffer);
    prepare<22>(buffer);
    prepare<23>(buffer);
    prepare<24>(buffer);
    prepare<25>(buffer);
    prepare<26>(buffer);
    prepare<27>(buffer);
    prepare<28>(buffer);
    prepare<29>(buffer);
    prepare<30>(buffer);
    prepare<31>(buffer);

    prepare<32>(buffer);
    prepare<33>(buffer);
    prepare<34>(buffer);
    prepare<35>(buffer);
    prepare<36>(buffer);
    prepare<37>(buffer);
    prepare<38>(buffer);
    prepare<39>(buffer);
    prepare<40>(buffer);
    prepare<41>(buffer);
    prepare<42>(buffer);
    prepare<43>(buffer);
    prepare<44>(buffer);
    prepare<45>(buffer);
    prepare<46>(buffer);
    prepare<47>(buffer);

    prepare<48>(buffer);
    prepare<49>(buffer);
    prepare<50>(buffer);
    prepare<51>(buffer);
    prepare<52>(buffer);
    prepare<53>(buffer);
    prepare<54>(buffer);
    prepare<55>(buffer);
    prepare<56>(buffer);
    prepare<57>(buffer);
    prepare<58>(buffer);
    prepare<59>(buffer);
    prepare<60>(buffer);
    prepare<61>(buffer);
    prepare<62>(buffer);
    prepare<63>(buffer);

    if constexpr (SHA::rounds == 80)
    {
        prepare<64>(buffer);
        prepare<65>(buffer);
        prepare<66>(buffer);
        prepare<67>(buffer);
        prepare<68>(buffer);
        prepare<69>(buffer);
        prepare<70>(buffer);
        prepare<71>(buffer);
        prepare<72>(buffer);
        prepare<73>(buffer);
        prepare<74>(buffer);
        prepare<75>(buffer);
        prepare<76>(buffer);
        prepare<77>(buffer);
        prepare<78>(buffer);
        prepare<79>(buffer);
    }

    add_k(buffer);
}

TEMPLATE
constexpr void CLASS::
schedule(buffer_t& buffer) NOEXCEPT
{
    if (std::is_constant_evaluated())
    {
        schedule_(buffer);
    }
    else if constexpr (native)
    {
        // Single block (with shani) message scheduling optimization.
        schedule_native(buffer);
    }
    else if constexpr (vector)
    {
        // Single block (without shani) message scheduling optimization.
        schedule_sigma(buffer);
    }
    else
    {
        schedule_(buffer);
    }
}

} // namespace sha
} // namespace system
} // namespace libbitcoin

#endif
