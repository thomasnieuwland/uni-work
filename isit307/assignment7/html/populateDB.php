<?php
	error_reporting(E_ALL);
	ini_set('display_errors', '1');

	$file = fopen("log.txt","a");
	date_default_timezone_set('Australia/Sydney');
	$servername = "localhost";
	$username = "root";
	$password = "";
	$dbname = "assignment7";
	$conn = new mysqli($servername,$username,$password,$dbname);
	if ($conn->connect_error){
		die("Connection Failed: ". $conn->connect_error);
	}
	$stmt = $conn->prepare("TRUNCATE TABLE shoes");
	$stmt->execute();
	$txt = date('l jS \of F Y h:i:s A').": Records truncated by populateDB.sql to prepare for generation of new random records\n";
	fwrite($file,$txt);
	$stmt = $conn->prepare("INSERT INTO shoes (brand, colour, size, price, stock) VALUES (?, ?, ?, ?, ?)");
	$stmt->bind_param('ssidi',$brand, $colour, $size, $price, $stock);
	$brands = array("Nike", "New Balance", "Adidas", "Timberlands", "Puma", "Converse", "Vans", "Reebok", "Asics");
	$colours = array("Red","Yellow","Pink","Green","Purple","Orange","Blue","Brown","White","Black","Grey","Silver","Gold","Suade","Maroon");
	$cnt = 0;
	foreach ($brands as $brand) {
		foreach ($colours as $colour){
	    $price = rand(15,300);
			for ($i=5; $i <14 ; $i++) {
				$size=$i;
				$stock = rand(0,8);
				$stmt->execute();
				$cnt++;
			}
		}
	}

	$txt = date('l jS \of F Y h:i:s A').": Generated ".$cnt." records using populateDB.php\n";
	fwrite($file,$txt);
	fclose($file);
	$conn->close();
	header("Location: ./index.php");
?>
