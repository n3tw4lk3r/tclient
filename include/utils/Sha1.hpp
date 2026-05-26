#pragma once

#include <array>
#include <cstdint>
#include <cstring>

class Sha1 {
public:
    Sha1();

    void Update(const uint8_t* data, size_t length);
    std::array<uint8_t, 20> Final();

private:
    static constexpr size_t kBlockSize = 64;
    static constexpr size_t kHashStateSize = 5;
    static constexpr size_t kDigestSize = 20;
    static constexpr size_t kWordScheduleSize = 80;
    static constexpr size_t kChunkWords = 16;
    
    std::array<uint8_t, kBlockSize> message_block;
    uint32_t hash_state[kHashStateSize];
    
    size_t bytes_in_block = 0;
    uint64_t total_bits_processed = 0;

private:
    static uint32_t RotateLeft(uint32_t value, uint32_t bits);
    
    void Reset();
    void ProcessBlock(const uint8_t* block);
    void ApplyPadding();
};

