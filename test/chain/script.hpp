/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_TEST_SCRIPT_HPP
#define LIBBITCOIN_SYSTEM_TEST_SCRIPT_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <bitcoin/system.hpp>

struct script_test
{
    std::string input;
    std::string output;
    std::string description;
    uint32_t input_sequence;
    uint32_t locktime;
    uint32_t version;
};

typedef std::vector<script_test> script_test_list;

// These are valid prior to and after BIP16 activation.
const script_test_list valid_bip16_scripts{
    {{"0 [51]", "hash160 [da1745e9b549bd0bfa1a569971c77eba30cd5a4b] equal",
      "trivial p2sh"},
     {"[1.] [0.51]", "hash160 [da1745e9b549bd0bfa1a569971c77eba30cd5a4b] equal",
      "basic p2sh"}}};

// These are valid prior to BIP16 activation and invalid after.
const script_test_list invalidated_bip16_scripts{
    {{"nop [51]", "hash160 [da1745e9b549bd0bfa1a569971c77eba30cd5a4b] equal",
      "is_push_only fails under bip16"},
     {"nop1 [51]", "hash160 [da1745e9b549bd0bfa1a569971c77eba30cd5a4b] equal",
      "is_push_only fails under bip16"},
     {"0 [50]", "hash160 [ece424a6bb6ddf4db592c0faed60685047a361b1] equal",
      "op_reserved as p2sh serialized script fails"},
     {"0 [62]", "hash160 [0f4d7845db968f2a81b530b6f3c1d6246d4c7e01] equal",
      "op_ver as p2sh serialized script fails"}}};

// These are valid prior to and after BIP65 activation.
const script_test_list valid_bip65_scripts{
    {{"42", "checklocktimeverify", "valid cltv, true return", 42, 99},
     {"42", "nop1 nop2 nop3 nop4 nop5 nop6 nop7 nop8 nop9 nop10 [2a] equal",
      "bip112 would fail nop3", 42, 99}}};

// These are invalid prior to and after BIP65 activation.
const script_test_list invalid_bip65_scripts{
    {{"", "nop2", "empty nop2", 42, 99},
     {"", "checklocktimeverify", "empty cltv", 42, 99}}};

// These are valid prior to BIP65 activation and invalid after.
const script_test_list invalidated_bip65_scripts{
    {{"1 -1", "checklocktimeverify", "negative cltv", 42, 99},
     {"1 100", "checklocktimeverify", "exceeded cltv", 42, 99},
     {"1 500000000", "checklocktimeverify", "mismatched cltv", 42, 99},
     {"'nop_1_to_10' nop1 nop2 nop3 nop4 nop5 nop6 nop7 nop8 nop9 nop10",
      "'nop_1_to_10' equal", "bip112 would fail nop3", 42, 99},
     {"nop", "nop2 1", "", 42, 99}}};

// These are always valid.
const script_test_list valid_multisig_scripts{{
    {"", "0 0 0 checkmultisig verify depth 0 equal",
     "checkmultisig is allowed to have zero keys and/or sigs"},
    {"", "0 0 0 checkmultisigverify depth 0 equal", ""},
    {"", "0 0 0 1 checkmultisig verify depth 0 equal",
     "zero sigs means no sigs are checked"},
    {"", "0 0 0 1 checkmultisigverify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 2 checkmultisig verify depth 0 equal",
     "test from up to 20 pubkeys, all not checked"},
    {"", "0 0 'a' 'b' 'c' 3 checkmultisig verify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 'c' 'd' 4 checkmultisig verify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 'c' 'd' 'e' 5 checkmultisig verify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 'c' 'd' 'e' 'f' 6 checkmultisig verify depth 0 equal",
     ""},
    {"", "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 7 checkmultisig verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 8 checkmultisig verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 9 checkmultisig verify depth 0 "
     "equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 10 checkmultisig verify "
     "depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 11 checkmultisig verify "
     "depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 12 checkmultisig "
     "verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 13 checkmultisig "
     "verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 14 "
     "checkmultisig verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 15 "
     "checkmultisig verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 16 "
     "checkmultisig verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "17 checkmultisig verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 18 checkmultisig verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 's' 19 checkmultisig verify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 's' 't' 20 checkmultisig verify depth 0 equal",
     ""},
    {"", "0 0 'a' 1 checkmultisigverify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 2 checkmultisigverify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 'c' 3 checkmultisigverify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 'c' 'd' 4 checkmultisigverify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 'c' 'd' 'e' 5 checkmultisigverify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 'c' 'd' 'e' 'f' 6 checkmultisigverify depth 0 equal", ""},
    {"", "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 7 checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 8 checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 9 checkmultisigverify depth 0 "
     "equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 10 checkmultisigverify depth "
     "0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 11 checkmultisigverify "
     "depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 12 "
     "checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 13 "
     "checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 14 "
     "checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 15 "
     "checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 16 "
     "checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "17 checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 18 checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 's' 19 checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 's' 't' 20 checkmultisigverify depth 0 equal",
     ""},
    {"",
     "0 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig",
     "nopcount is incremented by the number of keys evaluated in addition to "
     "the usual one op per op. in this case we have zero keys, so we can "
     "execute 201 checkmultisigs"},
    {"1",
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 "
     "checkmultisigverify 0 0 0 checkmultisigverify 0 0 0 checkmultisigverify "
     "0 0 0 checkmultisigverify",
     ""},
    {"",
     "nop nop nop nop nop nop nop nop nop nop nop nop 0 0 'a' 'b' 'c' 'd' 'e' "
     "'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' "
     "'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' "
     "'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig "
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 's' 't' 20 checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' "
     "'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' "
     "'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' "
     "'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig "
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 's' 't' 20 checkmultisig",
     "even though there are no signatures being checked nopcount is "
     "incremented by the number of keys."},
    {"1",
     "nop nop nop nop nop nop nop nop nop nop nop nop 0 0 'a' 'b' 'c' 'd' 'e' "
     "'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' "
     "'m' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisigverify 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' "
     "'m' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisigverify 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' "
     "'m' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisigverify 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' "
     "'m' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisigverify 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify",
     ""},
}};

// These are always invalid.
const script_test_list invalid_multisig_scripts{{
    {"",
     "0 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 checkmultisig 0 0 "
     "checkmultisig 0 0 checkmultisig 0 0 checkmultisig",
     "202 checkmultisigs, fails due to 201 op limit"},
    {"1",
     "0 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify 0 0 checkmultisigverify 0 0 "
     "checkmultisigverify 0 0 checkmultisigverify",
     ""},
    {"",
     "nop nop nop nop nop nop nop nop nop nop nop nop nop 0 0 'a' 'b' 'c' 'd' "
     "'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' "
     "'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' "
     "'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig "
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 's' 't' 20 checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' "
     "'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' "
     "'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig 0 0 'a' 'b' 'c' 'd' 'e' 'f' "
     "'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisig "
     "0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' "
     "'r' 's' 't' 20 checkmultisig",
     "fails due to 201 sig op limit"},
    {"1",
     "nop nop nop nop nop nop nop nop nop nop nop nop nop 0 0 'a' 'b' 'c' 'd' "
     "'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' "
     "'m' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisigverify 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' "
     "'m' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisigverify 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' "
     "'m' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisigverify 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify 0 0 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' "
     "'m' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 checkmultisigverify 0 0 'a' 'b' 'c' "
     "'d' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 20 "
     "checkmultisigverify",
     ""},
}};

// These are always valid.
const script_test_list valid_context_free_scripts{
    {{"", "depth 0 equal",
      "test the test: we should have an empty stack after scriptsig "
      "evaluation"},
     {"  ", "depth 0 equal", "and multiple spaces should not change that."},
     {"   ", "depth 0 equal", ""},
     {"    ", "depth 0 equal", ""},
     {"1 2", "2 equalverify 1 equal",
      "similarly whitespace around and between symbols"},
     {"1  2", "2 equalverify 1 equal", ""},
     {"  1  2", "2 equalverify 1 equal", ""},
     {"1  2  ", "2 equalverify 1 equal", ""},
     {"  1  2  ", "2 equalverify 1 equal", ""},
     {"1", "", ""},
     {"[0b]", "11 equal", "push 1 byte"},
     {"[417a]", "'Az' equal", "push 2 bytes"},
     {"[417a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7"
      "a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7"
      "a7a7a7a]",
      "'Azzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"
      "zzzz' equal",
      "push 75 bytes"},
     {"[1.07]", "7 equal", "non-minimal op_pushdata1"},
     {"[2.08]", "8 equal", "non-minimal op_pushdata2"},
     {"[4.09]", "9 equal", "non-minimal op_pushdata4"},
     {"0x4c", "0 equal", "0x4c is op_pushdata1"},
     {"0x4d", "0 equal", "0x4d is op_pushdata2"},
     {"0x4e", "0 equal", "0x4f is op_pushdata4"},
     {"0x4f 1000 add", "999 equal", "1000 - 1 = 999"},
     {"0", "if 0x50 endif 1", "0x50 is reserved (ok if not executed)"},
     {"0x51", "0x5f add 0x60 equal",
      "0x51 through 0x60 push 1 through 16 onto stack"},
     {"1", "nop", ""},
     {"0", "if ver else 1 endif", "ver non-functional (ok if not executed)"},
     {"0", "if reserved reserved1 reserved2 else 1 endif",
      "reserved ok in un-executed if"},
     {"1", "dup if endif", ""},
     {"1", "if 1 endif", ""},
     {"1", "dup if else endif", ""},
     {"1", "if 1 else endif", ""},
     {"0", "if else 1 endif", ""},
     {"1 1", "if if 1 else 0 endif endif", ""},
     {"1 0", "if if 1 else 0 endif endif", ""},
     {"1 1", "if if 1 else 0 endif else if 0 else 1 endif endif", ""},
     {"0 0", "if if 1 else 0 endif else if 0 else 1 endif endif", ""},
     {"1 0", "notif if 1 else 0 endif endif", ""},
     {"1 1", "notif if 1 else 0 endif endif", ""},
     {"1 0", "notif if 1 else 0 endif else if 0 else 1 endif endif", ""},
     {"0 1", "notif if 1 else 0 endif else if 0 else 1 endif endif", ""},
     {"0", "if 0 else 1 else 0 endif",
      "multiple else's are valid and executed inverts on each else "
      "encountered"},
     {"1", "if 1 else 0 else endif", ""},
     {"1", "if else 0 else 1 endif", ""},
     {"1", "if 1 else 0 else 1 endif add 2 equal", ""},
     {"'' 1",
      "if sha1 else else sha1 else else sha1 else else sha1 else else sha1 "
      "else else sha1 else else sha1 else else sha1 else else sha1 else else "
      "sha1 else else sha1 else else sha1 else else sha1 else else sha1 else "
      "else sha1 else else sha1 else else sha1 else else sha1 else else sha1 "
      "else else sha1 endif [68ca4fec736264c13b859bac43d5173df6871682] equal",
      ""},
     {"1", "notif 0 else 1 else 0 endif",
      "multiple else's are valid and execution inverts on each else "
      "encountered"},
     {"0", "notif 1 else 0 else endif", ""},
     {"0", "notif else 0 else 1 endif", ""},
     {"0", "notif 1 else 0 else 1 endif add 2 equal", ""},
     {"'' 0",
      "notif sha1 else else sha1 else else sha1 else else sha1 else else sha1 "
      "else else sha1 else else sha1 else else sha1 else else sha1 else else "
      "sha1 else else sha1 else else sha1 else else sha1 else else sha1 else "
      "else sha1 else else sha1 else else sha1 else else sha1 else else sha1 "
      "else else sha1 endif [68ca4fec736264c13b859bac43d5173df6871682] equal",
      ""},
     {"0",
      "if 1 if return else return else return endif else 1 if 1 else return "
      "else 1 endif else return endif add 2 equal",
      "nested else else"},
     {"1",
      "notif 0 notif return else return else return endif else 0 notif 1 else "
      "return else 1 endif else return endif add 2 equal",
      ""},
     {"0", "if return endif 1", "return only works if executed"},
     {"1 1", "verify", ""},
     {"10 0 11 toaltstack drop fromaltstack", "add 21 equal", ""},
     {"'gavin_was_here' toaltstack 11 fromaltstack",
      "'gavin_was_here' equalverify 11 equal", ""},
     {"0 ifdup", "depth 1 equalverify 0 equal", ""},
     {"1 ifdup", "depth 2 equalverify 1 equalverify 1 equal", ""},
     {"0 drop", "depth 0 equal", ""},
     {"0", "dup 1 add 1 equalverify 0 equal", ""},
     {"0 1", "nip", ""},
     {"1 0", "over depth 3 equalverify", ""},
     {"22 21 20", "0 pick 20 equalverify depth 3 equal", ""},
     {"22 21 20", "1 pick 21 equalverify depth 3 equal", ""},
     {"22 21 20", "2 pick 22 equalverify depth 3 equal", ""},
     {"22 21 20", "0 roll 20 equalverify depth 2 equal", ""},
     {"22 21 20", "1 roll 21 equalverify depth 2 equal", ""},
     {"22 21 20", "2 roll 22 equalverify depth 2 equal", ""},
     {"22 21 20", "rot 22 equal", ""},
     {"22 21 20", "rot drop 20 equal", ""},
     {"22 21 20", "rot drop drop 21 equal", ""},
     {"22 21 20", "rot rot 21 equal", ""},
     {"22 21 20", "rot rot rot 20 equal", ""},
     {"25 24 23 22 21 20", "rot2 24 equal", ""},
     {"25 24 23 22 21 20", "rot2 drop 25 equal", ""},
     {"25 24 23 22 21 20", "rot2 drop2 20 equal", ""},
     {"25 24 23 22 21 20", "rot2 drop2 drop 21 equal", ""},
     {"25 24 23 22 21 20", "rot2 drop2 drop2 22 equal", ""},
     {"25 24 23 22 21 20", "rot2 drop2 drop2 drop 23 equal", ""},
     {"25 24 23 22 21 20", "rot2 rot2 22 equal", ""},
     {"25 24 23 22 21 20", "rot2 rot2 rot2 20 equal", ""},
     {"1 0", "swap 1 equalverify 0 equal", ""},
     {"0 1", "tuck depth 3 equalverify swap drop2", ""},
     {"13 14", "dup2 rot equalverify equal", ""},
     {"-1 0 1 2",
      "dup3 depth 7 equalverify add add 3 equalverify drop2 0 equalverify", ""},
     {"1 2 3 5", "over2 add add 8 equalverify add add 6 equal", ""},
     {"1 3 5 7", "swap2 add 4 equalverify add 12 equal", ""},
     {"0", "size 0 equal", ""},
     {"1", "size 1 equal", ""},
     {"127", "size 1 equal", ""},
     {"128", "size 2 equal", ""},
     {"32767", "size 2 equal", ""},
     {"32768", "size 3 equal", ""},
     {"8388607", "size 3 equal", ""},
     {"8388608", "size 4 equal", ""},
     {"2147483647", "size 4 equal", ""},
     {"2147483648", "size 5 equal", ""},
     {"549755813887", "size 5 equal", ""},
     {"549755813888", "size 6 equal", ""},
     {"9223372036854775807", "size 8 equal", ""},
     {"-1", "size 1 equal", ""},
     {"-127", "size 1 equal", ""},
     {"-128", "size 2 equal", ""},
     {"-32767", "size 2 equal", ""},
     {"-32768", "size 3 equal", ""},
     {"-8388607", "size 3 equal", ""},
     {"-8388608", "size 4 equal", ""},
     {"-2147483647", "size 4 equal", ""},
     {"-2147483648", "size 5 equal", ""},
     {"-549755813887", "size 5 equal", ""},
     {"-549755813888", "size 6 equal", ""},
     {"-9223372036854775807", "size 8 equal", ""},
     {"'abcdefghijklmnopqrstuvwxyz'", "size 26 equal", ""},
     {"2 -2 add", "0 equal", ""},
     {"2147483647 -2147483647 add", "0 equal", ""},
     {"-1 -1 add", "-2 equal", ""},
     {"0 0", "equal", ""},
     {"1 1 add", "2 equal", ""},
     {"1 add1", "2 equal", ""},
     {"111 sub1", "110 equal", ""},
     {"111 1 add 12 sub", "100 equal", ""},
     {"0 abs", "0 equal", ""},
     {"16 abs", "16 equal", ""},
     {"-16 abs", "-16 negate equal", ""},
     {"0 not", "nop", ""},
     {"1 not", "0 equal", ""},
     {"11 not", "0 equal", ""},
     {"0 nonzero", "0 equal", ""},
     {"1 nonzero", "1 equal", ""},
     {"111 nonzero", "1 equal", ""},
     {"-111 nonzero", "1 equal", ""},
     {"1 1 booland", "nop", ""},
     {"1 0 booland", "not", ""},
     {"0 1 booland", "not", ""},
     {"0 0 booland", "not", ""},
     {"16 17 booland", "nop", ""},
     {"1 1 boolor", "nop", ""},
     {"1 0 boolor", "nop", ""},
     {"0 1 boolor", "nop", ""},
     {"0 0 boolor", "not", ""},
     {"16 17 boolor", "nop", ""},
     {"11 10 1 add", "numequal", ""},
     {"11 10 1 add", "numequalverify 1", ""},
     {"11 10 1 add", "numnotequal not", ""},
     {"111 10 1 add", "numnotequal", ""},
     {"11 10", "lessthan not", ""},
     {"4 4", "lessthan not", ""},
     {"10 11", "lessthan", ""},
     {"-11 11", "lessthan", ""},
     {"-11 -10", "lessthan", ""},
     {"11 10", "greaterthan", ""},
     {"4 4", "greaterthan not", ""},
     {"10 11", "greaterthan not", ""},
     {"-11 11", "greaterthan not", ""},
     {"-11 -10", "greaterthan not", ""},
     {"11 10", "lessthanorequal not", ""},
     {"4 4", "lessthanorequal", ""},
     {"10 11", "lessthanorequal", ""},
     {"-11 11", "lessthanorequal", ""},
     {"-11 -10", "lessthanorequal", ""},
     {"11 10", "greaterthanorequal", ""},
     {"4 4", "greaterthanorequal", ""},
     {"10 11", "greaterthanorequal not", ""},
     {"-11 11", "greaterthanorequal not", ""},
     {"-11 -10", "greaterthanorequal not", ""},
     {"1 0 min", "0 numequal", ""},
     {"0 1 min", "0 numequal", ""},
     {"-1 0 min", "-1 numequal", ""},
     {"0 -2147483647 min", "-2147483647 numequal", ""},
     {"2147483647 0 max", "2147483647 numequal", ""},
     {"0 100 max", "100 numequal", ""},
     {"-100 0 max", "0 numequal", ""},
     {"0 -2147483647 max", "0 numequal", ""},
     {"0 0 1", "within", ""},
     {"1 0 1", "within not", ""},
     {"0 -2147483647 2147483647", "within", ""},
     {"-1 -100 100", "within", ""},
     {"11 -100 100", "within", ""},
     {"-2147483647 -100 100", "within not", ""},
     {"2147483647 -100 100", "within not", ""},
     {"2147483647 2147483647 sub", "0 equal", ""},
     {"2147483647 dup add", "4294967294 equal", ">32 bit equal is valid"},
     {"2147483647 negate dup add", "-4294967294 equal", ""},
     {"''", "ripemd160 [9c1185a5c5e9fc54612808977ee8f548b2258d31] equal", ""},
     {"'a'", "ripemd160 [0bdc9d2d256b3ee9daae347be6f4dc835a467ffe] equal", ""},
     {"'abcdefghijklmnopqrstuvwxyz'",
      "ripemd160 [f71c27109c692c1b56bbdceb5b9d2865b3708dbc] equal", ""},
     {"''", "sha1 [da39a3ee5e6b4b0d3255bfef95601890afd80709] equal", ""},
     {"'a'", "sha1 [86f7e437faa5a7fce15d1ddcb9eaeaea377667b8] equal", ""},
     {"'abcdefghijklmnopqrstuvwxyz'",
      "sha1 [32d10c7b8cf96570ca04ce37f2a19d84240d3a89] equal", ""},
     {"''",
      "sha256 "
      "[e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855] "
      "equal",
      ""},
     {"'a'",
      "sha256 "
      "[ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb] "
      "equal",
      ""},
     {"'abcdefghijklmnopqrstuvwxyz'",
      "sha256 "
      "[71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73] "
      "equal",
      ""},
     {"''", "dup hash160 swap sha256 ripemd160 equal", ""},
     {"''", "dup hash256 swap sha256 sha256 equal", ""},
     {"''", "nop hash160 [b472a266d0bd89c13706a4132ccfb16f7c3b9fcb] equal", ""},
     {"'a'", "hash160 nop [994355199e516ff76c4fa4aab39337b9d84cf12b] equal",
      ""},
     {"'abcdefghijklmnopqrstuvwxyz'",
      "hash160 [1.c286a1af0947f58d1ad787385b1c2c4a976f9e71] equal", ""},
     {"''",
      "hash256 "
      "[5df6e0e2761359d30a8275058e299fcc0381534545f55cf43e41983f5d4c9456] "
      "equal",
      ""},
     {"'a'",
      "hash256 "
      "[bf5d3affb73efd2ec6c36ad3112dd933efed63c4e1cbffcfa88e2759c144f2d8] "
      "equal",
      ""},
     {"'abcdefghijklmnopqrstuvwxyz'",
      "hash256 "
      "[1.ca139bc10c2f660da42666f72e89a225936fc60f193c161124a672050c434671] "
      "equal",
      ""},
     {"1", "nop1 nop4 nop5 nop6 nop7 nop8 nop9 nop10 1 equal", ""},

     {"'nop_1_to_10' nop1 nop4 nop5 nop6 nop7 nop8 nop9 nop10",
      "'nop_1_to_10' equal", ""},
     {"0", "if 0xba else 1 endif", "opcodes above nop10 invalid if executed"},
     {"0", "if 0xbb else 1 endif", ""},
     {"0", "if 0xbc else 1 endif", ""},
     {"0", "if 0xbd else 1 endif", ""},
     {"0", "if 0xbe else 1 endif", ""},
     {"0", "if 0xbf else 1 endif", ""},
     {"0", "if 0xc0 else 1 endif", ""},
     {"0", "if 0xc1 else 1 endif", ""},
     {"0", "if 0xc2 else 1 endif", ""},
     {"0", "if 0xc3 else 1 endif", ""},
     {"0", "if 0xc4 else 1 endif", ""},
     {"0", "if 0xc5 else 1 endif", ""},
     {"0", "if 0xc6 else 1 endif", ""},
     {"0", "if 0xc7 else 1 endif", ""},
     {"0", "if 0xc8 else 1 endif", ""},
     {"0", "if 0xc9 else 1 endif", ""},
     {"0", "if 0xca else 1 endif", ""},
     {"0", "if 0xcb else 1 endif", ""},
     {"0", "if 0xcc else 1 endif", ""},
     {"0", "if 0xcd else 1 endif", ""},
     {"0", "if 0xce else 1 endif", ""},
     {"0", "if 0xcf else 1 endif", ""},
     {"0", "if 0xd0 else 1 endif", ""},
     {"0", "if 0xd1 else 1 endif", ""},
     {"0", "if 0xd2 else 1 endif", ""},
     {"0", "if 0xd3 else 1 endif", ""},
     {"0", "if 0xd4 else 1 endif", ""},
     {"0", "if 0xd5 else 1 endif", ""},
     {"0", "if 0xd6 else 1 endif", ""},
     {"0", "if 0xd7 else 1 endif", ""},
     {"0", "if 0xd8 else 1 endif", ""},
     {"0", "if 0xd9 else 1 endif", ""},
     {"0", "if 0xda else 1 endif", ""},
     {"0", "if 0xdb else 1 endif", ""},
     {"0", "if 0xdc else 1 endif", ""},
     {"0", "if 0xdd else 1 endif", ""},
     {"0", "if 0xde else 1 endif", ""},
     {"0", "if 0xdf else 1 endif", ""},
     {"0", "if 0xe0 else 1 endif", ""},
     {"0", "if 0xe1 else 1 endif", ""},
     {"0", "if 0xe2 else 1 endif", ""},
     {"0", "if 0xe3 else 1 endif", ""},
     {"0", "if 0xe4 else 1 endif", ""},
     {"0", "if 0xe5 else 1 endif", ""},
     {"0", "if 0xe6 else 1 endif", ""},
     {"0", "if 0xe7 else 1 endif", ""},
     {"0", "if 0xe8 else 1 endif", ""},
     {"0", "if 0xe9 else 1 endif", ""},
     {"0", "if 0xea else 1 endif", ""},
     {"0", "if 0xeb else 1 endif", ""},
     {"0", "if 0xec else 1 endif", ""},
     {"0", "if 0xed else 1 endif", ""},
     {"0", "if 0xee else 1 endif", ""},
     {"0", "if 0xef else 1 endif", ""},
     {"0", "if 0xf0 else 1 endif", ""},
     {"0", "if 0xf1 else 1 endif", ""},
     {"0", "if 0xf2 else 1 endif", ""},
     {"0", "if 0xf3 else 1 endif", ""},
     {"0", "if 0xf4 else 1 endif", ""},
     {"0", "if 0xf5 else 1 endif", ""},
     {"0", "if 0xf6 else 1 endif", ""},
     {"0", "if 0xf7 else 1 endif", ""},
     {"0", "if 0xf8 else 1 endif", ""},
     {"0", "if 0xf9 else 1 endif", ""},
     {"0", "if 0xfa else 1 endif", ""},
     {"0", "if 0xfb else 1 endif", ""},
     {"0", "if 0xfc else 1 endif", ""},
     {"0", "if 0xfd else 1 endif", ""},
     {"0", "if 0xfe else 1 endif", ""},
     {"0", "if 0xff else 1 endif", ""},
     {"nop",
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb'",
      "520 byte push"},
     {"1",
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61  0x61",
      "201 opcodes executed. 0x61 is nop"},
     {"1 2 3 4 5 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f",
      "1 2 3 4 5 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f",
      "1,000 stack size (0x6f is dup3)"},
     {"1 toaltstack 2 toaltstack 3 4 5 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f",
      "1 2 3 4 5 6 7 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f",
      "1,000 stack size (altstack cleared between scriptsig/scriptpubkey)"},
     {"'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaa' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f",
      "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaa' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f dup2 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61",
      "max-size (10,000-byte), max-push(520 bytes), max-opcodes(201), max "
      "stack size(1,000 items). 0x6f is dup3, 0x61 is nop"},
     {"0",
      "if 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 0x50 "
      "0x50 0x50 0x50 0x50 0x50 endif 1",
      ">201 opcodes, but reserved (0x50) doesn't count towards opcode limit."},
     {"nop", "1", ""},
     {"1", "[01] equal",
      "the following is useful for checking implementations of bn_bn2mpi"},
     {"127", "[7f] equal", ""},
     {"128", "[8000] equal", "leave room for the sign bit"},
     {"32767", "[ff7f] equal", ""},
     {"32768", "[008000] equal", ""},
     {"8388607", "[ffff7f] equal", ""},
     {"8388608", "[00008000] equal", ""},
     {"2147483647", "[ffffff7f] equal", ""},
     {"2147483648", "[0000008000] equal", ""},
     {"549755813887", "[ffffffff7f] equal", ""},
     {"549755813888", "[000000008000] equal", ""},
     {"9223372036854775807", "[ffffffffffffff7f] equal", ""},
     {"-1", "[81] equal",
      "numbers are little-endian with the msb being a sign bit"},
     {"-127", "[ff] equal", ""},
     {"-128", "[8080] equal", ""},
     {"-32767", "[ffff] equal", ""},
     {"-32768", "[008080] equal", ""},
     {"-8388607", "[ffffff] equal", ""},
     {"-8388608", "[00008080] equal", ""},
     {"-2147483647", "[ffffffff] equal", ""},
     {"-2147483648", "[0000008080] equal", ""},
     {"-4294967295", "[ffffffff80] equal", ""},
     {"-549755813887", "[ffffffffff] equal", ""},
     {"-549755813888", "[000000008080] equal", ""},
     {"-9223372036854775807", "[ffffffffffffffff] equal", ""},
     {"2147483647", "add1 2147483648 equal",
      "we can do math on 4-byte integers, and compare 5-byte ones"},
     {"2147483647", "add1 1", ""},
     {"-2147483647", "add1 1", ""},
     {"1", "[0100] equal not", "not the same byte array..."},
     {"1", "[0100] numequal", "... but they are numerically equal"},
     {"11", "[1.0b0000] numequal", ""},
     {"0", "[80] equal not", ""},
     {"0", "[80] numequal", "zero numerically equals negative zero"},
     {"0", "[0080] numequal", ""},
     {"[000080]", "[00000080] numequal", ""},
     {"[100080]", "[10000080] numequal", ""},
     {"[100000]", "[10000000] numequal", ""},
     {"nop", "nop 1",
      "the following tests check the if(stack.size() < n) tests in each "
      "opcode"},
     {"1", "if 1 endif", "they are here to catch copy-and-paste errors"},
     {"0", "notif 1 endif", "most of them are duplicated elsewhere,"},
     {"1", "verify 1", "but, hey, more is always better, right?"},
     {"0", "toaltstack 1", ""},
     {"1", "toaltstack fromaltstack", ""},
     {"0 0", "drop2 1", ""},
     {"0 1", "dup2", ""},
     {"0 0 1", "dup3", ""},
     {"0 1 0 0", "over2", ""},
     {"0 1 0 0 0 0", "rot2", ""},
     {"0 1 0 0", "swap2", ""},
     {"1", "ifdup", ""},
     {"nop", "depth 1", ""},
     {"0", "drop 1", ""},
     {"1", "dup", ""},
     {"0 1", "nip", ""},
     {"1 0", "over", ""},
     {"1 0 0 0 3", "pick", ""},
     {"1 0", "pick", ""},
     {"1 0 0 0 3", "roll", ""},
     {"1 0", "roll", ""},
     {"1 0 0", "rot", ""},
     {"1 0", "swap", ""},
     {"0 1", "tuck", ""},
     {"1", "size", ""},
     {"0 0", "equal", ""},
     {"0 0", "equalverify 1", ""},
     {"0", "add1", ""},
     {"2", "sub1", ""},
     {"-1", "negate", ""},
     {"-1", "abs", ""},
     {"0", "not", ""},
     {"-1", "nonzero", ""},
     {"1 0", "add", ""},
     {"1 0", "sub", ""},
     {"-1 -1", "booland", ""},
     {"-1 0", "boolor", ""},
     {"0 0", "numequal", ""},
     {"0 0", "numequalverify 1", ""},
     {"-1 0", "numnotequal", ""},
     {"-1 0", "lessthan", ""},
     {"1 0", "greaterthan", ""},
     {"0 0", "lessthanorequal", ""},
     {"0 0", "greaterthanorequal", ""},
     {"-1 0", "min", ""},
     {"1 0", "max", ""},
     {"-1 -1 0", "within", ""},
     {"0", "ripemd160", ""},
     {"0", "sha1", ""},
     {"0", "sha256", ""},
     {"0", "hash160", ""},
     {"0", "hash256", ""},
     {"nop", "codeseparator 1", ""},
     {"nop", "nop1 1", ""},
     {"nop", "nop4 1", ""},
     {"nop", "nop5 1", ""},
     {"nop", "nop6 1", ""},
     {"nop", "nop7 1", ""},
     {"nop", "nop8 1", ""},
     {"nop", "nop9 1", ""},
     {"nop", "nop10 1", ""},
     {"[42424242424242424242424242424242424242424242424242424242424242424242424"
      "242424242424242424242424242424242424242424242424242424242]",
      "[2."
      "424242424242424242424242424242424242424242424242424242424242424242424242"
      "42424242424242424242424242424242424242424242424242424242] equal",
      "basic push signedness check"},
     {"[1."
      "424242424242424242424242424242424242424242424242424242424242424242424242"
      "42424242424242424242424242424242424242424242424242424242]",
      "[2."
      "424242424242424242424242424242424242424242424242424242424242424242424242"
      "42424242424242424242424242424242424242424242424242424242] equal",
      "basic pushdata1 signedness check"},
     {"0x00", "size 0 equal", "basic op_0 execution"}}};

// These are always invalid.
const script_test_list invalid_context_free_scripts{
    {{"", "depth",
      "test the test: we should have an empty stack after scriptsig "
      "evaluation"},
     {"  ", "depth", "and multiple spaces should not change that."},
     {"   ", "depth", ""},
     {"    ", "depth", ""},
     {"", "", ""},
     {"", "nop", ""},
     {"", "nop depth", ""},
     {"nop", "", ""},
     {"nop", "depth", ""},
     {"nop", "nop", ""},
     {"nop", "nop depth", ""},
     {"depth", "", ""},

     // Reserved.
     {"1", "if 0x50 endif 1", "0x50 is reserved"},

     {"0x52", "0x5f add 0x60 equal",
      "0x51 through 0x60 push 1 through 16 onto stack"},
     {"0", "nop", ""},

     // Reserved.
     {"1", "if ver else 1 endif", "ver is reserved"},
     {"0", "if verif else 1 endif",
      "verif illegal everywhere (but also reserved?)"},
     {"0", "if else 1 else verif endif",
      "verif illegal everywhere (but also reserved?)"},
     {"0", "if vernotif else 1 endif",
      "vernotif illegal everywhere (but also reserved?)"},
     {"0", "if else 1 else vernotif endif",
      "vernotif illegal everywhere (but also reserved?)"},

     {"1 if", "1 endif", "if/endif can't span scriptsig/scriptpubkey"},
     {"1 if 0 endif", "1 endif", ""},
     {"1 else 0 endif", "1", ""},
     {"0 notif", "123", ""},
     {"0", "dup if endif", ""},
     {"0", "if 1 endif", ""},
     {"0", "dup if else endif", ""},
     {"0", "if 1 else endif", ""},
     {"0", "notif else 1 endif", ""},
     {"0 1", "if if 1 else 0 endif endif", ""},
     {"0 0", "if if 1 else 0 endif endif", ""},
     {"1 0", "if if 1 else 0 endif else if 0 else 1 endif endif", ""},
     {"0 1", "if if 1 else 0 endif else if 0 else 1 endif endif", ""},
     {"0 0", "notif if 1 else 0 endif endif", ""},
     {"0 1", "notif if 1 else 0 endif endif", ""},
     {"1 1", "notif if 1 else 0 endif else if 0 else 1 endif endif", ""},
     {"0 0", "notif if 1 else 0 endif else if 0 else 1 endif endif", ""},
     {"1", "if return else else 1 endif", "multiple elses"},
     {"1", "if 1 else else return endif", ""},
     {"1", "endif", "malformed if/else/endif sequence"},
     {"1", "else endif", ""},
     {"1", "endif else", ""},
     {"1", "endif else if", ""},
     {"1", "if else endif else", ""},
     {"1", "if else endif else endif", ""},
     {"1", "if endif endif", ""},
     {"1", "if else else endif endif", ""},
     {"1", "return", ""},
     {"1", "dup if return endif", ""},
     {"1", "return 'data'", "canonical prunable txout format"},
     {"0 if", "return endif 1",
      "still prunable because if/endif can't span scriptsig/scriptpubkey"},
     {"0", "verify 1", ""},
     {"1", "verify", ""},
     {"1", "verify 0", ""},
     {"1 toaltstack", "fromaltstack 1",
      "alt stack not shared between sig/pubkey"},
     {"ifdup", "depth 0 equal", ""},
     {"drop", "depth 0 equal", ""},
     {"dup", "depth 0 equal", ""},
     {"1", "dup 1 add 2 equalverify 0 equal", ""},
     {"nop", "nip", ""},
     {"nop", "1 nip", ""},
     {"nop", "1 0 nip", ""},
     {"nop", "over 1", ""},
     {"1", "over", ""},
     {"0 1", "over depth 3 equalverify", ""},
     {"19 20 21", "pick 19 equalverify depth 2 equal", ""},
     {"nop", "0 pick", ""},
     {"1", "-1 pick", ""},
     {"19 20 21", "0 pick 20 equalverify depth 3 equal", ""},
     {"19 20 21", "1 pick 21 equalverify depth 3 equal", ""},
     {"19 20 21", "2 pick 22 equalverify depth 3 equal", ""},
     {"nop", "0 roll", ""},
     {"1", "-1 roll", ""},
     {"19 20 21", "0 roll 20 equalverify depth 2 equal", ""},
     {"19 20 21", "1 roll 21 equalverify depth 2 equal", ""},
     {"19 20 21", "2 roll 22 equalverify depth 2 equal", ""},
     {"nop", "rot 1", ""},
     {"nop", "1 rot 1", ""},
     {"nop", "1 2 rot 1", ""},
     {"nop", "0 1 2 rot", ""},
     {"nop", "swap 1", ""},
     {"1", "swap 1", ""},
     {"0 1", "swap 1 equalverify", ""},
     {"nop", "tuck 1", ""},
     {"1", "tuck 1", ""},
     {"1 0", "tuck depth 3 equalverify swap drop2", ""},
     {"nop", "dup2 1", ""},
     {"1", "dup2 1", ""},
     {"nop", "dup3 1", ""},
     {"1", "dup3 1", ""},
     {"1 2", "dup3 1", ""},
     {"nop", "over2 1", ""},
     {"1", "2 3 over2 1", ""},
     {"nop", "swap2 1", ""},
     {"1", "2 3 swap2 1", ""},

     // Disabled.
     {"'a' 'b'", "cat", "cat disabled"},
     {"'a' 'b' 0", "if cat else 1 endif", "cat disabled"},
     {"'abc' 1 1", "substr", "substr disabled"},
     {"'abc' 1 1 0", "if substr else 1 endif", "substr disabled"},
     {"'abc' 2 0", "if left else 1 endif", "left disabled"},
     {"'abc' 2 0", "if right else 1 endif", "right disabled"},

     {"nop", "size 1", ""},

     // Disabled.
     {"'abc'", "if invert else 1 endif", "invert disabled"},
     {"1 2 0 if and else 1 endif", "nop", "and disabled"},
     {"1 2 0 if or else 1 endif", "nop", "or disabled"},
     {"1 2 0 if xor else 1 endif", "nop", "xor disabled"},
     {"2 0 if 2mul else 1 endif", "nop", "2mul disabled"},
     {"2 0 if 2div else 1 endif", "nop", "2div disabled"},
     {"2 2 0 if mul else 1 endif", "nop", "mul disabled"},
     {"2 2 0 if div else 1 endif", "nop", "div disabled"},
     {"2 2 0 if mod else 1 endif", "nop", "mod disabled"},
     {"2 2 0 if lshift else 1 endif", "nop", "lshift disabled"},
     {"2 2 0 if rshift else 1 endif", "nop", "rshift disabled"},

     {"0 1", "equal", ""},
     {"1 1 add", "0 equal", ""},
     {"11 1 add 12 sub", "11 equal", ""},
     {"2147483648 0 add", "nop",
      "arithmetic operands must be in range [-2^31...2^31] "},
     {"-2147483648 0 add", "nop",
      "arithmetic operands must be in range [-2^31...2^31] "},
     {"2147483647 dup add", "4294967294 numequal",
      "numequal must be in numeric range"},
     {"'abcdef' not", "0 equal", "not is an arithmetic operand"},

     // Disabled.
     {"2 dup mul", "4 equal", "mul disabled"},
     {"2 dup div", "1 equal", "div disabled"},
     {"2 2mul", "4 equal", "2mul disabled"},
     {"2 2div", "1 equal", "2div disabled"},
     {"7 3 mod", "1 equal", "mod disabled"},
     {"2 2 lshift", "8 equal", "lshift disabled"},
     {"2 1 rshift", "1 equal", "rshift disabled"},

     {"1", "nop1 nop2 nop3 nop4 nop5 nop6 nop7 nop8 nop9 nop10 2 equal", ""},
     {"'nop_1_to_10' nop1 nop2 nop3 nop4 nop5 nop6 nop7 nop8 nop9 nop10",
      "'nop_1_to_11' equal", ""},

     // Reserved.
     {"0x50", "1", "opcode 0x50 is reserved"},

     {"1", "if 0xba else 1 endif", "opcode 0xba invalid if executed"},
     {"1", "if 0xbb else 1 endif", "opcode 0xbb invalid if executed"},
     {"1", "if 0xbc else 1 endif", "opcode 0xbc invalid if executed"},
     {"1", "if 0xbd else 1 endif", "opcode 0xbd invalid if executed"},
     {"1", "if 0xbe else 1 endif", "opcode 0xbe invalid if executed"},
     {"1", "if 0xbf else 1 endif", "opcode 0xbf invalid if executed"},
     {"1", "if 0xc0 else 1 endif", "opcode 0xc0 invalid if executed"},
     {"1", "if 0xc1 else 1 endif", "opcode 0xc1 invalid if executed"},
     {"1", "if 0xc2 else 1 endif", "opcode 0xc2 invalid if executed"},
     {"1", "if 0xc3 else 1 endif", "opcode 0xc3 invalid if executed"},
     {"1", "if 0xc4 else 1 endif", "opcode 0xc4 invalid if executed"},
     {"1", "if 0xc5 else 1 endif", "opcode 0xc5 invalid if executed"},
     {"1", "if 0xc6 else 1 endif", "opcode 0xc6 invalid if executed"},
     {"1", "if 0xc7 else 1 endif", "opcode 0xc7 invalid if executed"},
     {"1", "if 0xc8 else 1 endif", "opcode 0xc8 invalid if executed"},
     {"1", "if 0xc9 else 1 endif", "opcode 0xc9 invalid if executed"},
     {"1", "if 0xca else 1 endif", "opcode 0xca invalid if executed"},
     {"1", "if 0xcb else 1 endif", "opcode 0xcb invalid if executed"},
     {"1", "if 0xcc else 1 endif", "opcode 0xcc invalid if executed"},
     {"1", "if 0xcd else 1 endif", "opcode 0xcd invalid if executed"},
     {"1", "if 0xce else 1 endif", "opcode 0xce invalid if executed"},
     {"1", "if 0xcf else 1 endif", "opcode 0xcf invalid if executed"},
     {"1", "if 0xd0 else 1 endif", "opcode 0xd0 invalid if executed"},
     {"1", "if 0xd1 else 1 endif", "opcode 0xd1 invalid if executed"},
     {"1", "if 0xd2 else 1 endif", "opcode 0xd2 invalid if executed"},
     {"1", "if 0xd3 else 1 endif", "opcode 0xd3 invalid if executed"},
     {"1", "if 0xd4 else 1 endif", "opcode 0xd4 invalid if executed"},
     {"1", "if 0xd5 else 1 endif", "opcode 0xd5 invalid if executed"},
     {"1", "if 0xd6 else 1 endif", "opcode 0xd6 invalid if executed"},
     {"1", "if 0xd7 else 1 endif", "opcode 0xd7 invalid if executed"},
     {"1", "if 0xd8 else 1 endif", "opcode 0xd8 invalid if executed"},
     {"1", "if 0xd9 else 1 endif", "opcode 0xd9 invalid if executed"},
     {"1", "if 0xda else 1 endif", "opcode 0xda invalid if executed"},
     {"1", "if 0xdb else 1 endif", "opcode 0xdb invalid if executed"},
     {"1", "if 0xdc else 1 endif", "opcode 0xdc invalid if executed"},
     {"1", "if 0xdd else 1 endif", "opcode 0xdd invalid if executed"},
     {"1", "if 0xde else 1 endif", "opcode 0xde invalid if executed"},
     {"1", "if 0xdf else 1 endif", "opcode 0xdf invalid if executed"},
     {"1", "if 0xe0 else 1 endif", "opcode 0xe0 invalid if executed"},
     {"1", "if 0xe1 else 1 endif", "opcode 0xe1 invalid if executed"},
     {"1", "if 0xe2 else 1 endif", "opcode 0xe2 invalid if executed"},
     {"1", "if 0xe3 else 1 endif", "opcode 0xe3 invalid if executed"},
     {"1", "if 0xe4 else 1 endif", "opcode 0xe4 invalid if executed"},
     {"1", "if 0xe5 else 1 endif", "opcode 0xe5 invalid if executed"},
     {"1", "if 0xe6 else 1 endif", "opcode 0xe6 invalid if executed"},
     {"1", "if 0xe7 else 1 endif", "opcode 0xe7 invalid if executed"},
     {"1", "if 0xe8 else 1 endif", "opcode 0xe8 invalid if executed"},
     {"1", "if 0xe9 else 1 endif", "opcode 0xe9 invalid if executed"},
     {"1", "if 0xea else 1 endif", "opcode 0xea invalid if executed"},
     {"1", "if 0xeb else 1 endif", "opcode 0xeb invalid if executed"},
     {"1", "if 0xec else 1 endif", "opcode 0xec invalid if executed"},
     {"1", "if 0xed else 1 endif", "opcode 0xed invalid if executed"},
     {"1", "if 0xee else 1 endif", "opcode 0xee invalid if executed"},
     {"1", "if 0xef else 1 endif", "opcode 0xef invalid if executed"},
     {"1", "if 0xf0 else 1 endif", "opcode 0xf0 invalid if executed"},
     {"1", "if 0xf1 else 1 endif", "opcode 0xf1 invalid if executed"},
     {"1", "if 0xf2 else 1 endif", "opcode 0xf2 invalid if executed"},
     {"1", "if 0xf3 else 1 endif", "opcode 0xf3 invalid if executed"},
     {"1", "if 0xf4 else 1 endif", "opcode 0xf4 invalid if executed"},
     {"1", "if 0xf5 else 1 endif", "opcode 0xf5 invalid if executed"},
     {"1", "if 0xf6 else 1 endif", "opcode 0xf6 invalid if executed"},
     {"1", "if 0xf7 else 1 endif", "opcode 0xf7 invalid if executed"},
     {"1", "if 0xf8 else 1 endif", "opcode 0xf8 invalid if executed"},
     {"1", "if 0xf9 else 1 endif", "opcode 0xf9 invalid if executed"},
     {"1", "if 0xfa else 1 endif", "opcode 0xfa invalid if executed"},
     {"1", "if 0xfb else 1 endif", "opcode 0xfb invalid if executed"},
     {"1", "if 0xfc else 1 endif", "opcode 0xfc invalid if executed"},
     {"1", "if 0xfd else 1 endif", "opcode 0xfd invalid if executed"},
     {"1", "if 0xfe else 1 endif", "opcode 0xfe invalid if executed"},
     {"1", "if 0xff else 1 endif", "opcode 0xff invalid if executed"},
     {"1 if 1 else", "0xff endif",
      "invalid because scriptsig and scriptpubkey are processed separately"},
     {"nop", "ripemd160", ""},
     {"nop", "sha1", ""},
     {"nop", "sha256", ""},
     {"nop", "hash160", ""},
     {"nop", "hash256", ""},
     {"nop",
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbb'",
      ">520 byte push"},
     {"0",
      "if "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbb' endif 1",
      ">520 byte push in non-executed if branch"},
     {"1",
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61",
      ">201 opcodes executed. 0x61 is nop"},
     {"0",
      "if 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 endif 1",
      ">201 opcodes including non-executed if branch. 0x61 is nop"},
     {"1 2 3 4 5 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f",
      "1 2 3 4 5 6 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f",
      ">1,000 stack size (0x6f is dup3)"},
     {"1 2 3 4 5 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f",
      "1 toaltstack 2 toaltstack 3 4 5 6 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f",
      ">1,000 stack+altstack size"},
     {"nop",
      "0 "
      "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaa' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' "
      "'bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      "bbbbbbbbbbbbbbbbb' 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f "
      "0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f 0x6f dup2 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 "
      "0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61 0x61",
      "10,001-byte scriptpubkey"},
     {"nop1", "nop10", ""},

     // Reserved.
     {"1", "ver", "op_ver is reserved"},
     {"1", "verif", "op_verif is reserved (no, it's disabled)"},
     {"1", "vernotif", "op_vernotif is reserved (no, it's disabled)"},
     {"1", "reserved", "op_reserved is reserved"},
     {"1", "reserved1", "op_reserved1 is reserved"},
     {"1", "reserved2", "op_reserved2 is reserved"},

     {"1", "0xba", "0xba == op_nop10 + 1"},
     {"2147483648", "add1 1", "we cannot do math on 5-byte integers"},
     {"2147483648", "negate 1", "we cannot do math on 5-byte integers"},
     {"-2147483648", "add1 1",
      "because we use a sign bit, -2147483648 is also 5 bytes"},
     {"2147483647", "add1 sub1 1",
      "we cannot do math on 5-byte integers, even if the result is 4-bytes"},
     {"2147483648", "sub1 1",
      "we cannot do math on 5-byte integers, even if the result is 4-bytes"},
     {"1", "1 endif", "endif without if"},
     {"1", "if 1", "if without endif"},
     {"1 if 1", "endif", "ifs don't carry over"},
     {"nop", "if 1 endif",
      "the following tests check the if(stack.size() < n) tests in each "
      "opcode"},
     {"nop", "notif 1 endif", "they are here to catch copy-and-paste errors"},
     {"nop", "verify 1", "most of them are duplicated elsewhere,"},
     {"nop", "toaltstack 1", "but, hey, more is always better, right?"},
     {"1", "fromaltstack", ""},
     {"1", "drop2 1", ""},
     {"1", "dup2", ""},
     {"1 1", "dup3", ""},
     {"1 1 1", "over2", ""},
     {"1 1 1 1 1", "rot2", ""},
     {"1 1 1", "swap2", ""},
     {"nop", "ifdup 1", ""},
     {"nop", "drop 1", ""},
     {"nop", "dup 1", ""},
     {"1", "nip", ""},
     {"1", "over", ""},
     {"1 1 1 3", "pick", ""},
     {"0", "pick 1", ""},
     {"1 1 1 3", "roll", ""},
     {"0", "roll 1", ""},
     {"1 1", "rot", ""},
     {"1", "swap", ""},
     {"1", "tuck", ""},
     {"nop", "size 1", ""},
     {"1", "equal 1", ""},
     {"1", "equalverify 1", ""},
     {"nop", "add1 1", ""},
     {"nop", "sub1 1", ""},
     {"nop", "negate 1", ""},
     {"nop", "abs 1", ""},
     {"nop", "not 1", ""},
     {"nop", "nonzero 1", ""},
     {"1", "add", ""},
     {"1", "sub", ""},
     {"1", "booland", ""},
     {"1", "boolor", ""},
     {"1", "numequal", ""},
     {"1", "numequalverify 1", ""},
     {"1", "numnotequal", ""},
     {"1", "lessthan", ""},
     {"1", "greaterthan", ""},
     {"1", "lessthanorequal", ""},
     {"1", "greaterthanorequal", ""},
     {"1", "min", ""},
     {"1", "max", ""},
     {"1 1", "within", ""},
     {"nop", "ripemd160 1", ""},
     {"nop", "sha1 1", ""},
     {"nop", "sha256 1", ""},
     {"nop", "hash160 1", ""},
     {"nop", "hash256 1", ""},
     {"0x00", "'00' equal", "basic op_0 execution"}}};

// These parse errors cannot happen in the libbitcoin syntax.
// Minimal encoding is not required for input (or consensus).
const script_test_list not_invalid_parse_scripts{{
    {"[1.01]", "0x01 nop", "PUSHDATA1 with not enough bytes"},
    ////{ "0x4c01", "0x01 nop", "PUSHDATA1 with not enough bytes" },
    {"[2.0200ff]", "0x01 nop", "PUSHDATA2 with not enough bytes"},
    ////{ "0x4d0200ff", "0x01 nop", "PUSHDATA2 with not enough bytes" },
    {"[4.03000000ffff]", "0x01 nop", "PUSHDATA4 with not enough bytes"}
    ////{ "0x4e03000000ffff", "0x01 nop", "PUSHDATA4 with not enough bytes" }
}};

// On the other hand, syntactic errors can happen (odd hex digits).
const script_test_list invalid_syntax_scripts{{
    {"[1.1]", "0x01 nop", "PUSHDATA1 with odd hex digits"},
    {"[2.200ff]", "0x01 nop", "PUSHDATA2 with odd hex digits"},
    {"[4.3000000ffff]", "0x01 nop", "PUSHDATA4with odd hex digits"},
}};

const script_test_list valid_push_data_scripts{{
    {"[1." + std::string(size_t(2 * 255), 'f') + "]", "0x01 nop",
     "PUSHDATA1 with max hex digits"},
}};

// And push_data overflow can happen.
const script_test_list invalid_overflowed_push_data_scripts{
    {{"[1." + std::string(size_t(2 * 256), 'f') + "]", "0x01 nop",
      "PUSHDATA1 with max + 1 hex digits"}}};

#endif
