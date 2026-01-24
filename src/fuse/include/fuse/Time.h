#pragma once

namespace fuse {

/// @brief
class Time {
public:
    /// @brief
    Time(double tempo) : mTime(tempo){}

    /// @return  The time in milli second.
    double asMilliSeconds() const { return mTime * 1000.0; }

    /// @return  The time in second.
    double asSeconds() const { return mTime; }

private:
    /// @brief Time in seconds.
    double mTime{};
};

} // namespace fuse
