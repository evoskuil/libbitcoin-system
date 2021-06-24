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
#ifndef LIBBITCOIN_SYSTEM_IOSTREAM_SOURCES_COPY_SOURCE_HPP
#define LIBBITCOIN_SYSTEM_IOSTREAM_SOURCES_COPY_SOURCE_HPP

#include <boost/iostreams/stream.hpp>
#include <bitcoin/system/iostream/sources/source.hpp>

namespace libbitcoin {
namespace system {

/// Source for boost::iostreams::stream, copies bytes from Container.
template <typename Container>
class copy_source
  : public base_source<source::tag::copy, Container>
{
public:
    copy_source(const Container& data) noexcept;

    /// detail::random_access direct (seekable)
    sequence input_sequence() noexcept;

    // Should not be necessary for direct seekable devices.
    // The boost stream wrapper can simply look ahead in the buffer.
    // Though it would seem read() is also unnecessary for the same reason.
    /////// peekable_tag
    ////bool putback(char_type) noexcept;

protected:
    virtual void do_read(value_type* to, size_type size) noexcept;

private:
    const typename Container& container_;
    typename Container::const_iterator from_;
};

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/iostream/sources/copy_source.ipp>

#endif
