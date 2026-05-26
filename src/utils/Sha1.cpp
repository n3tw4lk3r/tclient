#include "utils/Sha1.hpp"
#include <array>
#include <cstdint>
#include <cstring>

Sha1::Sha1()
{
    Reset();
}

void Sha1::Update(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        message_block[bytes_in_block] = data[i];
        ++bytes_in_block;
        total_bits_processed += 8;

        if (bytes_in_block == kBlockSize) {
            ProcessBlock(message_block.data());
            bytes_in_block = 0;
        }
    }
}

std::array<uint8_t, Sha1::kDigestSize> Sha1::Final() {
    ApplyPadding();
    std::array<uint8_t, kDigestSize> digest{};

    for (size_t i = 0; i < kHashStateSize; ++i) {
        digest[(i << 2) + 0] = static_cast<uint8_t>((hash_state[i] >> 24) & 0xFF);
        digest[(i << 2) + 1] = static_cast<uint8_t>((hash_state[i] >> 16) & 0xFF);
        digest[(i << 2) + 2] = static_cast<uint8_t>((hash_state[i] >> 8) & 0xFF);
        digest[(i << 2) + 3] = static_cast<uint8_t>(hash_state[i] & 0xFF);
    }

    return digest;
}

uint32_t Sha1::RotateLeft(uint32_t value, uint32_t bits) {
    return (value << bits) | (value >> (32 - bits));
}

void Sha1::Reset() {
    hash_state[0] = 0x67452301;
    hash_state[1] = 0xEFCDAB89;
    hash_state[2] = 0x98BADCFE;
    hash_state[3] = 0x10325476;
    hash_state[4] = 0xC3D2E1F0;
    bytes_in_block = 0;
    total_bits_processed = 0;
}

void Sha1::ProcessBlock(const uint8_t* block) {
    uint32_t word_schedule[kWordScheduleSize];

    for (size_t i = 0; i < kChunkWords; ++i) {
        word_schedule[i] =
            (static_cast<uint32_t>(block[(i << 2) + 0]) << 24) |
            (static_cast<uint32_t>(block[(i << 2) + 1]) << 16) |
            (static_cast<uint32_t>(block[(i << 2) + 2]) << 8) |
            (static_cast<uint32_t>(block[(i << 2) + 3]));
    }

    for (size_t i = kChunkWords; i < kWordScheduleSize; ++i) {
        word_schedule[i] = RotateLeft(
            word_schedule[i - 3] ^ word_schedule[i - 8] ^ 
                word_schedule[i - 14] ^ word_schedule[i - 16],
            1
        );
    }

    uint32_t a = hash_state[0];
    uint32_t b = hash_state[1];
    uint32_t c = hash_state[2];
    uint32_t d = hash_state[3];
    uint32_t e = hash_state[4];

    for (size_t i = 0; i < kWordScheduleSize; ++i) {
        uint32_t function_value;
        uint32_t round_constant;

        if (i < 20) {
            function_value = (b & c) | (~b & d);
            round_constant = 0x5A827999;
        } else if (i < 40) {
            function_value = b ^ c ^ d;
            round_constant = 0x6ED9EBA1;
        } else if (i < 60) {
            function_value = (b & c) | (b & d) | (c & d);
            round_constant = 0x8F1BBCDC;
        } else {
            function_value = b ^ c ^ d;
            round_constant = 0xCA62C1D6;
        }

        uint32_t temp = RotateLeft(a, 5) +
            function_value +
            e +
            round_constant +
            word_schedule[i];

        e = d;
        d = c;
        c = RotateLeft(b, 30);
        b = a;
        a = temp;
    }

    hash_state[0] += a;
    hash_state[1] += b;
    hash_state[2] += c;
    hash_state[3] += d;
    hash_state[4] += e;
}

void Sha1::ApplyPadding() {
    uint64_t original_bit_length = total_bits_processed;

    message_block[bytes_in_block] = 0x80;
    ++bytes_in_block;

    if (bytes_in_block > 56) {
        while (bytes_in_block < kBlockSize) {
            message_block[bytes_in_block] = 0;
            ++bytes_in_block;
        }
        ProcessBlock(message_block.data());
        bytes_in_block = 0;
    }

    while (bytes_in_block < 56) {
        message_block[bytes_in_block] = 0;
        ++bytes_in_block;
    }

    for (int i = 7; i >= 0; --i) {
        message_block[bytes_in_block] = static_cast<uint8_t>(
            (original_bit_length >> (i * 8)) & 0xFF
        );
        ++bytes_in_block;
    }

    ProcessBlock(message_block.data());
}

