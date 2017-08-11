<?php

if(isset($_COOKIE['id'])) {
	
	$conn = new PDO('mysql:host=localhost;dbname=FIREBUG', 'web', 'VerySecure');
	$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

	$stmt = $conn->prepare("SELECT * FROM user WHERE id=:id");
	$stmt->execute(array(
		"id" => $_COOKIE['id'],
	));

	$result = $stmt->fetchAll();

	if(count($result)==0) {
		
		unset($_COOKIE['id']);
		setcookie('id', '', time() - 3600);
		header('Location: http://'.$_SERVER['HTTP_HOST'].'/login');
	} else if(count($result) == 1) {
		
		// Valid login, move along
	} else {
		// Something fucked up
	}

}
else {
	header('Location: http://'.$_SERVER['HTTP_HOST'].'/login');
}

