<?php
error_reporting(E_ALL);
ini_set('display_errors', '1');

$servername = "localhost";
$username = "root";
$password = "tobygo19";
$dbname = "lab6";

//Create connection
$conn = new mysqli($servername,$username,$password,$dbname);

if ($conn->connect_error){
	die("Connection Failed: ". $conn->connect_error);
}

$stmt = $conn->prepare("INSERT INTO shoes (brand, colour, size, stock) VALUES (?, ?, ?, ?)");
$stmt->bind_param('sssi',$brand, $colour, $size, $stock);

$brands = array("Nike", "New Balance", "Adidas", "Timberlands", "Puma", "Converse", "Vans", "Reebok", "Asics");
$colours = array("Red","Yellow","Pink","Green","Purple","Orange","Blue","Brown","White","Black","Grey");
$sizes = array("5","6","7","8","9","10","11","12");

foreach ($brands as $brand) {
	foreach ($colours as $colour){
		foreach ($sizes as $size){
			$stock = rand(0,5);
			$stmt->execute();
		}
	}
}

echo "Done!";
?>
