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
#ifndef LIBBITCOIN_SYSTEM_WALLET_CONEXT_HPP
#define LIBBITCOIN_SYSTEM_WALLET_CONEXT_HPP

#include <cstdint>
#include <string>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

// See: hd_private::to_prefixes.
// TODO: the paramters are reversed from hd_private.
constexpr uint64_t hd(uint32_t xpub, uint32_t xprv)
{
    return uint64_t(xprv) << 32 | xpub;
};

/// Prefix namespace tree.
namespace prefix
{
    /// Witness address prefixes.
    namespace wit
    {
        // Can be compared during execution to validate a parameter.
        constexpr auto undefined = "";

        // github.com/bitcoin/bips/blob/master/bip-0173.mediawiki
        // github.com/litecoin-project/litecoin/blob/master/src/chainparams.cpp
        namespace main
        {
            constexpr auto btc = "bc";
            constexpr auto ltc = "ltc";
        };
        namespace test
        {
            constexpr auto btc = "tb";
            constexpr auto ltc = "tltc";
        };
    };

    /// WIF key prefix bytes.
    namespace wif
    {
        // Can be compared during execution to validate a parameter.
        constexpr auto undefined = 0xff;

        namespace main
        {
            constexpr auto btc = 0x80;
        };
        namespace test
        {
            constexpr auto btc = 0xef;
        };
    };

    /// Payment addresss version bytes.
    namespace add
    {
        // Can be compared during execution to validate a parameter.
        constexpr auto undefined = 0xff;

        namespace p2pkh
        {
            namespace main
            {
                constexpr auto btc = 0x00;
            };
            namespace test
            {
                constexpr auto btc = 0x6f;
            };
        };
        namespace p2sh
        {
            namespace main
            {
                constexpr auto btc = 0x05;
            };
            namespace test
            {
                constexpr auto btc = 0xc4;
            };
        };
    };

    /// Extended (hd) keys, data from Satoshi Labs registry.
    /// github.com/satoshilabs/slips/blob/master/slip-0132.md
    namespace ext
    {
        // Can be compared during execution to validate a parameter.
        constexpr auto undefined = hd(0xffffffff, 0xffffffff);

        namespace p2pkh
        {
            namespace main
            {
                // { xpub, xprv } m/44'/0'
                constexpr auto btc = hd(0x0488b21e, 0x0488ade4);
                // { Ltub, Ltpv } m/44'/2'
                constexpr auto ltc = hd(0x019da462, 0x019d9cfe);
                // { vtcp, vtcv } m/44'/28'
                constexpr auto vtc = hd(0x0488b21e, 0x0488ade4);
                // { ppub, pprv } m/44'/1997'
                constexpr auto pol = hd(0x03e25d7e, 0x03e25945);
            };
            namespace test
            {
                // { tpub, tprv } m/44'/1'
                constexpr auto btc = hd(0x043587cf, 0x04358394);
                // { ttub, ttpv } m/44'/1'
                constexpr auto ltc = hd(0x0436f6e1, 0x0436ef7d);
            };
        };
        namespace p2sh
        {
            namespace main
            {
                constexpr auto btc = p2pkh::main::btc;
                constexpr auto ltc = p2pkh::main::ltc;
                constexpr auto vtc = p2pkh::main::vtc;
            };
            namespace test
            {
                constexpr auto btc = p2pkh::test::btc;
            };
            namespace p2wpkh
            {
                namespace main
                {
                    // { ypub, yprv } m/49'/0'
                    constexpr auto btc = hd(0x049d7cb2, 0x049d7878);
                    // { Mtub, Mtpv } m/49'/1'
                    constexpr auto ltc = hd(0x01b26ef6, 0x01b26792);
                };
                namespace test
                {
                    // { upub, uprv } m/49'/1'
                    constexpr auto btc = hd(0x044a5262, 0x044a4e28);
                };
            };
            namespace p2wsh
            {
                namespace multisig
                {
                    namespace main
                    {
                        // { Ypub, Yprv }
                        constexpr auto btc = hd(0x0295b43f, 0x0295b005);
                    };
                    namespace test
                    {
                        // { Upub, Uprv }
                        constexpr auto btc = hd(0x024289ef, 0x024285b5);
                    };
                };
            };
        };
        namespace p2wpkh
        {
            namespace main
            {
                // { zpub, zprv } m/84'/0'
                constexpr auto btc = hd(0x04b24746, 0x04b2430c);
                // { zpub, zprv } m/84'/57'
                constexpr auto sys = btc;
            };
            namespace test
            {
                // { vpub, vprv } m/84'/1'
                constexpr auto btc = hd(0x045f1cf6, 0x045f18bc);
            };
        };
        namespace p2wsh
        {
            namespace multisig
            {
                namespace main
                {
                    // { Zpub, Zprv }
                    constexpr auto btc = hd(0x02aa7ed3, 0x02aa7a99);
                    constexpr auto sys = btc;
                };
                namespace test
                {
                    // { Vpub, Vprv }
                    constexpr auto btc = hd(0x02575483, 0x02575048);
                };
            };
        };
    };
};

/// Portable context for keys and addresses.
struct BC_API context
{
    /// Hierarchical-deterministic public/private key prefixes.
    uint64_t hd_prefixes = prefix::ext::undefined;

    /// Payment (legacy address) address version byte.
    uint8_t address_version = prefix::add::undefined;

    /// WIF (legacy private key) version byte.
    uint16_t wif_prefix = prefix::wif::undefined;

    /// Witness address prefix.
    const char* witness_prefix = prefix::wit::undefined;
};

/// Predefined wallet contexts.
/// Custom contexts can also be defined by simply populating a context struct.
namespace ctx
{
    namespace btc
    {
        namespace main
        {
            constexpr context p2pkh
            {
                prefix::ext::p2pkh::main::btc,
                prefix::add::p2pkh::main::btc,
                prefix::wif::main::btc,
                prefix::wit::main::btc
            };
            constexpr context p2sh
            {
                prefix::ext::p2sh::main::btc,
                prefix::add::p2sh::main::btc,
                prefix::wif::main::btc,
                prefix::wit::main::btc
            };
        };
        namespace test
        {
            constexpr context p2pkh
            {
                prefix::ext::p2pkh::test::btc,
                prefix::add::p2pkh::test::btc,
                prefix::wif::test::btc,
                prefix::wit::test::btc
            };
            constexpr context p2sh
            {
                prefix::ext::p2sh::test::btc,
                prefix::add::p2sh::test::btc,
                prefix::wif::test::btc,
                prefix::wit::test::btc
            };
        }
    };
    namespace ltc
    {
        namespace main
        {
            constexpr context p2pkh
            {
                prefix::ext::p2pkh::main::ltc,
                prefix::add::undefined,
                prefix::wif::undefined,
                prefix::wit::main::ltc
            };
            constexpr context p2sh
            {
                prefix::ext::p2sh::main::ltc,
                prefix::add::undefined,
                prefix::wif::undefined,
                prefix::wit::main::ltc
            };
        };
        namespace test
        {
            constexpr context p2pkh
            {
                prefix::ext::p2pkh::test::ltc,
                prefix::add::undefined,
                prefix::wif::undefined,
                prefix::wit::test::ltc
            };
            constexpr context p2sh
            {
                prefix::ext::undefined,
                prefix::add::undefined,
                prefix::wif::undefined,
                prefix::wit::test::ltc
            };
        }
    };
};

/// Predefined contexts for use as default parameter values.
extern const context btc_mainnet_p2kh;
extern const context btc_mainnet_p2sh;
extern const context btc_testnet_p2kh;
extern const context btc_testnet_p2sh;

extern const context btc_mainnet;
extern const context btc_testnet;

extern const context btc;

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
