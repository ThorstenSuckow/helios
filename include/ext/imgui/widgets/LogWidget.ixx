/**
 * @file LogWidget.ixx
 * @brief ImGui widget for displaying log messages in a scrollable text area.
 */
module;

#include <string>
#include <vector>
#include <unordered_map>
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
     * This widget maintains separate buffers for each log scope and renders them
     * in a scrollable text area. It supports filtering by log level, clearing
     * the buffer, and auto-scrolling to the latest messages.
     *
     * Each scope has its own buffer with a maximum of 1000 entries, allowing
     * efficient scope-based filtering without losing messages from other scopes.
     *
     * @note This widget uses internal locking for adding log entries from multiple threads.
     */
    class LogWidget : public ImGuiWidget {

    private:
        /**
         * @brief Key for the "all scopes" combined view.
         */
        static constexpr const char* ALL_SCOPES_KEY = "__all__";

        /**
         * @brief Per-scope log buffers. Key is scope name, value is entry vector.
         *
         * Special key "__all__" contains all entries (for "All Scopes" view).
         */
        std::unordered_map<std::string, std::vector<LogEntry>> scopeBuffers_;

        /**
         * @brief Maximum number of entries to retain per scope buffer.
         */
        std::size_t maxEntries_ = 1000;

        /**
         * @brief Mutex for thread-safe access to the log buffers.
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
         */
        std::atomic<bool> acceptNewEntries_{true};

        /**
         * @brief Counts how many log entries were skipped while logging was paused.
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
         * @brief Adds an entry to a specific buffer, trimming if necessary.
         */
        void addToBuffer(std::vector<LogEntry>& buffer, LogEntry entry) {
            buffer.push_back(std::move(entry));
            if (maxEntries_ > 0 && buffer.size() > maxEntries_) {
                const std::size_t overflow = buffer.size() - maxEntries_;
                buffer.erase(buffer.begin(),
                             buffer.begin() + static_cast<std::ptrdiff_t>(overflow));
            }
        }

        /**
         * @brief Returns the currently active buffer based on scope selection.
         */
        [[nodiscard]] const std::vector<LogEntry>& activeBuffer() const {
            if (activeScopeFilter_.empty()) {
                auto it = scopeBuffers_.find(ALL_SCOPES_KEY);
                if (it != scopeBuffers_.end()) {
                    return it->second;
                }
            } else {
                auto it = scopeBuffers_.find(activeScopeFilter_);
                if (it != scopeBuffers_.end()) {
                    return it->second;
                }
            }
            static const std::vector<LogEntry> empty;
            return empty;
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
         * @brief Adds a log entry to the appropriate scope buffer(s).
         *
         * Thread-safe. Adds to both the scope-specific buffer and the "all" buffer.
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

            // Add to scope-specific buffer
            addToBuffer(scopeBuffers_[scope], entry);

            // Add to "all scopes" buffer
            addToBuffer(scopeBuffers_[ALL_SCOPES_KEY], entry);

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
         * @brief Clears all log entries from all buffers.
         */
        void clear() noexcept {
            std::lock_guard<std::mutex> lock(bufferMutex_);
            scopeBuffers_.clear();
            skippedEntries_.store(0, std::memory_order_relaxed);
        }

        /**
         * @brief Sets the maximum number of entries to retain per scope.
         *
         * @param max The maximum buffer size.
         */
        void setMaxEntries(std::size_t max) noexcept {
            std::lock_guard<std::mutex> lock(bufferMutex_);
            maxEntries_ = max;
            // Trim all existing buffers
            for (auto& [scope, buffer] : scopeBuffers_) {
                if (maxEntries_ > 0 && buffer.size() > maxEntries_) {
                    const std::size_t overflow = buffer.size() - maxEntries_;
                    buffer.erase(buffer.begin(),
                                 buffer.begin() + static_cast<std::ptrdiff_t>(overflow));
                }
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
         * @brief Returns the current number of log entries in the active buffer.
         *
         * @return The number of entries currently stored.
         */
        [[nodiscard]] std::size_t entryCount() const noexcept {
            std::lock_guard<std::mutex> lock(bufferMutex_);
            return activeBuffer().size();
        }

        /**
         * @brief Renders the log widget using ImGui.
         */
        void draw() override {
            ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

            if (ImGui::Begin("Log Console", nullptr, ImGuiWindowFlags_NoCollapse)) {

                // Toolbar row
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

                    std::string scopePreview = (selectedScopeIndex_ == 0)
                        ? "All Scopes"
                        : (selectedScopeIndex_ <= static_cast<int>(collectedScopes_.size())
                            ? collectedScopes_[selectedScopeIndex_ - 1]
                            : "All Scopes");

                    ImGui::SetNextItemWidth(150);
                    if (ImGui::BeginCombo("Scope", scopePreview.c_str())) {
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

                textFilter_.Draw("Filter", 150);

                ImGui::Separator();

                // Log area
                const float footerHeight =
                    ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                if (ImGui::BeginChild("LogScrollRegion", ImVec2(0, -footerHeight),
                                       ImGuiChildFlags_Borders,
                                       ImGuiWindowFlags_HorizontalScrollbar)) {

                    // Copy active buffer
                    std::vector<LogEntry> bufferCopy;
                    bool hasNewContent = false;
                    {
                        std::lock_guard<std::mutex> lock(bufferMutex_);
                        bufferCopy = activeBuffer();
                        hasNewContent = scrollToBottom_;
                        scrollToBottom_ = false;
                    }

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

                    for (const auto& entry : bufferCopy) {
                        // Filter by level
                        if (static_cast<int>(entry.level) < static_cast<int>(filterLevel_)) {
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

                    // Scroll state
                    float scrollY = ImGui::GetScrollY();
                    float scrollMaxY = ImGui::GetScrollMaxY();
                    bool atBottomNow = (scrollMaxY <= 0.0f) || (scrollY >= scrollMaxY - 5.0f);

                    // Auto-scroll decision
                    if (hasNewContent && (autoScroll_ || wasAtBottom_)) {
                        ImGui::SetScrollHereY(1.0f);
                    }

                    // Pause logging if:
                    // - AutoScroll is off AND user has not scrolled to lower bottom
                    bool pauseLogging = (!autoScroll_ && !atBottomNow);
                    acceptNewEntries_.store(!pauseLogging, std::memory_order_relaxed);
                    wasAtBottom_ = atBottomNow;
                }
                ImGui::EndChild();

                // Footer
                {
                    std::lock_guard<std::mutex> lock(bufferMutex_);
                    ImGui::Text("Entries: %zu / %zu", activeBuffer().size(), maxEntries_);
                }

                bool loggingPaused = !acceptNewEntries_.load(std::memory_order_relaxed);
                auto skipped = skippedEntries_.load(std::memory_order_relaxed);

                if (loggingPaused) {
                    ImGui::SameLine();
                    if (skipped > 0) {
                        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
                                           "Logging paused (%zu entries skipped)", skipped);
                    } else {
                        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Logging paused");
                    }
                } else if (skipped > 0) {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f),
                                       "%zu entries were skipped", skipped);
                    skippedEntries_.store(0, std::memory_order_relaxed);
                }
            }
            ImGui::End();
        }
    };

}
