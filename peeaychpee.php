<?php
// Retrieve form data
$name = $_POST['name'];
$email = $_POST['email'];
$course = $_POST['course'];
$professor = $_POST['professor'];
$comments = $_POST['comments'];

// Connect to the main database
$db = new SQLite3('main.db');

// Create the main table if it doesn't exist
$db->exec('CREATE TABLE IF NOT EXISTS form_data (name TEXT, email TEXT, course TEXT, professor TEXT)');

// Insert data into the main table
$db->exec("INSERT INTO form_data (name, email, course, professor) VALUES ('$name', '$email', '$course', '$professor')");

// Connect to the comments database
$commentsDB = new SQLite3('comments.db');

// Create the comments table if it doesn't exist
$commentsDB->exec('CREATE TABLE IF NOT EXISTS comments (comment TEXT)');

// Insert comments into the comments table
$commentsDB->exec("INSERT INTO comments (comment) VALUES ('$comments')");

// Close database connections
$db->close();
$commentsDB->close();

// Send a success response
echo "Form data and comments/questions have been saved.";
?>

