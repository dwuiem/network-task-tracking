#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "message_sender.h"

#include <boost/regex.hpp>

class CommandHandler {
public:
    explicit CommandHandler(const std::shared_ptr<User>& user, const std::shared_ptr<MessageSender>& notifier);
    static std::pair<std::string, std::vector<std::string>> parse_command(const std::string& input);
    void execute(const std::string& command_line);
private:
    static inline const boost::regex command_pattern{R"((?:[^\s"]+|"[^"]*")+)"};
    void create_task(const std::vector<std::string>& args);
    void display(const std::vector<std::string>& args) const;

    std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commands = {
        {"create", [this](const std::vector<std::string>& args) {
            create_task(args);
        }},
        {"display", [this](const std::vector<std::string>& args) {
            display(args);
        }}
    };

    std::shared_ptr<User> user_;
    std::shared_ptr<MessageSender> notifier_;
};

class InvalidCommandException : public std::exception {
public:
    explicit InvalidCommandException(std::string message) : message_(std::move(message)) {}
    const char *what() const noexcept override {
        return message_.c_str();
    }
private:
    std::string message_;
};


#endif //COMMAND_EXECUTOR_H