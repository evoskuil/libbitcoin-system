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
#include <bitcoin/system/wallet/addresses/qr_matrix.hpp>

#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/wallet/addresses/qr_encoder.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

BC_PUSH_WARNING(NO_THROW_IN_NOEXCEPT)
BC_PUSH_WARNING(NO_ARRAY_INDEXING)

static constexpr size_t absolute_difference(size_t left, size_t right) NOEXCEPT
{
    return left > right ? left - right : right - left;
}

// BCH remainder of value shifted by degree, modulo generator.
static constexpr uint32_t bch_remainder(uint32_t value, uint32_t generator,
    size_t degree) NOEXCEPT
{
    auto result = value;
    for (size_t bit = 0; bit < degree; ++bit)
        result = (result << 1) ^ ((result >> sub1(degree)) * generator);

    return result;
}

static std_vector<size_t> alignments(uint8_t version, size_t width) NOEXCEPT
{
    if (version < 2u)
        return {};

    const size_t count = version / 7u + 2u;
    const auto step = version == 32u ? 26u :
        (version * 4u + count * 2u + 1u) / (count * 2u - 2u) * 2u;

    std_vector<size_t> positions(count, 6u);
    for (size_t index = 1; index < count; ++index)
        positions[index] = width - 7u - (count - add1(index)) * step;

    return positions;
}

// public
// ----------------------------------------------------------------------------

size_t qr_matrix::width(uint8_t version) NOEXCEPT
{
    if (version < qr_encoder::minimum_version ||
        version > qr_encoder::maximum_version)
        return zero;

    return 17u + 4u * version;
}

data_chunk qr_matrix::encode(const data_chunk& codewords, uint8_t version,
    recovery_level level) NOEXCEPT
{
    const auto size = width(version);
    if (is_zero(size) || codewords.size() != qr_encoder::codewords(version))
        return {};

    auto frame = to_frame(version);
    place(frame, size, codewords);

    const auto area = size * size;
    auto minimum = max_size_t;
    data_chunk masked{};
    data_chunk best{};

    for (size_t mask = 0; mask < masks; ++mask)
    {
        apply_mask(masked, frame, size, mask);
        write_format(masked, size, level, mask);

        const auto darks = to_unsigned(std::count_if(masked.begin(),
            masked.end(), [](uint8_t module) NOEXCEPT
            {
                return to_bool(module & dark);
            }));

        const auto ratio = (200u * darks + area) / area / 2u;
        const auto score = (absolute_difference(ratio, 50u) / 5u) * 10u +
            penalty(masked, size);

        if (score < minimum)
        {
            minimum = score;
            best = masked;
        }
    }

    for (auto& module: best)
        module &= dark;

    return best;
}

// protected
// ----------------------------------------------------------------------------

data_chunk qr_matrix::to_frame(uint8_t version) NOEXCEPT
{
    const auto size = width(version);
    data_chunk frame(size * size, light);

    const auto set = [&](size_t row, size_t column, bool on) NOEXCEPT
    {
        frame[row * size + column] = reserved | (on ? dark : light);
    };

    // Finder patterns and separators.
    const auto finder = [&](size_t row, size_t column) NOEXCEPT
    {
        const auto top = row < 4u ? zero : row - 4u;
        const auto left = column < 4u ? zero : column - 4u;
        const auto bottom = std::min(row + 4u, sub1(size));
        const auto right = std::min(column + 4u, sub1(size));

        for (auto y = top; y <= bottom; ++y)
        {
            for (auto x = left; x <= right; ++x)
            {
                const auto distance = std::max(absolute_difference(y, row),
                    absolute_difference(x, column));
                set(y, x, distance != 2u && distance != 4u);
            }
        }
    };

    finder(3u, 3u);
    finder(3u, size - 4u);
    finder(size - 4u, 3u);

    // Format information areas.
    for (size_t index = 0; index < 9u; ++index)
    {
        set(8u, index, false);
        set(index, 8u, false);
    }

    for (size_t index = 0; index < 8u; ++index)
    {
        set(8u, size - 8u + index, false);
        set(size - 8u + index, 8u, false);
    }

    // Timing patterns.
    for (size_t index = 8; index < size - 8u; ++index)
    {
        set(6u, index, is_even(index));
        set(index, 6u, is_even(index));
    }

    // Alignment patterns.
    const auto positions = alignments(version, size);
    const auto count = positions.size();
    for (size_t row = 0; row < count; ++row)
    {
        for (size_t column = 0; column < count; ++column)
        {
            if ((is_zero(row) && is_zero(column)) ||
                (is_zero(row) && column == sub1(count)) ||
                (row == sub1(count) && is_zero(column)))
                continue;

            const auto y = positions[row];
            const auto x = positions[column];
            for (auto dy = y - 2u; dy <= y + 2u; ++dy)
                for (auto dx = x - 2u; dx <= x + 2u; ++dx)
                    set(dy, dx, std::max(absolute_difference(dy, y),
                        absolute_difference(dx, x)) != 1u);
        }
    }

    // Version information.
    if (version >= 7u)
    {
        const auto bits = version_bits(version);
        for (size_t bit = 0; bit < 18u; ++bit)
        {
            const auto on = get_right(bits, bit);
            const auto short_side = bit / 3u;
            const auto long_side = size - 11u + bit % 3u;
            set(short_side, long_side, on);
            set(long_side, short_side, on);
        }
    }

    // Dark module.
    set(size - 8u, 8u, true);
    return frame;
}

uint32_t qr_matrix::format_bits(recovery_level level, size_t mask) NOEXCEPT
{
    uint32_t indicator{};
    switch (level)
    {
        case recovery_level::low:
            indicator = 1;
            break;
        case recovery_level::medium:
            indicator = 0;
            break;
        case recovery_level::high:
            indicator = 3;
            break;
        default:
        case recovery_level::highest:
            indicator = 2;
            break;
    }

    const auto data = (indicator << 3) | possible_narrow_cast<uint32_t>(mask);
    return ((data << 10) | bch_remainder(data, 0x0537, 10)) ^ 0x5412;
}

uint32_t qr_matrix::version_bits(uint8_t version) NOEXCEPT
{
    return (uint32_t{ version } << 12) | bch_remainder(version, 0x1f25, 12);
}

void qr_matrix::place(data_chunk& frame, size_t width,
    const data_chunk& codewords) NOEXCEPT
{
    const auto bits = to_bits(codewords.size());
    size_t bit{};

    for (size_t pair = 0; pair < to_half(width); ++pair)
    {
        auto right = sub1(width) - pair * two;
        if (right <= 6u)
            --right;

        const auto upward = !get_right(add1(right), one);
        for (size_t step = 0; step < width; ++step)
        {
            const auto row = upward ? sub1(width) - step : step;
            for (size_t offset = 0; offset < two; ++offset)
            {
                auto& module = frame[row * width + right - offset];
                if (to_bool(module & reserved) || bit >= bits)
                    continue;

                const auto byte = codewords[bit / byte_bits];
                const auto shift = sub1(byte_bits) - bit % byte_bits;
                module = get_right(byte, shift) ? dark : light;
                ++bit;
            }
        }
    }
}

bool qr_matrix::is_masked(size_t mask, size_t row, size_t column) NOEXCEPT
{
    const auto product = row * column;
    switch (mask)
    {
        case 0:
            return is_even(row + column);
        case 1:
            return is_even(row);
        case 2:
            return is_zero(column % 3u);
        case 3:
            return is_zero((row + column) % 3u);
        case 4:
            return is_even(row / 2u + column / 3u);
        case 5:
            return is_zero((product % 2u) + (product % 3u));
        case 6:
            return is_even((product % 2u) + (product % 3u));
        default:
        case 7:
            return is_even((product % 3u) + ((row + column) % 2u));
    }
}

void qr_matrix::apply_mask(data_chunk& out, const data_chunk& frame,
    size_t width, size_t mask) NOEXCEPT
{
    out = frame;
    for (size_t row = 0; row < width; ++row)
    {
        for (size_t column = 0; column < width; ++column)
        {
            auto& module = out[row * width + column];
            if (!to_bool(module & reserved) && is_masked(mask, row, column))
                module ^= dark;
        }
    }
}

void qr_matrix::write_format(data_chunk& frame, size_t width,
    recovery_level level, size_t mask) NOEXCEPT
{
    const auto bits = format_bits(level, mask);
    const auto set = [&](size_t row, size_t column, size_t bit) NOEXCEPT
    {
        frame[row * width + column] = reserved |
            (get_right(bits, bit) ? dark : light);
    };

    for (size_t bit = 0; bit < 8u; ++bit)
    {
        set(8u, sub1(width) - bit, bit);
        set(bit < 6u ? bit : add1(bit), 8u, bit);
    }

    for (size_t bit = 0; bit < 7u; ++bit)
    {
        set(width - 7u + bit, 8u, bit + 8u);
        set(8u, is_zero(bit) ? 7u : 6u - bit, bit + 8u);
    }
}

size_t qr_matrix::penalty(const data_chunk& modules, size_t width) NOEXCEPT
{
    const auto at = [&](size_t row, size_t column) NOEXCEPT
    {
        return to_bool(modules[row * width + column] & dark);
    };

    size_t total{};
    for (size_t row = 1; row < width; ++row)
    {
        for (size_t column = 1; column < width; ++column)
        {
            const auto module = at(row, column);
            if (module == at(row, sub1(column)) &&
                module == at(sub1(row), column) &&
                module == at(sub1(row), sub1(column)))
                total += 3u;
        }
    }

    // Runs alternate from light, so a leading dark run follows an empty run.
    std_vector<size_t> runs{};
    runs.reserve(add1(width));
    const auto scan = [&](bool rows) NOEXCEPT
    {
        for (size_t line = 0; line < width; ++line)
        {
            runs.assign(one, zero);
            auto previous = false;
            for (size_t index = 0; index < width; ++index)
            {
                const auto module = rows ? at(line, index) : at(index, line);
                if (module == previous)
                {
                    ++runs.back();
                }
                else
                {
                    runs.push_back(one);
                    previous = module;
                }
            }

            total += penalty_runs(runs);
        }
    };

    scan(true);
    scan(false);
    return total;
}

size_t qr_matrix::penalty_runs(const std_vector<size_t>& runs) NOEXCEPT
{
    const auto count = runs.size();

    size_t total{};
    for (size_t index = 0; index < count; ++index)
    {
        const auto run = runs[index];
        if (run >= 5u)
            total += 3u + (run - 5u);

        if (is_even(index) || index < 3u || index + 2u >= count ||
            !is_zero(run % 3u))
            continue;

        const auto unit = run / 3u;
        if (runs[index - 2u] == unit && runs[index - 1u] == unit &&
            runs[index + 1u] == unit && runs[index + 2u] == unit &&
            (index == 3u || runs[index - 3u] >= 4u * unit ||
            index + 4u >= count || runs[index + 3u] >= 4u * unit))
            total += 40u;
    }

    return total;
}

BC_POP_WARNING()
BC_POP_WARNING()

} // namespace wallet
} // namespace system
} // namespace libbitcoin
