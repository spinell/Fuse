#pragma once

namespace fuse {

/// @brief
///
///
class Timer {
public:
    Timer() noexcept;

    /// @brief Get the elapsed time since the last call to reset() without counting the time in pause.
    /// @return The elapsed time in second.
    [[nodiscard]] float totalTime() const noexcept;

    /// @brief Get the elapsed time since the last call to tick()
    /// @return The elapsed time in second.
    [[nodiscard]] float deltaTime() const noexcept;

    /// @brief Reset the timer.
    void reset() noexcept;

    /// @brief Update the timer (Must be called each frame).
    void tick() noexcept;

    /// @brief Start the timer if it was in pause.
    void start() noexcept;

    /// @brief Pause the timer if it was not already paused.
    void stop() noexcept;

private:
    double mSecondsPerCount{};
    double mDeltaTime{-1.0};

    unsigned long long mBaseTime{};
    unsigned long long mPausedTime{}; //< Accumulation of time passed in pause.
    unsigned long long mStopTime{};   //< The time when the timer as been stop (pause).
    unsigned long long mPrevTime{};
    unsigned long long mCurrTime{};

    bool mStopped = false;
};

} // namespace fuse
