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
#include <bitcoin/system/chain/witness.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <numeric>
#include <string>
#include <utility>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/data/collection.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/data/string.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/iostream/iostream.hpp>
#include <bitcoin/system/machine/operation.hpp>
#include <bitcoin/system/machine/program.hpp>
#include <bitcoin/system/message/messages.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

using namespace bc::system::machine;
static const auto checksig_script = script{ { opcode::checksig } };

// Constructors.
//-----------------------------------------------------------------------------

// A default instance is invalid (until modified).
witness::witness()
  : valid_(false)
{
}

witness::witness(witness&& other)
  : stack_(std::move(other.stack_)), valid_(other.valid_)
{
}

witness::witness(const witness& other)
  : stack_(other.stack_), valid_(other.valid_)
{
}

witness::witness(const data_stack& stack)
{
    stack_ = stack;
}

witness::witness(data_stack&& stack)
{
    stack_ = std::move(stack);
}

witness::witness(data_chunk&& encoded, bool prefix)
{
    from_data(encoded, prefix);
}

witness::witness(const data_chunk& encoded, bool prefix)
{
    from_data(encoded, prefix);
}

// Operators.
//-----------------------------------------------------------------------------

witness& witness::operator=(witness&& other)
{
    reset();
    stack_ = std::move(other.stack_);
    valid_ = other.valid_;
    return *this;
}

witness& witness::operator=(const witness& other)
{
    reset();
    stack_ = other.stack_;
    valid_ = other.valid_;
    return *this;
}

bool witness::operator==(const witness& other) const
{
    return stack_ == other.stack_;
}

bool witness::operator!=(const witness& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
witness witness::factory(const data_chunk& encoded, bool prefix)
{
    witness instance;
    instance.from_data(encoded, prefix);
    return instance;
}

// static
witness witness::factory(std::istream& stream, bool prefix)
{
    witness instance;
    instance.from_data(stream, prefix);
    return instance;
}

// static
witness witness::factory(reader& source, bool prefix)
{
    witness instance;
    instance.from_data(source, prefix);
    return instance;
}

bool witness::from_data(const data_chunk& encoded, bool prefix)
{
    data_source istream(encoded);
    return from_data(istream, prefix);
}

bool witness::from_data(std::istream& stream, bool prefix)
{
    istream_reader source(stream);
    return from_data(source, prefix);
}

// Prefixed data assumed valid here though caller may confirm with is_valid.
bool witness::from_data(reader& source, bool prefix)
{
    reset();
    valid_ = true;

    const auto read_element = [](reader& source)
    {
        // Tokens encoded as variable integer prefixed byte array (bip144).
        const auto size = source.read_size_little_endian();

        // The script and push data size limits are removed (bip-taproot).
        if (size > max_block_weight)
        {
            source.invalidate();
            return data_chunk{};
        }

        return source.read_bytes(size);
    };

    // TODO: optimize store serialization to avoid loop, reading data directly.
    if (prefix)
    {
        // Witness prefix is an element count, not byte length (unlike script).
        // On wire each witness is prefixed with number of elements (bip144).
        for (auto count = source.read_size_little_endian(); count > 0; --count)
             stack_.push_back(read_element(source));
    }
    else
    {
        while (!source.is_exhausted())
            stack_.push_back(read_element(source));
    }

    if (!source)
        reset();

    return source;
}

// private/static
size_t witness::serialized_size(const data_stack& stack)
{
    const auto sum = [](size_t total, const data_chunk& element)
    {
        // Tokens encoded as variable integer prefixed byte array (bip144).
        const auto size = element.size();
        return total + message::variable_uint_size(size) + size;
    };

    return std::accumulate(stack.begin(), stack.end(), size_t(0), sum);
}

// protected
void witness::reset()
{
    valid_ = false;
    stack_.clear();
    stack_.shrink_to_fit();
}

bool witness::is_valid() const
{
    // Witness validity is consistent with stack validity (unlike script).
    return valid_;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk witness::to_data(bool prefix) const
{
    data_chunk data;
    const auto size = serialized_size(prefix);
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream, prefix);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void witness::to_data(std::ostream& stream, bool prefix) const
{
    ostream_writer sink(stream);
    to_data(sink, prefix);
}

void witness::to_data(writer& sink, bool prefix) const
{
    // Witness prefix is an element count, not byte length (unlike script).
    if (prefix)
        sink.write_variable_little_endian(stack_.size());

    const auto serialize = [&sink](const data_chunk& element)
    {
        // Tokens encoded as variable integer prefixed byte array (bip144).
        sink.write_variable_little_endian(element.size());
        sink.write_bytes(element);
    };

    // TODO: optimize store serialization to avoid loop, writing data directly.
    std::for_each(stack_.begin(), stack_.end(), serialize);
}

std::string witness::to_string() const
{
    if (!valid_)
        return "<invalid>";

    std::string text;
    const auto serialize = [&text](const data_chunk& element)
    {
        text += "[" + encode_base16(element) + "] ";
    };

    std::for_each(stack_.begin(), stack_.end(), serialize);
    return trim_copy(text);
}

// Iteration.
//-----------------------------------------------------------------------------
// These are syntactic sugar that allow the caller to iterate stack directly.

void witness::clear()
{
    reset();
}

bool witness::empty() const
{
    return stack_.empty();
}

size_t witness::size() const
{
    return stack_.size();
}

const data_chunk& witness::front() const
{
    BITCOIN_ASSERT(!stack_.empty());
    return stack_.front();
}

const data_chunk& witness::back() const
{
    BITCOIN_ASSERT(!stack_.empty());
    return stack_.back();
}

const data_chunk& witness::operator[](size_t index) const
{
    BITCOIN_ASSERT(index < stack_.size());
    return stack_[index];
}

witness::iterator witness::begin() const
{
    return stack_.begin();
}

witness::iterator witness::end() const
{
    return stack_.end();
}

// Properties.
//-----------------------------------------------------------------------------

size_t witness::serialized_size(bool prefix) const
{
    // Witness prefix is an element count, not a byte length (unlike script).
    return (prefix ? message::variable_uint_size(stack_.size()) : 0u) +
        serialized_size(stack_);
}

const data_stack& witness::stack() const
{
    return stack_;
}

data_chunk witness::annex(bool prefix) const
{
    if (!annexed())
        return {};

    if (prefix)
    {
        // TODO: reserialize and return annex with size prefix.
        return {};
    }
    else
    {
        return stack_.back();
    }
}

bool witness::annexed() const
{
    return is_annexed(stack_);
}

// Utilities.
//-----------------------------------------------------------------------------

// static
bool witness::is_annexed(const data_stack& stack)
{
    static constexpr size_t annex_sentinel = 0x50;

    return stack.size() > 1 && stack.front().front() == annex_sentinel;
}

// static
bool witness::is_push_size(const data_stack& stack)
{
    const auto push_size = [](const data_chunk& element)
    {
        return element.size() <= max_push_data_size;
    };

    return std::all_of(stack.begin(), stack.end(), push_size);
}

// static
// The (only) coinbase witness must be (arbitrary) 32-byte value (bip141).
bool witness::is_reserved_pattern(const data_stack& stack)
{
    return stack.size() == 1 &&
        stack[0].size() == hash_size;
}

// private
// This is an internal optimization over using script::to_pay_key_hash_pattern.
operation::list witness::to_pay_key_hash(data_chunk&& program)
{
    BITCOIN_ASSERT(program.size() == short_hash_size);

    return operation::list
    {
        { opcode::dup },
        { opcode::hash160 },
        { std::move(program) },
        { opcode::equalverify },
        { opcode::checksig }
    };
}

// Extractions.
//-----------------------------------------------------------------------------

inline data_chunk top_element(const data_stack stack)
{
    return stack.empty() ? data_chunk{} : stack.back();
}

// The return script is only useful only for sigop counting.
bool witness::extract_sigop_script(script& out_script,
    const script& program_script, bool native) const
{
    // Caller may recycle script parameter.
    out_script.clear();

    switch (program_script.version())
    {
        case script_version::zero:
        {
            switch (program_script.witness_program().size())
            {
                // Each p2wkh input is counted as 1 sigop (bip141).
                case short_hash_size:
                    out_script = checksig_script;
                    return true;

                // p2wsh sigops are counted as before for p2sh (bip141).
                case hash_size:
                    out_script.from_data(top_element(stack_), false);
                    return true;

                // Undefined v0 witness script, will not validate.
                default:
                    return true;
            }
        }

        // A taproot non-tapscript script has sigops.
        // Sigops are not statically counted (bip-tapscript).
        case script_version::one:
        {
            if (native && program_script.witness_program().size() == hash_size)
            {
                // TODO: 
                // TODO: consider only non-tapscript.
            }

            return true;
        }

        // Remaining paths are reserved for future extensions (bip141).
        case script_version::reserved:
            return true;

        // Return false only if this is not a witness script.
        case script_version::unversioned:
        default:
            return false;
    }
}

// Extract v0 script (or fail) and initial execution stack.
bool witness::extract_version_0_script(script& out_script,
    data_stack& out_stack, const script& program_script) const
{
    auto program = program_script.witness_program();
    out_stack = stack_;

    switch (program.size())
    {
        // p2wkh
        // witness stack : <signature> <public-key>
        // input script  : (empty)
        // output script : <0> <20-byte-hash-of-public-key>
        case short_hash_size:
        {
            // Stack must be 2 elements (bip141).
            if (out_stack.size() != 2)
                return false;

            // Create a pay-to-key-hash input script from the program.
            // The hash160 of public key must match program (bip141).
            out_script.from_operations(to_pay_key_hash(
                std::move(program)));
            return true;
        }

        // p2wsh
        // witness stack : <script> [stack-elements]
        // input script  : (empty)
        // output script : <0> <32-byte-hash-of-script>
        case hash_size:
        {
            // The stack must consist of at least 1 element (bip141).
            if (out_stack.empty())
                return false;

            // Input script is popped from the stack (bip141).
            out_script.from_data(pop(out_stack), false);

            // The sha256 of popped script must match program (bip141).
            return std::equal(program.begin(), program.end(),
                sha256_hash(out_script.to_data(false)).begin());
        }

        // The witness extraction is invalid for v0.
        default:
            return false;
    }
}

static bool is_valid_control_block(const data_chunk& control)
{
    // TODO: compute and enforce control length.
    // TODO: (control - 33) : modulo 32 == 0 && < 32 * 128. 
    // Require 33 + 32m bytes, m is [0...128] (bip-taproot).
    // TODO: implement crypto nonsense from bip-taproot.

    return false;
}

static bool is_valid_script_path(const data_chunk& control,
    const script& script)
{
    // TODO: Validate script against control block (bip-taproot).
    return false;
}

// Extract (v1) tapscript (if indicated) and initial execution stack.
bool witness::extract_taproot(script& out_script,
    data_stack& out_stack, const script& program_script,
    bool bip_tapscript) const
{
    // taproot
    // input script  : (empty)
    // output script : <1> <32-byte-schnorr-public-key>
    // A taproot program is 32 bytes (bip-taproot).
    auto program = program_script.witness_program();

    // Fail if the witness stack has 0 elements (bip-taproot).
    if (out_stack.empty())
        return false;

    // witness stack : <signature> [annex]
    // The annex is ignored except by signature hash (bip-taproot).
    if (is_annexed(out_stack))
        pop(out_stack);

    // A single element stack is a key path taproot spend (bip-taproot).
    if (out_stack.size() == 1)
    {
        // witness stack : <signature> <public-key>
        out_stack.push_back(std::move(program));
        out_script = checksig_script;
        ////out_script.set_taproot();
        return true;
    }

    // A multi-element stack is a script path taproot spend (bip-taproot).
    // witness stack : [stack-elements] <script> <control> [annex]

    // Exclude the control block from the stack (bip-taproot).
    const auto control = pop(out_stack);

    // Validate the control block (bip-taproot).
    if (!is_valid_control_block(control))
        return false;

    // Obtain the script and exclude from the stack (bip-taproot).
    out_script.from_data(pop(out_stack), false);

    // Validate script against the control block (bip-taproot).
    if (!is_valid_script_path(control, out_script))
        return false;

    out_script.set_taproot();

    if (!bip_tapscript)
        return true;

    // TODO: implement tapscript spend.
    // It is a tapscript script path taproot spend... (bip-tapscript).

    out_script.set_tapscript();
    return true;
}

// Validation.
//-----------------------------------------------------------------------------

// static
// The program script is either a prevout script or an embedded script.
// It validates this witness, from which the witness script is derived.
code witness::verify(const transaction& tx, uint32_t input_index,
    uint32_t forks, const script& program_script, uint64_t value,
    bool native) const
{
    code ec;
    script script;
    data_stack stack;
    const auto version = program_script.version();
    const auto bip_taproot = script::is_enabled(forks, rule_fork::bip_taproot_rule);
    const auto bip_tapscript = script::is_enabled(forks, rule_fork::bip_tapscript_rule);

    switch (version)
    {
        // Version 0 (bip141).
        case script_version::zero:
        {
            if (!extract_version_0_script(script, stack, program_script))
                return error::invalid_witness;

            program witness(script, tx, input_index, forks, std::move(stack),
                value, version, false);

            if ((ec = witness.evaluate()))
                return ec;

            // A v0 script must succeed with a clean true stack (bip141).
            return witness.stack_result(true) ? error::success :
                error::stack_false;
        }

        // Version 1 (bip-taproot).
        case script_version::one:
        {
            // Non-taproot outputs remain unencumbered (bip-taproot).
            // Taproot is v1 native with 32 byte witness program (bip-taproot).
            if (!(bip_taproot && native &&
                program_script.witness_program().size() == hash_size))
                return error::success;

            if (!extract_taproot(script, stack, program_script, bip_tapscript))
                return error::invalid_witness;

            program witness(script, tx, input_index, forks, std::move(stack),
                value, version, bip_tapscript);

            if ((ec = witness.evaluate()))
                return ec;

            // A v1 script must succeed with a clean true stack (bip-taproot).
            return witness.stack_result(true) ? ec : error::stack_false;
        }

        // These versions are reserved for future extensions (bip141).
        case script_version::reserved:
            return error::success;

        case script_version::unversioned:
        default:
            return error::operation_failed;
    }
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
