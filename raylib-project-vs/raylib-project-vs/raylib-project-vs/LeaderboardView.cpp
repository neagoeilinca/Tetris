#include "raylib.h"

#include "LeaderboardView.h"
#include "UIConstants.h"

LeaderboardView::LeaderboardView(PlayerManager& pm) : playerManager(pm)
{
    loadLeaderboard();
}

void LeaderboardView::loadLeaderboard()
{
    leaderboard = playerManager.getTopPlayers(10);  // Top 10 players
}

void LeaderboardView::refresh()
{
    loadLeaderboard();
}

void LeaderboardView::draw() const
{
    BeginDrawing();
    ClearBackground(Color{ 20, 20, 30, 255 });

    int centerX = SCREEN_WIDTH / 2;
    int startY = 60;

    DrawText("LEADERBOARD", centerX - 150, startY, 50, RAYWHITE);

    // Column headers
    DrawText("Rank", 100, startY + 70, 24, YELLOW);
    DrawText("Player Name", 200, startY + 70, 24, YELLOW);
    DrawText("High Score", 550, startY + 70, 24, YELLOW);

    DrawLine(80, startY + 100, SCREEN_WIDTH - 80, startY + 100, RAYWHITE);

    // Display leaderboard entries
    int yOffset = startY + 120;
    const int lineHeight = 40;

    if (leaderboard.empty())
    {
        DrawText("No players yet", centerX - 80, yOffset, 24, LIGHTGRAY);
    }
    else
    {
        for (size_t i = 0; i < leaderboard.size(); ++i)
        {
            int yPos = yOffset + static_cast<int>(i) * lineHeight;

            // Rank
            std::string rank = std::to_string(i + 1);
            Color rankColor = i == 0 ? GOLD : (i == 1 ? LIGHTGRAY : (i == 2 ? Color{ 205, 127, 50, 255 } : LIGHTGRAY));
            DrawText(rank.c_str(), 120, yPos, 24, rankColor);

            // Player name
            DrawText(leaderboard[i].first.c_str(), 200, yPos, 24, RAYWHITE);

            // High score
            std::string score = std::to_string(leaderboard[i].second);
            DrawText(score.c_str(), 550, yPos, 24, YELLOW);

            // Medal for top 3
            if (i == 0)
            {
                DrawText("🥇", 50, yPos, 28, GOLD);
            }
            else if (i == 1)
            {
                DrawText("🥈", 50, yPos, 28, LIGHTGRAY);
            }
            else if (i == 2)
            {
                DrawText("🥉", 50, yPos, 28, Color{ 205, 127, 50, 255 });
            }
        }
    }

    // Instructions
    DrawText("Press ESC to go back", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 50, 20, LIGHTGRAY);

    EndDrawing();
}