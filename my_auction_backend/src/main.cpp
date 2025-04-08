#include <iostream>
#include <string>
#include "crow.h"           // Crow header (from your third_party/crow/include directory)
#include "user_handler.h"   // Contains register_user() and authenticate_user()

// Update the connection string with your actual db details from pgAdmin
// Replace "auctiondb", "postgres", and "yourpassword" with your real database name, username, and password.
const std::string CONNECTION_STRING = "dbname=auctiondb user=postgres password=123 host=127.0.0.1 port=5432";


int main()
{
    crow::SimpleApp app;

    // Simple status endpoint to verify the backend is running
    CROW_ROUTE(app, "/api/status")([](){
        crow::json::wvalue res;
        res["status"] = "Backend is running";
        return res;
    });

    // Registration endpoint: accepts a JSON payload and returns a JSON response
    CROW_ROUTE(app, "/api/register").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid JSON payload");

        std::string username  = body["username"].s();
        std::string email     = body["email"].s();
        std::string password  = body["password"].s();
        std::string firstName = body["first_name"].s();
        std::string lastName  = body["last_name"].s();

        bool success = register_user(username, email, password, firstName, lastName);

        crow::json::wvalue res;
        res["message"] = success ? "User registered successfully" : "Registration failed";
        return crow::response(success ? 200 : 500, res.dump());
    });

    // Login endpoint: accepts a JSON payload and returns a JSON response
    CROW_ROUTE(app, "/api/login").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid JSON payload");

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        bool authenticated = authenticate_user(username, password);

        crow::json::wvalue res;
        res["message"] = authenticated ? "Login successful" : "Invalid credentials";
        return crow::response(authenticated ? 200 : 401, res.dump());
    });

    // Run the backend server on port 18080 with multithreading enabled
    app.port(18080).multithreaded().run();

    return 0;
}
