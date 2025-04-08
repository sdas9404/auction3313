#ifndef USER_HANDLER_H
#define USER_HANDLER_H

#include <string>

bool register_user(const std::string& username,
                   const std::string& email,
                   const std::string& password,
                   const std::string& firstName,
                   const std::string& lastName);

bool authenticate_user(const std::string& username, const std::string& password);

#endif // USER_HANDLER_H
