#ifndef CLIENT_NETWORKING_INCLUDE_SERVER_CLIENT_AUTHORIZATION_H_
#define CLIENT_NETWORKING_INCLUDE_SERVER_CLIENT_AUTHORIZATION_H_

#include <string>
#include <unordered_map>
#include <memory>

#include <boost/regex.hpp>

#include "user.h"
#include "connection.h"

class ClientAuthorization {
public:
    ClientAuthorization() = default;
    static inline std::unordered_map<std::string, std::shared_ptr<User>> user_map;
protected:
    void authorize_user(const std::string& username);
    static bool user_exists(const std::string& username) noexcept;
    std::shared_ptr<User> user;
private:
    static inline const boost::regex username_pattern_{"^[a-zA-Z][a-zA-Z0-9_.]{2,15}$"};
    static inline const std::string ON_AUTH_FAIL = "User name is incorrect. Please retry";
};

class InvalidUsernameException : public std::exception {
public:
    explicit InvalidUsernameException(std::string message) : message_(std::move(message)) {}
    const char *what() const noexcept override {
        return message_.c_str();
    }
private:
    std::string message_;
};

#endif //CLIENT_NETWORKING_INCLUDE_SERVER_CLIENT_AUTHORIZATION_H_