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

    const char* createTableQuery = "CREATE TABLE form_data (name TEXT, email TEXT, course TEXT, professor TEXT);";
    rc = sqlite3_exec(db, createTableQuery, 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot create table: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    std::string name, email, course, professor;
    std::cout << "Name: ";
    std::getline(std::cin, name);

    std::cout << "Email: ";
    std::getline(std::cin, email);

    std::cout << "Course: ";
    std::getline(std::cin, course);

    std::cout << "Professor: ";
    std::getline(std::cin, professor);

    std::cout << "Comments: ";
    std::string comments;
    std::getline(std::cin, comments);

    std::string insertQuery = "INSERT INTO form_data (name, email, course, professor) VALUES ('" + name + "', '" + email + "', '" + course + "', '" + professor + "');";
    rc = sqlite3_exec(db, insertQuery.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot insert data: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite3 *commentsDB;
    rc = sqlite3_open(":memory:", &commentsDB);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open SQLite comments database: " << sqlite3_errmsg(commentsDB) << std::endl;
        return rc;
    }

    const char* createCommentsTableQuery = "CREATE TABLE comments (comment TEXT);";
    rc = sqlite3_exec(commentsDB, createCommentsTableQuery, 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot create comments table: " << sqlite3_errmsg(commentsDB) << std::endl;
        return rc;
    }

    std::string insertCommentsQuery = "INSERT INTO comments (comment) VALUES ('" + comments + "');";
    rc = sqlite3_exec(commentsDB, insertCommentsQuery.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot insert comments: " << sqlite3_errmsg(commentsDB) << std::endl;
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
    outputFile << "Name,Email,Course,Professor\n";

    
