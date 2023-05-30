#include <iostream>
#include <sqlite3.h>
#include <fstream>

int main() {
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open SQLite database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    const char* createTableQuery = "CREATE TABLE form_data (name TEXT, email TEXT, message TEXT);";
    rc = sqlite3_exec(db, createTableQuery, 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot create table: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    std::string name, email, message;
    std::cout << "Name: ";
    std::getline(std::cin, name);

    std::cout << "Email: ";
    std::getline(std::cin, email);

    std::cout << "Message: ";
    std::getline(std::cin, message);

    std::string insertQuery = "INSERT INTO form_data (name, email, message) VALUES ('" + name + "', '" + email + "', '" + message + "');";
    rc = sqlite3_exec(db, insertQuery.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot insert data: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    std::string selectQuery = "SELECT * FROM form_data;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot prepare select statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    std::ofstream outputFile("output.csv");
    outputFile << "Name,Email,Message\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* nameResult = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* emailResult = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* messageResult = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        outputFile << nameResult << "," << emailResult << "," << messageResult << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "CSV file has been generated successfully." << std::endl;

    return 0;
}
