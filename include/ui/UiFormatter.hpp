#pragma once

#include <chrono>
#include <cstdint>
#include <string>

class UiFormatter {
public:
    static std::string FormatDuration(
        std::chrono::seconds duration
    );

    static std::string FormatBytes(uint64_t bytes);

    static std::string BuildProgressBar(
        double progress,
        size_t width = 50
    );

    static std::string Trim(
        const std::string& text,
        size_t max_width
    );

    static std::string Center(
        const std::string& text,
        size_t width
    );
};

