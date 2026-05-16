#ifndef ELX_UTIL_LOGGER_HPP
#define ELX_UTIL_LOGGER_HPP

#include <atomic>
#include <chrono>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <mutex>
#include <source_location>
#include <string>
#include <string_view>

namespace elx::util {

    /**
     * @brief Thread-safe logger with coloured terminal output and file logging.
     *
     * Initialise once with Logger::init(), then access from anywhere via Logger::get().
     * Prefer the free functions declared in ember_lynx.hpp (elx::info, elx::warn, …)
     * over calling Logger::write() directly.
     */
    class Logger {
     public:
        /** @brief Logging severity levels, in increasing order of importance. */
        enum class Level : uint8_t {
            Trace = 0,
            Info  = 1,
            Warn  = 2,
            Err   = 3,
            Fatal = 4,
        };

        /** @brief Logger initialisation parameters. */
        struct Config {
            std::filesystem::path log_file_path;
        };

        Logger(Logger const&)            = delete;
        Logger& operator=(Logger const&) = delete;
        Logger(Logger&&)                 = delete;
        Logger& operator=(Logger&&)      = delete;
        ~Logger();

        /**
         * @brief Initialise the Logger singleton.
         *
         * Opens the log file (truncating any previous content), writes the banner
         * line to both terminal and file, then makes the instance available via get().
         * @return An error string on failure.
         */
        [[nodiscard]] static std::expected<void, std::string> init(Config const& config) noexcept;

        /**
         * @brief Retrieve the Logger singleton.
         * @pre init() must have been called successfully.
         */
        [[nodiscard]] static Logger& get() noexcept;

        /** @brief Destroy the singleton and flush all pending output. */
        static void shutdown() noexcept;

        /** @brief Returns true if the singleton has been successfully initialised. */
        [[nodiscard]] static bool is_initialised() noexcept;

        /** @brief Change the active log level at runtime (thread-safe). */
        void set_level(Level level) noexcept;

        /** @brief Return the current active log level (thread-safe). */
        [[nodiscard]] Level level() const noexcept;

        /**
         * @brief Write a pre-formatted message at the given severity.
         *
         * Messages whose level is below the active log level are silently dropped.
         * This method is thread-safe.
         */
        void write(Level level, std::source_location const& loc, std::string_view message);

     private:
        explicit Logger(std::ofstream file) noexcept;

        [[nodiscard]] static std::string_view level_name_(Level level) noexcept;
        [[nodiscard]] static std::string_view level_color_(Level level) noexcept;
        [[nodiscard]] std::string             make_timestamp_() const;

        static std::unique_ptr<Logger> s_instance_;

        std::chrono::steady_clock::time_point start_time_;
        std::atomic<Level>                    level_{ Level::Trace };
        std::mutex                            mutex_;
        std::ofstream                         file_;
    };

} // namespace elx::util

// ---- User-facing free functions -------------------------------------------------

namespace elx::detail {

    /**
     * @brief Wraps a format string and captures the caller's source location.
     *
     * The default argument for @p loc is evaluated at the call site, so
     * constructing a LogRecord from a string literal always records the line
     * where the log call appears — no macros needed.
     *
     * @note The constructors are intentionally non-explicit: implicit conversion
     *       from string literals is the mechanism that makes elx::info("…") work
     *       without macros.
     */
    struct LogRecord {
        std::string_view     fmt;
        std::source_location loc;

        /** @brief Accepts string literals directly (one UDC: array decay + this ctor). */
        LogRecord(const char* fmt, std::source_location loc = std::source_location::current()) noexcept : // NOLINT(*-explicit-constructor)
            fmt{ fmt }, loc{ loc } {}

        LogRecord(std::string_view fmt, std::source_location loc = std::source_location::current()) noexcept
            : // NOLINT(*-explicit-constructor)
            fmt{ fmt }, loc{ loc } {}
    };

} // namespace elx::detail

namespace elx {

    /** @brief Change the active log level at runtime. */
    inline void set_log_level(util::Logger::Level level) noexcept {
        util::Logger::get().set_level(level);
    }

    /** @brief Log at Trace level (white). */
    template<typename... Args>
    inline void trace(detail::LogRecord record, Args&&... args) {
        if (!util::Logger::is_initialised()) {
            return;
        }
        util::Logger& logger = util::Logger::get();
        if (static_cast<uint8_t>(util::Logger::Level::Trace) < static_cast<uint8_t>(logger.level())) {
            return;
        }
        logger.write(util::Logger::Level::Trace, record.loc, std::vformat(record.fmt, std::make_format_args(std::forward<Args>(args)...)));
    }

    /** @brief Log at Info level (green). */
    template<typename... Args>
    inline void info(detail::LogRecord record, Args&&... args) {
        if (!util::Logger::is_initialised()) {
            return;
        }
        util::Logger& logger = util::Logger::get();
        if (static_cast<uint8_t>(util::Logger::Level::Info) < static_cast<uint8_t>(logger.level())) {
            return;
        }
        logger.write(util::Logger::Level::Info, record.loc, std::vformat(record.fmt, std::make_format_args(std::forward<Args>(args)...)));
    }

    /** @brief Log at Warn level (yellow). */
    template<typename... Args>
    inline void warn(detail::LogRecord record, Args&&... args) {
        if (!util::Logger::is_initialised()) {
            return;
        }
        util::Logger& logger = util::Logger::get();
        if (static_cast<uint8_t>(util::Logger::Level::Warn) < static_cast<uint8_t>(logger.level())) {
            return;
        }
        logger.write(util::Logger::Level::Warn, record.loc, std::vformat(record.fmt, std::make_format_args(std::forward<Args>(args)...)));
    }

    /** @brief Log at Err level (red). */
    template<typename... Args>
    inline void err(detail::LogRecord record, Args&&... args) {
        if (!util::Logger::is_initialised()) {
            return;
        }
        util::Logger& logger = util::Logger::get();
        if (static_cast<uint8_t>(util::Logger::Level::Err) < static_cast<uint8_t>(logger.level())) {
            return;
        }
        logger.write(util::Logger::Level::Err, record.loc, std::vformat(record.fmt, std::make_format_args(std::forward<Args>(args)...)));
    }

    /** @brief Log at Fatal level (purple). */
    template<typename... Args>
    inline void fatal(detail::LogRecord record, Args&&... args) {
        if (!util::Logger::is_initialised()) {
            return;
        }
        util::Logger& logger = util::Logger::get();
        if (static_cast<uint8_t>(util::Logger::Level::Fatal) < static_cast<uint8_t>(logger.level())) {
            return;
        }
        logger.write(util::Logger::Level::Fatal, record.loc, std::vformat(record.fmt, std::make_format_args(std::forward<Args>(args)...)));
    }

} // namespace elx

#endif // ELX_UTIL_LOGGER_HPP
