#include "server/client_session.h"

ClientSession::ClientSession(tcp::socket&& socket) {
    connection_ = TCP::Connection::create(std::move(socket));
    ip_address_ = connection_->get_client_address();
}

void ClientSession::start() {
    connection_->on_connect = [this]() {
        EventHandler::on_connect(ip_address_);
        send(ON_JOIN);
    };

    connection_->on_disconnect = [this]() {
        EventHandler::on_disconnect(ip_address_);
    };

    connection_->start();

    send(ASK_TO_AUTH);
    change_action(&ClientSession::authorize_user);
}

void ClientSession::send(std::string text) {
    std::stringstream message;
    message << text;
    connection_->send(message.str());
}

void ClientSession::change_action(void (ClientSession::*callback)(const std::string&)) {
    connection_->set_on_read(std::bind(callback, this, std::placeholders::_1));
}

void ClientSession::authorize_user(const std::string& name) {
    try {
        std::regex username_pattern("^[a-zA-Z][a-zA-Z0-9_.]{2,15}$");
        if (!std::regex_match(name, username_pattern)) {
            throw IncorrectUsername(ON_AUTH_FAIL);
        }
        user_ = std::make_shared<User>(name);
        send(ON_AUTH_MESSAGE + ", " + name);
        display_commands();
    } catch (const IncorrectUsername& e) {
        send(e.what());
    }
}

void ClientSession::display_commands() {
    send(ASK_TO_COMMAND);
    change_action(&ClientSession::parse_command);
}

void ClientSession::parse_command(const std::string& line) {
    try {
        std::string command;
        std::vector<std::string> args;
        std::string arg;
        std::stringstream ss(line);

        std::getline(ss, command, DELIM);
        while (std::getline(ss, arg, DELIM)) {
            args.push_back(arg);
        }
    } catch (...) {
        send("Command is incorrect");
    }
}
