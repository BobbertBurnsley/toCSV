#include <iostream>
#include <sqlite3.h>
#include <fstream>

static int callback(void *, int, char **, char **);

int main() {
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    const char* createTableQuery = "CREATE TABLE form_data (name TEXT, email TEXT, course TEXT, professor TEXT, comments TEXT);";
    rc = sqlite3_exec(db, createTableQuery, 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot create table: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    std::string name, email, course, professor, comments;

    std::cout << "Content-type:text/html\r\n\r\n";
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "<title>Form Submission Result</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";

    // Read form input from CGI environment variables
    char *data = getenv("QUERY_STRING");

    if (data != nullptr) {
        sscanf(data, "name=%[^&]&email=%[^&]&course=%[^&]&professor=%[^&]&comments=%s", name, email, course, professor, comments);

        std::string insertQuery = "INSERT INTO form_data (name, email, course, professor, comments) VALUES ('" + name + "', '" + email + "', '" + course + "', '" + professor + "', '" + comments + "');";
        rc = sqlite3_exec(db, insertQuery.c_str(), 0, 0, 0);

        if (rc != SQLITE_OK) {
            std::cerr << "Cannot insert data: " << sqlite3_errmsg(db) << std::endl;
            return rc;
        }

        std::cout << "<h2>Form submitted successfully!</h2>\n";
        std::cout << "<p>Name: " << name << "</p>\n";
        std::cout << "<p>Email: " << email << "</p>\n";
        std::cout << "<p>Course: " << course << "</p>\n";
        std::cout << "<p>Professor: " << professor << "</p>\n";
        std::cout << "<p>Comments: " << comments << "</p>\n";
    }

    std::cout << "</body>\n";
    std::cout << "</html>\n";

    sqlite3_close(db);

    return 0;
}

