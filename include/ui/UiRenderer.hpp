#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "core/TorrentTask.hpp"

class UiRenderer {
public:
    void Render(
        const TorrentTask& task,
        std::chrono::seconds elapsed,
        const std::vector<std::string>& logs
    ) const;

private:
    static constexpr size_t kScreenWidth = 70;
    static constexpr size_t kLogLines = 10;

private:
    static void ClearScreen();
};

