#include "ember_lynx/util/Logger.hpp"

#include <cassert>
#include <filesystem>
#include <format>
#include <iostream>
#include <print>

namespace elx::util {

    namespace {
        constexpr std::string_view k_reset  = "\033[0m";
        constexpr std::string_view k_white  = "\033[97m";
        constexpr std::string_view k_green  = "\033[92m";
        constexpr std::string_view k_yellow = "\033[93m";
        constexpr std::string_view k_red    = "\033[91m";
        constexpr std::string_view k_purple = "\033[95m";
    } // namespace

    std::unique_ptr<Logger> Logger::s_instance_{ nullptr };

    Logger::Logger(std::ofstream file) noexcept : start_time_{ std::chrono::steady_clock::now() }, file_{ std::move(file) } {}

    Logger::~Logger() {
        if (file_.is_open()) {
            file_.flush();
            file_.close();
        }
    }

    std::expected<void, std::string> Logger::init(Config const& config) noexcept {
        if (s_instance_ != nullptr) {
            return std::unexpected{ "Logger is already initialised" };
        }

        if (auto parent = config.log_file_path.parent_path(); !parent.empty()) {
            std::error_code ec;
            std::filesystem::create_directories(parent, ec);
            if (ec) {
                return std::unexpected{ std::format("Failed to create log directory '{}': {}", parent.string(), ec.message()) };
            }
        }

        std::ofstream file{ config.log_file_path, std::ios::out | std::ios::trunc };
        if (!file.is_open()) {
            return std::unexpected{ std::format("Failed to open log file '{}'", config.log_file_path.string()) };
        }

        s_instance_ = std::unique_ptr<Logger>{ new Logger{ std::move(file) } };

        auto now      = std::chrono::system_clock::now();
        auto datetime = std::format("{:%Y-%m-%d %H:%M:%S} UTC", std::chrono::floor<std::chrono::seconds>(now));
        auto banner   = std::format("EmberLynx Logger, {}", datetime);

        std::println("{}{}{}", k_white, banner, k_reset);
        s_instance_->file_ << banner << std::endl;

        return {};
    }

    Logger& Logger::get() noexcept {
        assert(s_instance_ != nullptr && "Logger::get() called before Logger::init()");
        return *s_instance_;
    }

    void Logger::shutdown() noexcept {
        s_instance_.reset();
    }

    bool Logger::is_initialised() noexcept {
        return s_instance_ != nullptr;
    }

    void Logger::set_level(Level level) noexcept {
        level_.store(level, std::memory_order_relaxed);
    }

    Logger::Level Logger::level() const noexcept {
        return level_.load(std::memory_order_relaxed);
    }

    void Logger::write(Level level, std::source_location const& loc, std::string_view message) {
        if (static_cast<int32_t>(level) < static_cast<int32_t>(level_.load(std::memory_order_relaxed))) {
            return;
        }

        // Build all strings before acquiring the lock.
        std::string      timestamp = make_timestamp_();
        std::string      filename  = std::filesystem::path{ loc.file_name() }.filename().string();
        uint32_t         line      = loc.line();
        std::string_view color     = level_color_(level);
        std::string_view name      = level_name_(level);

        std::string terminal_line = std::format("{}{}  ({}:{}) - {}{}", color, timestamp, filename, line, message, k_reset);
        std::string file_line     = std::format("[{}] - {} ({}:{}) - {}", name, timestamp, filename, line, message);

        std::lock_guard lock{ mutex_ };
        std::println("{}", terminal_line);
        if (file_.is_open()) {
            file_ << file_line << std::endl;
        }
    }

    std::string Logger::make_timestamp_() const {
        auto    elapsed    = std::chrono::steady_clock::now() - start_time_;
        auto    elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
        int64_t secs       = elapsed_ns.count() / 1'000'000'000LL;
        int64_t nanos      = elapsed_ns.count() % 1'000'000'000LL;
        return std::format("{}.{:09d}", secs, nanos);
    }

    std::string_view Logger::level_name_(Level level) noexcept {
        switch (level) {
        case Level::Trace:
            return "TRACE";
        case Level::Info:
            return "INFO ";
        case Level::Warn:
            return "WARN ";
        case Level::Err:
            return "ERR  ";
        case Level::Fatal:
            return "FATAL";
        default:
            return "?????";
        }
    }

    std::string_view Logger::level_color_(Level level) noexcept {
        switch (level) {
        case Level::Trace:
            return k_white;
        case Level::Info:
            return k_green;
        case Level::Warn:
            return k_yellow;
        case Level::Err:
            return k_red;
        case Level::Fatal:
            return k_purple;
        default:
            return k_reset;
        }
    }

} // namespace elx::util
