#pragma once

#include "config.h"
#include "logging.hpp"
#include "utils/messages.hpp" // TYPE should be an enum {AccountUpdate, OrderbookUpdate, TradeUpdate, MarketOrder}

#include <fstream>
#include <optional>
#include <string>

namespace nutc {
namespace events {

enum class MESSAGE_TYPE { // needs to be changed to something better, but I will leave
                          // this here for now
    MARKET_ORDER,
    MATCH
};

class Logger {
    /**
     * @brief The file name to log events to
     */
    std::string file_name_;

    /**
     * @brief The output file object
     */
    std::ofstream output_file_;

public:
    /**
     * @brief Construct a new Logger object
     *
     * @param file_name File name to log to
     */

    static Logger& get_logger();

    // Logger(const Logger&) = delete;
    // Logger(Logger&&) = delete;
    // Logger& operator=(const Logger&) = delete;
    // Logger& operator=(Logger&&) = delete;

    /**
     * @brief Log an event to this Logger's file
     *
     * @param type the type of message this is, see `enum MessageType`
     * @param json_message the message to log
     * @param uid optional UID to log with this message
     */
    void log_event(
        MESSAGE_TYPE type, const std::string& json_message,
        const std::optional<std::string>& uid = std::nullopt
    );

    /**
     * @brief Get the file name string
     *
     * @return std::string
     */
    [[nodiscard]] const std::string&
    get_file_name() const
    {
        return file_name_;
    }

private:
    explicit Logger(std::string file_name) :
        file_name_(file_name), output_file_(file_name, std::ios::out | std::ios::app)
    {}
};

} // namespace events
} // namespace nutc
