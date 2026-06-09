#include "Timer.h"

Timer::Timer() : isPaused(false), isRunning(false) {
    totalPausedDuration = std::chrono::seconds(0);
}

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    totalPausedDuration = std::chrono::seconds(0);
    isPaused = false;
    isRunning = true;
}

void Timer::pause() {
    if (isRunning && !isPaused) {
        pausedTime = std::chrono::high_resolution_clock::now();
        isPaused = true;
    }
}

void Timer::resume() {
    if (isRunning && isPaused) {
        auto resumeTime = std::chrono::high_resolution_clock::now();
        totalPausedDuration += std::chrono::duration_cast<std::chrono::seconds>(resumeTime - pausedTime);
        isPaused = false;
    }
}

void Timer::stop() {
    if (isRunning && !isPaused) {
        pausedTime = std::chrono::high_resolution_clock::now();
        isPaused = true;
    }
    isRunning = false;
}

void Timer::reset() {
    startTime = std::chrono::high_resolution_clock::now();
    totalPausedDuration = std::chrono::seconds(0);
    isPaused = false;
    isRunning = false;
}

int Timer::getElapsedSeconds() const {
    if (!isRunning && !isPaused) return 0;

    auto endTime = (isPaused ? pausedTime : std::chrono::high_resolution_clock::now());
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime - totalPausedDuration);
    return static_cast<int>(elapsed.count());
}

int Timer::getMinutes() const {
    return getElapsedSeconds() / 60;
}

int Timer::getSeconds() const {
    return getElapsedSeconds() % 60;
}
