#include "user_handler.h"
#include <iostream>
#include <pqxx/pqxx>

// Database connection string (it could be moved to a config file)
const std::string CONNECTION_STRING = "dbname=auctiondb user=postgres password=123 host=127.0.0.1 port=5432";


bool register_user(const std::string& username,
                   const std::string& email,
                   const std::string& password,
                   const std::string& firstName,
                   const std::string& lastName)
{
    try {
        pqxx::connection conn(CONNECTION_STRING);
        if (!conn.is_open()) {
            std::cerr << "Failed to open database." << std::endl;
            return false;
        }
        pqxx::work txn(conn);
        txn.exec_params(
            "INSERT INTO users (username, email, password, first_name, last_name) "
            "VALUES ($1, $2, $3, $4, $5)",
            username, email, password, firstName, lastName
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "register_user() error: " << e.what() << std::endl;
        return false;
    }
}

bool authenticate_user(const std::string& username, const std::string& password)
{
    try {
        pqxx::connection conn(CONNECTION_STRING);
        if (!conn.is_open()) {
            std::cerr << "Failed to open database." << std::endl;
            return false;
        }
        pqxx::work txn(conn);
        pqxx::result r = txn.exec_params("SELECT password FROM users WHERE username = $1", username);
        if (r.empty()) {
            return false; // User not found
        }
        std::string storedPassword = r[0][0].c_str();
        return (storedPassword == password); // Plain text for demonstration
    } catch (const std::exception& e) {
        std::cerr << "authenticate_user() error: " << e.what() << std::endl;
        return false;
    }
}
