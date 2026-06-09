#pragma once

#include <atomic>

#include "core/TorrentClient.hpp"
#include "ui/UiRenderer.hpp"

class TorrentUi {
public:
    explicit TorrentUi(TorrentClient& client);

    void Run();

private:
    TorrentClient& client;
    UiRenderer renderer;
    std::atomic<bool> running {true};
};

