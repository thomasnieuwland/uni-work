<?php
  error_reporting(E_ALL);
  ini_set('display_errors', '1');

  $servername = "localhost";
  $username = "root";
  $password = "";
  $dbname = "assignment7";
  $conn = new mysqli($servername,$username,$password,$dbname);
  if ($conn->connect_error){
    die("Connection Failed: ". $conn->connect_error);
  }

  $sql = "INSERT INTO shoes(brand,colour,stock,size,price) VALUES (\"".$_REQUEST["brand"]."\",\"".$_REQUEST["colour"]."\",".$_REQUEST["stock"].",".$_REQUEST["size"].",".$_REQUEST["cost"].")";
  echo $sql;
  $result = mysqli_query($conn, $sql);
  $conn->close();
  $file = fopen("log.txt","a");
	date_default_timezone_set('Australia/Sydney');
  $txt = date('l jS \of F Y h:i:s A').": User added new record.\n";
	fwrite($file,$txt);
	fclose($file);
  echo "<script type=\"text/javascript\">
       document.addEventListener('DOMContentLoaded', function() {javascript:window.location.href = \"./index.php\"}, false);
      </script>";
?>
