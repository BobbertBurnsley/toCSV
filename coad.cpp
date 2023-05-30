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
    
        while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* nameResult = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* emailResult = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* courseResult = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* professorResult = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        outputFile << nameResult << "," << emailResult << "," << courseResult << "," << professorResult << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "CSV file has been generated successfully." << std::endl;

    // Comments Database
    std::string selectCommentsQuery = "SELECT * FROM comments;";
    sqlite3_stmt *commentsStmt;
    rc = sqlite3_prepare_v2(commentsDB, selectCommentsQuery.c_str(), -1, &commentsStmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot prepare select statement for comments: " << sqlite3_errmsg(commentsDB) << std::endl;
        return rc;
    }

    std::ofstream commentsOutputFile("comments.csv");
    commentsOutputFile << "Comment\n";

    while (sqlite3_step(commentsStmt) == SQLITE_ROW) {
        const char* commentResult = reinterpret_cast<const char*>(sqlite3_column_text(commentsStmt, 0));
        commentsOutputFile << commentResult << "\n";
    }

    sqlite3_finalize(commentsStmt);
    sqlite3_close(commentsDB);

    std::cout << "Comments CSV file has been generated successfully." << std::endl;

    return 0;
}


    
