#pragma once

#include <chrono>
#include <numeric>
#include <vector>

namespace rog
{

template <typename T>
class Averager
{
    std::vector<T> _history;
    size_t _i{};

  public:
    Averager(size_t average_size = 100) : _history(average_size, T{}) {}

    void add(T t)
    {
        _history[_i++] = t;
        _i %= _history.size();
    }

    [[nodiscard]] T get() const
    {
        return std::accumulate(_history.begin(), _history.end(), T{}) / static_cast<T>(_history.size());
    }
};

class FPSTimer
{
    Averager<int> _fps_history;
    std::chrono::high_resolution_clock::time_point _fps_previous;

    int _fps{};
    double _delta{};

  public:
    FPSTimer(size_t average_size = 100) : _fps_history{average_size}
    {
        _fps_previous = std::chrono::high_resolution_clock::now();
    }

    void fps_tick()
    {
        const auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> micros = now - _fps_previous;
        _fps_previous = now;
        _delta = micros.count() / 1'000'000.0;

        _fps = static_cast<int>(1.0 / _delta);

        _fps_history.add(_fps);
    }
    [[nodiscard]] double delta() const noexcept { return _delta; }
    [[nodiscard]] float delta_f() const noexcept { return static_cast<float>(_delta); }
    [[nodiscard]] int fps() const noexcept { return _fps; }

    [[nodiscard]] int average_fps() const { return _fps_history.get(); }
};

class DurationTimer
{
    Averager<double> _activity_history;
    std::chrono::high_resolution_clock::time_point _activity_before;
    double _latest{};

  public:
    DurationTimer(size_t average_size = 100) : _activity_history{average_size} {}

    void begin_activity() { _activity_before = std::chrono::high_resolution_clock::now(); }

    double end_activity()
    {
        auto after = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> micros = after - _activity_before;

        _latest = micros.count();
        _activity_history.add(_latest);

        return _latest;
    }

    [[nodiscard]] double latest() const noexcept { return _latest; }
    [[nodiscard]] int latest_i() const noexcept { return static_cast<int>(_latest); }

    [[nodiscard]] double average() const { return _activity_history.get(); }
};

} // namespace rog
