<?php
// Retrieve form data
$name = $_POST['name'];
$email = $_POST['email'];
$course = $_POST['course'];
$professor = $_POST['professor'];
$comments = $_POST['comments'];

// Database connection
$db = new SQLite3('data.db');

// Insert form data into the main table
$query = "INSERT INTO submissions (name, email, course, professor) VALUES ('$name', '$email', '$course', '$professor')";
$db->exec($query);

// Insert comments into the comments table
$query = "INSERT INTO comments (comments) VALUES ('$comments')";
$db->exec($query);

// Generate the CSV file
$csvData = [
    ['Name', 'Email', 'Course', 'Professor'],
    [$name, $email, $course, $professor]
];
$csvFilePath = 'submission.csv';

$csvFile = fopen($csvFilePath, 'w');
foreach ($csvData as $csvRow) {
    fputcsv($csvFile, $csvRow);
}
fclose($csvFile);

// Close the database connection
$db->close();

// Send a response message
echo 'Form Complete, Thank You!';
?>
