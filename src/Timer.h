#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::duration<double> totalPausedDuration;
    std::chrono::time_point<std::chrono::high_resolution_clock> pausedTime;
    bool isPaused;
    bool isRunning;

public:
    Timer();

    // control functions
    void start();
    void pause();
    void resume();
    void reset();
    void stop(); // stop timer when paused

    // query functions
    int getElapsedSeconds() const;
    bool running() const { return isRunning; }
    bool paused() const { return isPaused; }

    // display functions
    int getMinutes() const;
    int getSeconds() const;
};

