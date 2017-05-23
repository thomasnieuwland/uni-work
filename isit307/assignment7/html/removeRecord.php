<?php
  $servername = "localhost";
  $username = "root";
  $password = "";
  $dbname = "assignment7";
  $conn = new mysqli($servername,$username,$password,$dbname);
  if ($conn->connect_error){
    die("Connection Failed: ". $conn->connect_error);
  }
  $stmt = $conn->prepare("DELETE FROM shoes WHERE id = ?");
  $stmt->bind_param('d',$_REQUEST['id']);
  $stmt->execute();
  $conn->close();
  $file = fopen("log.txt","a");
	date_default_timezone_set('Australia/Sydney');
  $txt = date('l jS \of F Y h:i:s A').": User deleted record #".$_REQUEST['id']."\n";
	fwrite($file,$txt);
	fclose($file);

  header('Location: ' . $_SERVER['HTTP_REFERER']);

 ?>
