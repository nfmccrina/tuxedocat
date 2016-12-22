/*
* The MIT License (MIT)
*
* Copyright (c) 2016 Nathan McCrina
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
* NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
* USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../include/LookupData.hpp"

const uint64_t TuxedoCat::LookupData::knightAttacks[64] = {
    0x0000000000020400ULL, 0x0000000000050800ULL,
    0x00000000000A1100ULL, 0x0000000000142200ULL,
    0x0000000000284400ULL, 0x0000000000508800ULL,
    0x0000000000A01000ULL, 0x0000000000402000ULL,
    0x0000000002040004ULL, 0x0000000005080008ULL,
    0x000000000A110011ULL, 0x0000000014220022ULL,
    0x0000000028440044ULL, 0x0000000050880088ULL,
    0x00000000A0100010ULL, 0x0000000040200020ULL,
    0x0000000204000402ULL, 0x0000000508000805ULL,
    0x0000000A1100110AULL, 0x0000001422002214ULL,
    0x0000002844004428ULL, 0x0000005088008850ULL,
    0x000000A0100010A0ULL, 0x0000004020002040ULL,
    0x0000020400040200ULL, 0x0000050800080500ULL,
    0x00000A1100110A00ULL, 0x0000142200221400ULL,
    0x0000284400442800ULL, 0x0000508800885000ULL,
    0x0000A0100010A000ULL, 0x0000402000204000ULL,
    0x0002040004020000ULL, 0x0005080008050000ULL,
    0x000A1100110A0000ULL, 0x0014220022140000ULL,
    0x0028440044280000ULL, 0x0050880088500000ULL,
    0x00A0100010A00000ULL, 0x0040200020400000ULL,
    0x0204000402000000ULL, 0x0508000805000000ULL,
    0x0A1100110A000000ULL, 0x1422002214000000ULL,
    0x2844004428000000ULL, 0x5088008850000000ULL,
    0xA0100010A0000000ULL, 0x4020002040000000ULL,
    0x0400040200000000ULL, 0x0800080500000000ULL,
    0x1100110A00000000ULL, 0x2200221400000000ULL,
    0x4400442800000000ULL, 0x8800885000000000ULL,
    0x100010A000000000ULL, 0x2000204000000000ULL,
    0x0004020000000000ULL, 0x0008050000000000ULL,
    0x00110A0000000000ULL, 0x0022140000000000ULL,
    0x0044280000000000ULL, 0x0088500000000000ULL,
    0x0010A00000000000ULL, 0x0020400000000000ULL
};

const uint64_t TuxedoCat::LookupData::kingAttacks[64] = {
    0x0000000000000302ULL, 0x0000000000000705ULL,
    0x0000000000000E0AULL, 0x0000000000001C14ULL,
    0x0000000000003828ULL, 0x0000000000007050ULL,
    0x000000000000E0A0ULL, 0x000000000000C040ULL,
    0x0000000000030203ULL, 0x0000000000070507ULL,
    0x00000000000E0A0EULL, 0x00000000001C141CULL,
    0x0000000000382838ULL, 0x0000000000705070ULL,
    0x0000000000E0A0E0ULL, 0x0000000000C040C0ULL,
    0x0000000003020300ULL, 0x0000000007050700ULL,
    0x000000000E0A0E00ULL, 0x000000001C141C00ULL,
    0x0000000038283800ULL, 0x0000000070507000ULL,
    0x00000000E0A0E000ULL, 0x00000000C040C000ULL,
    0x0000000302030000ULL, 0x0000000705070000ULL,
    0x0000000E0A0E0000ULL, 0x0000001C141C0000ULL,
    0x0000003828380000ULL, 0x0000007050700000ULL,
    0x000000E0A0E00000ULL, 0x000000C040C00000ULL,
    0x0000030203000000ULL, 0x0000070507000000ULL,
    0x00000E0A0E000000ULL, 0x00001C141C000000ULL,
    0x0000382838000000ULL, 0x0000705070000000ULL,
    0x0000E0A0E0000000ULL, 0x0000C040C0000000ULL,
    0x0003020300000000ULL, 0x0007050700000000ULL,
    0x000E0A0E00000000ULL, 0x001C141C00000000ULL,
    0x0038283800000000ULL, 0x0070507000000000ULL,
    0x00E0A0E000000000ULL, 0x00C040C000000000ULL,
    0x0302030000000000ULL, 0x0705070000000000ULL,
    0x0E0A0E0000000000ULL, 0x1C141C0000000000ULL,
    0x3828380000000000ULL, 0x7050700000000000ULL,
    0xE0A0E00000000000ULL, 0xC040C00000000000ULL,
    0x0203000000000000ULL, 0x0507000000000000ULL,
    0x0A0E000000000000ULL, 0x141C000000000000ULL,
    0x2838000000000000ULL, 0x5070000000000000ULL,
    0xA0E0000000000000ULL, 0x40C0000000000000ULL
};

const uint64_t TuxedoCat::LookupData::rayAttacksN[64] = {
    0x0101010101010100ULL, 0x0202020202020200ULL,
    0x0404040404040400ULL, 0x0808080808080800ULL,
    0x1010101010101000ULL, 0x2020202020202000ULL,
    0x4040404040404000ULL, 0x8080808080808000ULL,
    0x0101010101010000ULL, 0x0202020202020000ULL,
    0x0404040404040000ULL, 0x0808080808080000ULL,
    0x1010101010100000ULL, 0x2020202020200000ULL,
    0x4040404040400000ULL, 0x8080808080800000ULL,
    0x0101010101000000ULL, 0x0202020202000000ULL,
    0x0404040404000000ULL, 0x0808080808000000ULL,
    0x1010101010000000ULL, 0x2020202020000000ULL,
    0x4040404040000000ULL, 0x8080808080000000ULL,
    0x0101010100000000ULL, 0x0202020200000000ULL,
    0x0404040400000000ULL, 0x0808080800000000ULL,
    0x1010101000000000ULL, 0x2020202000000000ULL,
    0x4040404000000000ULL, 0x8080808000000000ULL,
    0x0101010000000000ULL, 0x0202020000000000ULL,
    0x0404040000000000ULL, 0x0808080000000000ULL,
    0x1010100000000000ULL, 0x2020200000000000ULL,
    0x4040400000000000ULL, 0x8080800000000000ULL,
    0x0101000000000000ULL, 0x0202000000000000ULL,
    0x0404000000000000ULL, 0x0808000000000000ULL,
    0x1010000000000000ULL, 0x2020000000000000ULL,
    0x4040000000000000ULL, 0x8080000000000000ULL,
    0x0100000000000000ULL, 0x0200000000000000ULL,
    0x0400000000000000ULL, 0x0800000000000000ULL,
    0x1000000000000000ULL, 0x2000000000000000ULL,
    0x4000000000000000ULL, 0x8000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL
};

const uint64_t TuxedoCat::LookupData::rayAttacksS[64] = {
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000001ULL, 0x0000000000000002ULL,
    0x0000000000000004ULL, 0x0000000000000008ULL,
    0x0000000000000010ULL, 0x0000000000000020ULL,
    0x0000000000000040ULL, 0x0000000000000080ULL,
    0x0000000000000101ULL, 0x0000000000000202ULL,
    0x0000000000000404ULL, 0x0000000000000808ULL,
    0x0000000000001010ULL, 0x0000000000002020ULL,
    0x0000000000004040ULL, 0x0000000000008080ULL,
    0x0000000000010101ULL, 0x0000000000020202ULL,
    0x0000000000040404ULL, 0x0000000000080808ULL,
    0x0000000000101010ULL, 0x0000000000202020ULL,
    0x0000000000404040ULL, 0x0000000000808080ULL,
    0x0000000001010101ULL, 0x0000000002020202ULL,
    0x0000000004040404ULL, 0x0000000008080808ULL,
    0x0000000010101010ULL, 0x0000000020202020ULL,
    0x0000000040404040ULL, 0x0000000080808080ULL,
    0x0000000101010101ULL, 0x0000000202020202ULL,
    0x0000000404040404ULL, 0x0000000808080808ULL,
    0x0000001010101010ULL, 0x0000002020202020ULL,
    0x0000004040404040ULL, 0x0000008080808080ULL,
    0x0000010101010101ULL, 0x0000020202020202ULL,
    0x0000040404040404ULL, 0x0000080808080808ULL,
    0x0000101010101010ULL, 0x0000202020202020ULL,
    0x0000404040404040ULL, 0x0000808080808080ULL,
    0x0001010101010101ULL, 0x0002020202020202ULL,
    0x0004040404040404ULL, 0x0008080808080808ULL,
    0x0010101010101010ULL, 0x0020202020202020ULL,
    0x0040404040404040ULL, 0x0080808080808080ULL
};

const uint64_t TuxedoCat::LookupData::rayAttacksE[64] = {
    0x00000000000000FEULL, 0x00000000000000FCULL,
    0x00000000000000F8ULL, 0x00000000000000F0ULL,
    0x00000000000000E0ULL, 0x00000000000000C0ULL,
    0x0000000000000080ULL, 0x0000000000000000ULL,
    0x000000000000FE00ULL, 0x000000000000FC00ULL,
    0x000000000000F800ULL, 0x000000000000F000ULL,
    0x000000000000E000ULL, 0x000000000000C000ULL,
    0x0000000000008000ULL, 0x0000000000000000ULL,
    0x0000000000FE0000ULL, 0x0000000000FC0000ULL,
    0x0000000000F80000ULL, 0x0000000000F00000ULL,
    0x0000000000E00000ULL, 0x0000000000C00000ULL,
    0x0000000000800000ULL, 0x0000000000000000ULL,
    0x00000000FE000000ULL, 0x00000000FC000000ULL,
    0x00000000F8000000ULL, 0x00000000F0000000ULL,
    0x00000000E0000000ULL, 0x00000000C0000000ULL,
    0x0000000080000000ULL, 0x0000000000000000ULL,
    0x000000FE00000000ULL, 0x000000FC00000000ULL,
    0x000000F800000000ULL, 0x000000F000000000ULL,
    0x000000E000000000ULL, 0x000000C000000000ULL,
    0x0000008000000000ULL, 0x0000000000000000ULL,
    0x0000FE0000000000ULL, 0x0000FC0000000000ULL,
    0x0000F80000000000ULL, 0x0000F00000000000ULL,
    0x0000E00000000000ULL, 0x0000C00000000000ULL,
    0x0000800000000000ULL, 0x0000000000000000ULL,
    0x00FE000000000000ULL, 0x00FC000000000000ULL,
    0x00F8000000000000ULL, 0x00F0000000000000ULL,
    0x00E0000000000000ULL, 0x00C0000000000000ULL,
    0x0080000000000000ULL, 0x0000000000000000ULL,
    0xFE00000000000000ULL, 0xFC00000000000000ULL,
    0xF800000000000000ULL, 0xF000000000000000ULL,
    0xE000000000000000ULL, 0xC000000000000000ULL,
    0x8000000000000000ULL, 0x0000000000000000ULL
};

const uint64_t TuxedoCat::LookupData::rayAttacksW[64] = {
    0x0000000000000000ULL, 0x0000000000000001ULL,
    0x0000000000000003ULL, 0x0000000000000007ULL,
    0x000000000000000FULL, 0x000000000000001FULL,
    0x000000000000003FULL, 0x000000000000007FULL,
    0x0000000000000000ULL, 0x0000000000000100ULL,
    0x0000000000000300ULL, 0x0000000000000700ULL,
    0x0000000000000F00ULL, 0x0000000000001F00ULL,
    0x0000000000003F00ULL, 0x0000000000007F00ULL,
    0x0000000000000000ULL, 0x0000000000010000ULL,
    0x0000000000030000ULL, 0x0000000000070000ULL,
    0x00000000000F0000ULL, 0x00000000001F0000ULL,
    0x00000000003F0000ULL, 0x00000000007F0000ULL,
    0x0000000000000000ULL, 0x0000000001000000ULL,
    0x0000000003000000ULL, 0x0000000007000000ULL,
    0x000000000F000000ULL, 0x000000001F000000ULL,
    0x000000003F000000ULL, 0x000000007F000000ULL,
    0x0000000000000000ULL, 0x0000000100000000ULL,
    0x0000000300000000ULL, 0x0000000700000000ULL,
    0x0000000F00000000ULL, 0x0000001F00000000ULL,
    0x0000003F00000000ULL, 0x0000007F00000000ULL,
    0x0000000000000000ULL, 0x0000010000000000ULL,
    0x0000030000000000ULL, 0x0000070000000000ULL,
    0x00000F0000000000ULL, 0x00001F0000000000ULL,
    0x00003F0000000000ULL, 0x00007F0000000000ULL,
    0x0000000000000000ULL, 0x0001000000000000ULL,
    0x0003000000000000ULL, 0x0007000000000000ULL,
    0x000F000000000000ULL, 0x001F000000000000ULL,
    0x003F000000000000ULL, 0x007F000000000000ULL,
    0x0000000000000000ULL, 0x0100000000000000ULL,
    0x0300000000000000ULL, 0x0700000000000000ULL,
    0x0F00000000000000ULL, 0x1F00000000000000ULL,
    0x3F00000000000000ULL, 0x7F00000000000000ULL
};

const uint64_t TuxedoCat::LookupData::rayAttacksNE[64] = {
    0x8040201008040200ULL, 0x0080402010080400ULL,
    0x0000804020100800ULL, 0x0000008040201000ULL,
    0x0000000080402000ULL, 0x0000000000804000ULL,
    0x0000000000008000ULL, 0x0000000000000000ULL,
    0x4020100804020000ULL, 0x8040201008040000ULL,
    0x0080402010080000ULL, 0x0000804020100000ULL,
    0x0000008040200000ULL, 0x0000000080400000ULL,
    0x0000000000800000ULL, 0x0000000000000000ULL,
    0x2010080402000000ULL, 0x4020100804000000ULL,
    0x8040201008000000ULL, 0x0080402010000000ULL,
    0x0000804020000000ULL, 0x0000008040000000ULL,
    0x0000000080000000ULL, 0x0000000000000000ULL,
    0x1008040200000000ULL, 0x2010080400000000ULL,
    0x4020100800000000ULL, 0x8040201000000000ULL,
    0x0080402000000000ULL, 0x0000804000000000ULL,
    0x0000008000000000ULL, 0x0000000000000000ULL,
    0x0804020000000000ULL, 0x1008040000000000ULL,
    0x2010080000000000ULL, 0x4020100000000000ULL,
    0x8040200000000000ULL, 0x0080400000000000ULL,
    0x0000800000000000ULL, 0x0000000000000000ULL,
    0x0402000000000000ULL, 0x0804000000000000ULL,
    0x1008000000000000ULL, 0x2010000000000000ULL,
    0x4020000000000000ULL, 0x8040000000000000ULL,
    0x0080000000000000ULL, 0x0000000000000000ULL,
    0x0200000000000000ULL, 0x0400000000000000ULL,
    0x0800000000000000ULL, 0x1000000000000000ULL,
    0x2000000000000000ULL, 0x4000000000000000ULL,
    0x8000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL
};

const uint64_t TuxedoCat::LookupData::rayAttacksNW[64] = {
    0x0000000000000000ULL, 0x0000000000000100ULL,
    0x0000000000010200ULL, 0x0000000001020400ULL,
    0x0000000102040800ULL, 0x0000010204081000ULL,
    0x0001020408102000ULL, 0x0102040810204000ULL,
    0x0000000000000000ULL, 0x0000000000010000ULL,
    0x0000000001020000ULL, 0x0000000102040000ULL,
    0x0000010204080000ULL, 0x0001020408100000ULL,
    0x0102040810200000ULL, 0x0204081020400000ULL,
    0x0000000000000000ULL, 0x0000000001000000ULL,
    0x0000000102000000ULL, 0x0000010204000000ULL,
    0x0001020408000000ULL, 0x0102040810000000ULL,
    0x0204081020000000ULL, 0x0408102040000000ULL,
    0x0000000000000000ULL, 0x0000000100000000ULL,
    0x0000010200000000ULL, 0x0001020400000000ULL,
    0x0102040800000000ULL, 0x0204081000000000ULL,
    0x0408102000000000ULL, 0x0810204000000000ULL,
    0x0000000000000000ULL, 0x0000010000000000ULL,
    0x0001020000000000ULL, 0x0102040000000000ULL,
    0x0204080000000000ULL, 0x0408100000000000ULL,
    0x0810200000000000ULL, 0x1020400000000000ULL,
    0x0000000000000000ULL, 0x0001000000000000ULL,
    0x0102000000000000ULL, 0x0204000000000000ULL,
    0x0408000000000000ULL, 0x0810000000000000ULL,
    0x1020000000000000ULL, 0x2040000000000000ULL,
    0x0000000000000000ULL, 0x0100000000000000ULL,
    0x0200000000000000ULL, 0x0400000000000000ULL,
    0x0800000000000000ULL, 0x1000000000000000ULL,
    0x2000000000000000ULL, 0x4000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL
};

const uint64_t TuxedoCat::LookupData::rayAttacksSE[64] = {
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000002ULL, 0x0000000000000004ULL,
    0x0000000000000008ULL, 0x0000000000000010ULL,
    0x0000000000000020ULL, 0x0000000000000040ULL,
    0x0000000000000080ULL, 0x0000000000000000ULL,
    0x0000000000000204ULL, 0x0000000000000408ULL,
    0x0000000000000810ULL, 0x0000000000001020ULL,
    0x0000000000002040ULL, 0x0000000000004080ULL,
    0x0000000000008000ULL, 0x0000000000000000ULL,
    0x0000000000020408ULL, 0x0000000000040810ULL,
    0x0000000000081020ULL, 0x0000000000102040ULL,
    0x0000000000204080ULL, 0x0000000000408000ULL,
    0x0000000000800000ULL, 0x0000000000000000ULL,
    0x0000000002040810ULL, 0x0000000004081020ULL,
    0x0000000008102040ULL, 0x0000000010204080ULL,
    0x0000000020408000ULL, 0x0000000040800000ULL,
    0x0000000080000000ULL, 0x0000000000000000ULL,
    0x0000000204081020ULL, 0x0000000408102040ULL,
    0x0000000810204080ULL, 0x0000001020408000ULL,
    0x0000002040800000ULL, 0x0000004080000000ULL,
    0x0000008000000000ULL, 0x0000000000000000ULL,
    0x0000020408102040ULL, 0x0000040810204080ULL,
    0x0000081020408000ULL, 0x0000102040800000ULL,
    0x0000204080000000ULL, 0x0000408000000000ULL,
    0x0000800000000000ULL, 0x0000000000000000ULL,
    0x0002040810204080ULL, 0x0004081020408000ULL,
    0x0008102040800000ULL, 0x0010204080000000ULL,
    0x0020408000000000ULL, 0x0040800000000000ULL,
    0x0080000000000000ULL, 0x0000000000000000ULL
};

const uint64_t TuxedoCat::LookupData::rayAttacksSW[64] = {
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000001ULL,
    0x0000000000000002ULL, 0x0000000000000004ULL,
    0x0000000000000008ULL, 0x0000000000000010ULL,
    0x0000000000000020ULL, 0x0000000000000040ULL,
    0x0000000000000000ULL, 0x0000000000000100ULL,
    0x0000000000000201ULL, 0x0000000000000402ULL,
    0x0000000000000804ULL, 0x0000000000001008ULL,
    0x0000000000002010ULL, 0x0000000000004020ULL,
    0x0000000000000000ULL, 0x0000000000010000ULL,
    0x0000000000020100ULL, 0x0000000000040201ULL,
    0x0000000000080402ULL, 0x0000000000100804ULL,
    0x0000000000201008ULL, 0x0000000000402010ULL,
    0x0000000000000000ULL, 0x0000000001000000ULL,
    0x0000000002010000ULL, 0x0000000004020100ULL,
    0x0000000008040201ULL, 0x0000000010080402ULL,
    0x0000000020100804ULL, 0x0000000040201008ULL,
    0x0000000000000000ULL, 0x0000000100000000ULL,
    0x0000000201000000ULL, 0x0000000402010000ULL,
    0x0000000804020100ULL, 0x0000001008040201ULL,
    0x0000002010080402ULL, 0x0000004020100804ULL,
    0x0000000000000000ULL, 0x0000010000000000ULL,
    0x0000020100000000ULL, 0x0000040201000000ULL,
    0x0000080402010000ULL, 0x0000100804020100ULL,
    0x0000201008040201ULL, 0x0000402010080402ULL,
    0x0000000000000000ULL, 0x0001000000000000ULL,
    0x0002010000000000ULL, 0x0004020100000000ULL,
    0x0008040201000000ULL, 0x0010080402010000ULL,
    0x0020100804020100ULL, 0x0040201008040201ULL
};

const uint64_t TuxedoCat::LookupData::fileMask[64] = {
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL,
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL,
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL,
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL,
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL,
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL,
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL,
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL
};

const uint64_t TuxedoCat::LookupData::rankMask[64] = {
    0x00000000000000FFULL, 0x00000000000000FFULL,
    0x00000000000000FFULL, 0x00000000000000FFULL,
    0x00000000000000FFULL, 0x00000000000000FFULL,
    0x00000000000000FFULL, 0x00000000000000FFULL,
    0x000000000000FF00ULL, 0x000000000000FF00ULL,
    0x000000000000FF00ULL, 0x000000000000FF00ULL,
    0x000000000000FF00ULL, 0x000000000000FF00ULL,
    0x000000000000FF00ULL, 0x000000000000FF00ULL,
    0x0000000000FF0000ULL, 0x0000000000FF0000ULL,
    0x0000000000FF0000ULL, 0x0000000000FF0000ULL,
    0x0000000000FF0000ULL, 0x0000000000FF0000ULL,
    0x0000000000FF0000ULL, 0x0000000000FF0000ULL,
    0x00000000FF000000ULL, 0x00000000FF000000ULL,
    0x00000000FF000000ULL, 0x00000000FF000000ULL,
    0x00000000FF000000ULL, 0x00000000FF000000ULL,
    0x00000000FF000000ULL, 0x00000000FF000000ULL,
    0x000000FF00000000ULL, 0x000000FF00000000ULL,
    0x000000FF00000000ULL, 0x000000FF00000000ULL,
    0x000000FF00000000ULL, 0x000000FF00000000ULL,
    0x000000FF00000000ULL, 0x000000FF00000000ULL,
    0x0000FF0000000000ULL, 0x0000FF0000000000ULL,
    0x0000FF0000000000ULL, 0x0000FF0000000000ULL,
    0x0000FF0000000000ULL, 0x0000FF0000000000ULL,
    0x0000FF0000000000ULL, 0x0000FF0000000000ULL,
    0x00FF000000000000ULL, 0x00FF000000000000ULL,
    0x00FF000000000000ULL, 0x00FF000000000000ULL,
    0x00FF000000000000ULL, 0x00FF000000000000ULL,
    0x00FF000000000000ULL, 0x00FF000000000000ULL,
    0xFF00000000000000ULL, 0xFF00000000000000ULL,
    0xFF00000000000000ULL, 0xFF00000000000000ULL,
    0xFF00000000000000ULL, 0xFF00000000000000ULL,
    0xFF00000000000000ULL, 0xFF00000000000000ULL
};

const uint64_t TuxedoCat::LookupData::swneMask[64] = {
    0x8040201008040201ULL, 0x0080402010080402ULL,
    0x0000804020100804ULL, 0x0000008040201008ULL,
    0x0000000080402010ULL, 0x0000000000804020ULL,
    0x0000000000008040ULL, 0x0000000000000080ULL,
    0x4020100804020100ULL, 0x8040201008040201ULL,
    0x0080402010080402ULL, 0x0000804020100804ULL,
    0x0000008040201008ULL, 0x0000000080402010ULL,
    0x0000000000804020ULL, 0x0000000000008040ULL,
    0x2010080402010000ULL, 0x4020100804020100ULL,
    0x8040201008040201ULL, 0x0080402010080402ULL,
    0x0000804020100804ULL, 0x0000008040201008ULL,
    0x0000000080402010ULL, 0x0000000000804020ULL,
    0x1008040201000000ULL, 0x2010080402010000ULL,
    0x4020100804020100ULL, 0x8040201008040201ULL,
    0x0080402010080402ULL, 0x0000804020100804ULL,
    0x0000008040201008ULL, 0x0000000080402010ULL,
    0x0804020100000000ULL, 0x1008040201000000ULL,
    0x2010080402010000ULL, 0x4020100804020100ULL,
    0x8040201008040201ULL, 0x0080402010080402ULL,
    0x0000804020100804ULL, 0x0000008040201008ULL,
    0x0402010000000000ULL, 0x0804020100000000ULL,
    0x1008040201000000ULL, 0x2010080402010000ULL,
    0x4020100804020100ULL, 0x8040201008040201ULL,
    0x0080402010080402ULL, 0x0000804020100804ULL,
    0x0201000000000000ULL, 0x0402010000000000ULL,
    0x0804020100000000ULL, 0x1008040201000000ULL,
    0x2010080402010000ULL, 0x4020100804020100ULL,
    0x8040201008040201ULL, 0x0080402010080402ULL,
    0x0100000000000000ULL, 0x0201000000000000ULL,
    0x0402010000000000ULL, 0x0804020100000000ULL,
    0x1008040201000000ULL, 0x2010080402010000ULL,
    0x4020100804020100ULL, 0x8040201008040201ULL
};

const uint64_t TuxedoCat::LookupData::nwseMask[64] = {
    0x0000000000000001ULL, 0x0000000000000102ULL,
    0x0000000000010204ULL, 0x0000000001020408ULL,
    0x0000000102040810ULL, 0x0000010204081020ULL,
    0x0001020408102040ULL, 0x0102040810204080ULL,
    0x0000000000000102ULL, 0x0000000000010204ULL,
    0x0000000001020408ULL, 0x0000000102040810ULL,
    0x0000010204081020ULL, 0x0001020408102040ULL,
    0x0102040810204080ULL, 0x0204081020408000ULL,
    0x0000000000010204ULL, 0x0000000001020408ULL,
    0x0000000102040810ULL, 0x0000010204081020ULL,
    0x0001020408102040ULL, 0x0102040810204080ULL,
    0x0204081020408000ULL, 0x0408102040800000ULL,
    0x0000000001020408ULL, 0x0000000102040810ULL,
    0x0000010204081020ULL, 0x0001020408102040ULL,
    0x0102040810204080ULL, 0x0204081020408000ULL,
    0x0408102040800000ULL, 0x0810204080000000ULL,
    0x0000000102040810ULL, 0x0000010204081020ULL,
    0x0001020408102040ULL, 0x0102040810204080ULL,
    0x0204081020408000ULL, 0x0408102040800000ULL,
    0x0810204080000000ULL, 0x1020408000000000ULL,
    0x0000010204081020ULL, 0x0001020408102040ULL,
    0x0102040810204080ULL, 0x0204081020408000ULL,
    0x0408102040800000ULL, 0x0810204080000000ULL,
    0x1020408000000000ULL, 0x2040800000000000ULL,
    0x0001020408102040ULL, 0x0102040810204080ULL,
    0x0204081020408000ULL, 0x0408102040800000ULL,
    0x0810204080000000ULL, 0x1020408000000000ULL,
    0x2040800000000000ULL, 0x4080000000000000ULL,
    0x0102040810204080ULL, 0x0204081020408000ULL,
    0x0408102040800000ULL, 0x0810204080000000ULL,
    0x1020408000000000ULL, 0x2040800000000000ULL,
    0x4080000000000000ULL, 0x8000000000000000ULL
};

const int TuxedoCat::LookupData::index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
    61, 58, 50, 42, 38, 29, 17,  4,
    62, 55, 59, 36, 53, 51, 43, 22,
    45, 39, 33, 30, 24, 18, 12,  5,
    63, 47, 56, 27, 60, 41, 37, 16,
    54, 35, 52, 21, 44, 32, 23, 11,
    46, 26, 40, 15, 34, 20, 31, 10,
    25, 14, 19,  9, 13,  8,  7,  6
};

const int TuxedoCat::LookupData::index64Reverse[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
    57, 49, 41, 37, 28, 16,  3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11,  4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30,  9, 24,
    13, 18,  8, 12,  7,  6,  5, 63
};

const uint64_t TuxedoCat::LookupData::k1 = 0x5555555555555555ULL;
const uint64_t TuxedoCat::LookupData::k2 = 0x3333333333333333ULL;
const uint64_t TuxedoCat::LookupData::k4 = 0x0f0f0f0f0f0f0f0fULL;
const uint64_t TuxedoCat::LookupData::kf = 0x0101010101010101ULL;