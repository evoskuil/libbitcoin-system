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
#ifndef LIBBITCOIN_SYSTEM_TEST_LANGUAGES_HPP
#define LIBBITCOIN_SYSTEM_TEST_LANGUAGES_HPP

#include <array>
#include <bitcoin/system.hpp>

 // Avoid using namespace in shared headers, but okay here.
using namespace bc::system::wallet;

constexpr auto ideographic_space = "\xe3\x80\x80";
constexpr auto ascii_space = "\x20";

const size_t test_dictionary_size = 10;
typedef std::array<const char*, test_dictionary_size> test_words;

const test_words test_words_es
{
    {
        "ábaco",
        "abdomen",
        "abeja",
        "abierto",
        "abogado",
        "abono",
        "aborto",
        "abrazo",
        "abrir",
        "abuelo"
    }
};

const test_words test_words_zh_Hans
{
    {
        "的",
        "一",
        "是",
        "在",
        "不",
        "了",
        "有",
        "和",
        "人",
        "这"
    }
};

const test_words test_words_zh_Hant
{
    {
        "的",
        "一",
        "是",
        "在",
        "不",
        "了",
        "有",
        "和",
        "人",
        "這"
    }
};

// languages is a protected class, provide test accesss.
class accessor
  : public languages
{
public:
    accessor()
      : languages(), value_()
    {
    }

    accessor(const languages& other)
      : languages(other), value_()
    {
    }

    accessor(const data_chunk& entropy, const string_list& words,
        language identifier, const std::string& value="")
      : languages(entropy, words, identifier), value_(value)
    {
    }

    // Expose protected method.
    static string_list normalize(const string_list& words)
    {
        return languages::normalize(words);
    }

    // Verify assignment and copy construction with derived member.
    const std::string& value() const
    {
        return value_;
    }

private:
    std::string value_;
};

#endif