#include <string>
#include "Helpers.h"
#include "WelcomeWindow.h"
#include "GameWindow.h"

int main() {

    // read config file
    int cols, rows, mines;
    readConfig(cols, rows, mines);

    // display welcome window
    std::string playerName = showWelcomeWindow(cols * 32, rows * 32 + 100);

    // when player close window -> exit program
    if (playerName.empty()) {
        return 0;
    }

    // display game window
    showGameWindow(playerName, cols, rows, mines);

    return 0;
}