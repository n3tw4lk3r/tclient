#include "ui/UiRenderer.hpp"

#include <iostream>

#include "ui/UiFormatter.hpp"

void UiRenderer::ClearScreen() {
    std::cout << "\033[2J" << "\033[H";
}

void UiRenderer::Render(
    const TorrentTask& task,
    std::chrono::seconds elapsed,
    const std::vector<std::string>& logs
) const {
    ClearScreen();

    std::cout
        << UiFormatter::Center(
               "== TORRENT CLIENT ==",
               kScreenWidth
           )
        << "\n\n";

    std::cout
        << UiFormatter::Center(
               UiFormatter::Trim(
                   task.filename,
                   60
               ),
               kScreenWidth
           )
        << "\n\n";

    std::cout
        << UiFormatter::Center(
               UiFormatter::BuildProgressBar(
                   task.progress
               ),
               kScreenWidth
           )
        << "\n\n";

    std::cout
        << UiFormatter::Center(
               std::to_string(
                   static_cast<int>(task.progress)
               ) + "%",
               kScreenWidth
           )
        << '\n';

    std::cout
        << UiFormatter::Center(
               UiFormatter::FormatBytes(task.downloaded) +
               " / " +
               UiFormatter::FormatBytes(task.total_size),
               kScreenWidth
           )
        << '\n';

    std::cout
        << UiFormatter::Center(
               std::to_string(task.downloaded_pieces_count) +
               " / " +
               std::to_string(task.total_pieces_count) +
               " pieces",
               kScreenWidth
           )
        << "\n\n";

    std::cout
        << UiFormatter::Center(
               "[STATUS] " +
               task.GetStatusString(),
               kScreenWidth
           )
        << '\n';

    std::cout
        << UiFormatter::Center(
               "[ELAPSED] " +
               UiFormatter::FormatDuration(elapsed),
               kScreenWidth
           )
        << '\n';

    std::cout
        << UiFormatter::Center(
               "[PEERS] " +
               std::to_string(task.connected_peers) +
               " connected / " +
               std::to_string(task.total_peers_count) +
               " known",
               kScreenWidth
           )
        << "\n\n";

    std::cout
        << UiFormatter::Center(
               "[LOG]",
               kScreenWidth
           )
        << "\n";

    size_t begin = 0;

    if (logs.size() > kLogLines) {
        begin = logs.size() - kLogLines;
    }

    for (size_t i = begin; i < logs.size(); ++i) {
        std::cout
            << UiFormatter::Trim(logs[i], kScreenWidth)
            << '\n';
    }

    std::cout.flush();
}

