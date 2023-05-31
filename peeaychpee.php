<?php
// Retrieve form data
$name = $_POST['name'] ?? '';
$email = $_POST['email'] ?? '';
$course = $_POST['course'] ?? '';
$professor = $_POST['professor'] ?? '';
$comments = $_POST['comments'] ?? '';

// Connect to the main database
$db = new SQLite3('main.db');

if (!$db) {
    die("Connection to main database failed.");
}

// Create the main table if it doesn't exist
$query = 'CREATE TABLE IF NOT EXISTS form_data (name TEXT, email TEXT, course TEXT, professor TEXT)';
$result = $db->exec($query);

if (!$result) {
    die("Error creating main table: " . $db->lastErrorMsg());
}

// Insert data into the main table
$query = "INSERT INTO form_data (name, email, course, professor) VALUES ('$name', '$email', '$course', '$professor')";
$result = $db->exec($query);

if (!$result) {
    die("Error inserting data into main table: " . $db->lastErrorMsg());
}

// Connect to the comments database
$commentsDB = new SQLite3('comments.db');

if (!$commentsDB) {
    die("Connection to comments database failed.");
}

// Create the comments table if it doesn't exist
$query = 'CREATE TABLE IF NOT EXISTS comments (comment TEXT)';
$result = $commentsDB->exec($query);

if (!$result) {
    die("Error creating comments table: " . $commentsDB->lastErrorMsg());
}

// Insert comments into the comments table
$query = "INSERT INTO comments (comment) VALUES ('$comments')";
$result = $commentsDB->exec($query);

if (!$result) {
    die("Error inserting comments into comments table: " . $commentsDB->lastErrorMsg());
}

// Close database connections
$db->close();
$commentsDB->close();

// Send a success response
echo "Form data and comments/questions have been saved.";
?>

