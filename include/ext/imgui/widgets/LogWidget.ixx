/**
 * @file LogWidget.ixx
 * @brief ImGui widget for displaying log messages in a scrollable text area.
 */
module;

#include <string>
#include <vector>
#include <mutex>
#include <cstdint>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <atomic>
#include <functional>
#include "imgui.h"

export module helios.ext.imgui.widgets.LogWidget;

import helios.ext.imgui.ImGuiWidget;

export namespace helios::ext::imgui::widgets {

    /**
     * @brief Log severity level for categorizing and filtering messages.
     */
    enum class LogLevel : std::uint8_t {
        Debug = 0,
        Info  = 1,
        Warn  = 2,
        Error = 3
    };

    /**
     * @brief Represents a single log entry with level, scope, and message.
     */
    struct LogEntry {
        /**
         * @brief Severity level of this log entry.
         */
        LogLevel level = LogLevel::Info;

        /**
         * @brief Source scope/module that generated this entry.
         */
        std::string scope;

        /**
         * @brief The log message text.
         */
        std::string message;

        /**
         * @brief Formatted timestamp when this entry was created.
         */
        std::string timestamp;
    };

    /**
     * @class LogWidget
     * @brief Debug widget for displaying log output in a scrollable ImGui panel.
     *
     * This widget maintains an internal buffer of log messages and renders them
     * in a scrollable text area. It supports filtering by log level, clearing
     * the buffer, and auto-scrolling to the latest messages.
     *
     * Log messages can be added programmatically via `addLog()` or by using the
     * convenience methods `debug()`, `info()`, `warn()`, and `error()`.
     *
     * @note This widget uses internal locking for adding log entries from multiple threads.
     */
    class LogWidget : public ImGuiWidget {

    private:
        /**
         * @brief Internal buffer storing log entries.
         */
        std::vector<LogEntry> logBuffer_;

        /**
         * @brief Maximum number of entries to retain in the buffer.
         */
        std::size_t maxEntries_ = 1000;

        /**
         * @brief Mutex for thread-safe access to the log buffer.
         */
        mutable std::mutex bufferMutex_;

        /**
         * @brief Whether to automatically scroll to the bottom on new entries.
         */
        bool autoScroll_ = true;

        /**
         * @brief Flag to indicate that new content was added and scroll is needed.
         */
        bool scrollToBottom_ = false;

        /**
         * @brief Tracks if user was at bottom in the previous frame.
         */
        bool wasAtBottom_ = true;

        /**
         * @brief Whether new entries are currently accepted into the buffer.
         *
         * When AutoScroll is disabled and the user is not at the bottom,
         * this value is set to false. New log entries are then discarded
         * until the user scrolls back to the bottom or AutoScroll is
         * enabled again.
         */
        std::atomic<bool> acceptNewEntries_{true};

        /**
         * @brief Counts how many log entries were skipped while logging was paused.
         *
         * Informational only – displayed in the footer while logging is paused.
         */
        std::atomic<std::size_t> skippedEntries_{0};

        /**
         * @brief Minimum log level to display (filters out lower levels).
         */
        LogLevel filterLevel_ = LogLevel::Debug;

        /**
         * @brief Text filter for searching within log messages.
         */
        ImGuiTextFilter textFilter_;

        /**
         * @brief Current filter level selection index for the combo box.
         */
        int filterLevelIndex_ = 0;

        /**
         * @brief Collection of all unique scopes seen in log entries.
         */
        std::vector<std::string> collectedScopes_;

        /**
         * @brief Currently selected scope index in the combo box (0 = "All").
         */
        int selectedScopeIndex_ = 0;

        /**
         * @brief Currently active scope filter (empty = show all).
         */
        std::string activeScopeFilter_;

        /**
         * @brief Callback function to notify external systems of scope filter changes.
         *
         * Called when user selects a scope in the combo box.
         * Signature: void(const std::string& scope) where empty string means "all".
         */
        std::function<void(const std::string&)> onScopeFilterChanged_;

        /**
         * @brief Adds a scope to the collection if not already present.
         *
         * @param scope The scope to add.
         */
        void collectScope(const std::string& scope) {
            for (const auto& existing : collectedScopes_) {
                if (existing == scope) return;
            }
            collectedScopes_.push_back(scope);
        }

        /**
         * @brief Returns an ImVec4 color for the given log level.
         *
         * @param level The log level to get a color for.
         *
         * @return The color corresponding to the log level.
         */
        [[nodiscard]] static ImVec4 colorForLevel(LogLevel level) noexcept {
            switch (level) {
                case LogLevel::Debug: return ImVec4(0.6f, 0.6f, 0.6f, 1.0f); // Gray
                case LogLevel::Info:  return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
                case LogLevel::Warn:  return ImVec4(1.0f, 0.8f, 0.0f, 1.0f); // Yellow
                case LogLevel::Error: return ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // Red
                default:              return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            }
        }

        /**
         * @brief Returns a string label for the given log level.
         *
         * @param level The log level to get a label for.
         *
         * @return The label string (e.g., "[DEBUG]", "[INFO]").
         */
        [[nodiscard]] static const char* labelForLevel(LogLevel level) noexcept {
            switch (level) {
                case LogLevel::Debug: return "[DEBUG]";
                case LogLevel::Info:  return "[INFO] ";
                case LogLevel::Warn:  return "[WARN] ";
                case LogLevel::Error: return "[ERROR]";
                default:              return "[?????]";
            }
        }

        /**
         * @brief Generates a simple timestamp string (HH:MM:SS.mmm).
         *
         * @return The current time formatted as "HH:MM:SS.mmm".
         */
        [[nodiscard]] static std::string currentTimestamp() noexcept {
            auto now        = std::chrono::system_clock::now();
            auto time_t_now = std::chrono::system_clock::to_time_t(now);
            auto ms         = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) % 1000;

            std::tm tm_buf{};
#ifdef _WIN32
            localtime_s(&tm_buf, &time_t_now);
#else
            localtime_r(&time_t_now, &tm_buf);
#endif
            std::ostringstream oss;
            oss << std::put_time(&tm_buf, "%H:%M:%S")
                << '.' << std::setfill('0') << std::setw(3) << ms.count();
            return oss.str();
        }

    public:
        LogWidget() = default;
        ~LogWidget() override = default;

        /**
         * @brief Adds a log entry to the buffer.
         *
         * Thread-safe.
         *
         * Behavior:
         * - If `acceptNewEntries_ == false`, the entry is discarded and
         *   `skippedEntries_` is incremented.
         * - If `acceptNewEntries_ == true`, the entry is appended and, if
         *   `maxEntries_` is exceeded, old entries at the beginning are trimmed.
         *
         * @param level   The severity level of the log message.
         * @param scope   The source scope or module name.
         * @param message The log message text.
         */
        void addLog(LogLevel level, const std::string& scope, const std::string& message) {
            // If user scrolls up and AutoScroll is off, we do not accept new entries
            // to make sure memory is not flooded with new log entries in the background.
            if (!acceptNewEntries_.load(std::memory_order_relaxed)) {
                skippedEntries_.fetch_add(1, std::memory_order_relaxed);
                return;
            }

            std::lock_guard<std::mutex> lock(bufferMutex_);

            // Collect unique scopes for the filter dropdown
            collectScope(scope);

            LogEntry entry;
            entry.level     = level;
            entry.scope     = scope;
            entry.message   = message;
            entry.timestamp = currentTimestamp();

            logBuffer_.push_back(std::move(entry));

            // Trim buffer if exceeding max size
            if (maxEntries_ > 0 && logBuffer_.size() > maxEntries_) {
                const std::size_t overflow = logBuffer_.size() - maxEntries_;
                logBuffer_.erase(logBuffer_.begin(),
                                 logBuffer_.begin() + static_cast<std::ptrdiff_t>(overflow));
            }

            // Always signal new content - scroll decision happens in draw()
            scrollToBottom_ = true;
        }

        /**
         * @brief Convenience method to add a debug-level log entry.
         *
         * @param scope   The source scope or module name.
         * @param message The log message text.
         */
        void debug(const std::string& scope, const std::string& message) {
            addLog(LogLevel::Debug, scope, message);
        }

        /**
         * @brief Convenience method to add an info-level log entry.
         *
         * @param scope   The source scope or module name.
         * @param message The log message text.
         */
        void info(const std::string& scope, const std::string& message) {
            addLog(LogLevel::Info, scope, message);
        }

        /**
         * @brief Convenience method to add a warning-level log entry.
         *
         * @param scope   The source scope or module name.
         * @param message The log message text.
         */
        void warn(const std::string& scope, const std::string& message) {
            addLog(LogLevel::Warn, scope, message);
        }

        /**
         * @brief Convenience method to add an error-level log entry.
         *
         * @param scope   The source scope or module name.
         * @param message The log message text.
         */
        void error(const std::string& scope, const std::string& message) {
            addLog(LogLevel::Error, scope, message);
        }

        /**
         * @brief Clears all log entries from the buffer.
         */
        void clear() noexcept {
            std::lock_guard<std::mutex> lock(bufferMutex_);
            logBuffer_.clear();
            skippedEntries_.store(0, std::memory_order_relaxed);
        }

        /**
         * @brief Sets the maximum number of entries to retain.
         *
         * @param max The maximum buffer size.
         */
        void setMaxEntries(std::size_t max) noexcept {
            std::lock_guard<std::mutex> lock(bufferMutex_);
            maxEntries_ = max;
            if (maxEntries_ > 0 && logBuffer_.size() > maxEntries_) {
                const std::size_t overflow = logBuffer_.size() - maxEntries_;
                logBuffer_.erase(logBuffer_.begin(),
                                 logBuffer_.begin() + static_cast<std::ptrdiff_t>(overflow));
            }
        }

        /**
         * @brief Enables or disables auto-scrolling to the latest entry.
         *
         * @param enabled True to enable auto-scroll.
         */
        void setAutoScroll(bool enabled) noexcept {
            autoScroll_ = enabled;
        }

        /**
         * @brief Sets the minimum log level to display.
         *
         * @param level Entries below this level are hidden.
         */
        void setFilterLevel(LogLevel level) noexcept {
            filterLevel_      = level;
            filterLevelIndex_ = static_cast<int>(level);
        }

        /**
         * @brief Sets a callback to be invoked when the scope filter changes.
         *
         * The callback receives the selected scope string, or an empty string
         * when "All" is selected. Use this to integrate with LogManager::setScopeFilter().
         *
         * @param callback Function to call on scope filter change.
         */
        void setScopeFilterCallback(std::function<void(const std::string&)> callback) {
            onScopeFilterChanged_ = std::move(callback);
        }

        /**
         * @brief Returns the current number of log entries in the buffer.
         *
         * @return The number of entries currently stored.
         */
        [[nodiscard]] std::size_t entryCount() const noexcept {
            std::lock_guard<std::mutex> lock(bufferMutex_);
            return logBuffer_.size();
        }

        /**
         * @brief Renders the log widget using ImGui.
         */
        void draw() override {
            ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

            if (ImGui::Begin("Log Console", nullptr, ImGuiWindowFlags_NoCollapse)) {

                // Toolbar row 1
                if (ImGui::Button("Clear")) {
                    clear();
                }
                ImGui::SameLine();

                ImGui::Checkbox("Auto-Scroll", &autoScroll_);
                ImGui::SameLine();

                // Level filter combo
                const char* levelItems[] = { "Debug", "Info", "Warn", "Error" };
                ImGui::SetNextItemWidth(80);
                if (ImGui::Combo("Level", &filterLevelIndex_, levelItems, IM_ARRAYSIZE(levelItems))) {
                    filterLevel_ = static_cast<LogLevel>(filterLevelIndex_);
                }
                ImGui::SameLine();

                // Scope filter combo
                {
                    std::lock_guard<std::mutex> lock(bufferMutex_);

                    // Build scope items list: "All" + collected scopes
                    std::string scopePreview = (selectedScopeIndex_ == 0)
                        ? "All Scopes"
                        : (selectedScopeIndex_ <= static_cast<int>(collectedScopes_.size())
                            ? collectedScopes_[selectedScopeIndex_ - 1]
                            : "All Scopes");

                    ImGui::SetNextItemWidth(150);
                    if (ImGui::BeginCombo("Scope", scopePreview.c_str())) {
                        // "All" option
                        bool isSelected = (selectedScopeIndex_ == 0);
                        if (ImGui::Selectable("All Scopes", isSelected)) {
                            selectedScopeIndex_ = 0;
                            activeScopeFilter_.clear();
                            if (onScopeFilterChanged_) {
                                onScopeFilterChanged_("");
                            }
                        }
                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }

                        // Individual scopes
                        for (std::size_t i = 0; i < collectedScopes_.size(); ++i) {
                            isSelected = (selectedScopeIndex_ == static_cast<int>(i + 1));
                            if (ImGui::Selectable(collectedScopes_[i].c_str(), isSelected)) {
                                selectedScopeIndex_ = static_cast<int>(i + 1);
                                activeScopeFilter_ = collectedScopes_[i];
                                if (onScopeFilterChanged_) {
                                    onScopeFilterChanged_(activeScopeFilter_);
                                }
                            }
                            if (isSelected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
                ImGui::SameLine();

                // Text filter
                textFilter_.Draw("Filter", 150);

                ImGui::Separator();

                // Log area with scrollable child region
                const float footerHeight =
                    ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                if (ImGui::BeginChild("LogScrollRegion", ImVec2(0, -footerHeight),
                                       ImGuiChildFlags_Borders,
                                       ImGuiWindowFlags_HorizontalScrollbar)) {

                    // Read scroll position
                    float scrollY    = ImGui::GetScrollY();
                    float scrollMaxY = ImGui::GetScrollMaxY();
                    bool  atBottomNow =
                        (scrollMaxY <= 0.0f) || (scrollY >= scrollMaxY - 5.0f);

                    // Pause logging if:
                    // - AutoScroll is off AND user has not scrolled to lower bottom
                    bool pauseLogging = (!autoScroll_ && !atBottomNow);
                    acceptNewEntries_.store(!pauseLogging, std::memory_order_relaxed);

                    // Remember state for next frame
                    wasAtBottom_ = atBottomNow;

                    // Copy buffer and check scroll state under lock
                    std::vector<LogEntry> bufferCopy;
                    bool hasNewContent = false;
                    {
                        std::lock_guard<std::mutex> lock(bufferMutex_);
                        bufferCopy     = logBuffer_;
                        hasNewContent  = scrollToBottom_;
                        scrollToBottom_ = false;
                    }

                    // Determine if we should auto-scroll:
                    // - Always scroll if autoScroll_ is enabled
                    // - If autoScroll_ is disabled, only scroll if user was at bottom in previous frame
                    bool shouldScroll = false;
                    if (hasNewContent) {
                        if (autoScroll_) {
                            shouldScroll = true;
                        } else {
                            shouldScroll = wasAtBottom_;
                        }
                    }

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

                    for (const auto& entry : bufferCopy) {
                        // Filter by level
                        if (static_cast<int>(entry.level) < static_cast<int>(filterLevel_)) {
                            continue;
                        }

                        // Filter by scope (if a specific scope is selected)
                        if (!activeScopeFilter_.empty() && entry.scope != activeScopeFilter_) {
                            continue;
                        }

                        // Build display string for text filter
                        std::string displayLine =
                            entry.timestamp + " " +
                            labelForLevel(entry.level) + " [" +
                            entry.scope + "] " + entry.message;

                        // Apply text filter
                        if (!textFilter_.PassFilter(displayLine.c_str())) {
                            continue;
                        }

                        // Render with color
                        ImVec4 color = colorForLevel(entry.level);
                        ImGui::PushStyleColor(ImGuiCol_Text, color);
                        ImGui::TextUnformatted(displayLine.c_str());
                        ImGui::PopStyleColor();
                    }

                    ImGui::PopStyleVar();

                    // Auto-scroll: use SetScrollHereY at the end of content
                    if (shouldScroll) {
                        ImGui::SetScrollHereY(1.0f);
                    }
                }
                ImGui::EndChild();

                // Footer with entry count and status
                {
                    std::lock_guard<std::mutex> lock(bufferMutex_);
                    ImGui::Text("Entries: %zu / %zu", logBuffer_.size(), maxEntries_);

                    bool loggingPaused = !acceptNewEntries_.load(std::memory_order_relaxed);
                    auto skipped       = skippedEntries_.load(std::memory_order_relaxed);

                    if (loggingPaused) {
                        if (skipped > 0) {
                            ImGui::SameLine();
                            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
                                               "Logging paused (%zu entries skipped)",
                                               skipped);
                        } else {
                            ImGui::SameLine();
                            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
                                               "Logging paused");
                        }
                    } else if (skipped > 0) {
                        // Optional: Hinweis, dass beim letzten Hochscrollen Einträge übersprungen wurden
                        ImGui::SameLine();
                        ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f),
                                           "%zu entries were skipped while scrolled up",
                                           skipped);
                        // Zähler zurücksetzen, sobald wir wieder am Ende sind
                        skippedEntries_.store(0, std::memory_order_relaxed);
                    }
                }
            }
            ImGui::End();
        }
    };

}
