<?php

  $servername = "localhost";
  $username = "root";
  $password = "";
  $dbname = "assignment7";
  $conn = new mysqli($servername,$username,$password,$dbname);
  if ($conn->connect_error){
    die("Connection Failed: ". $conn->connect_error);
  }

  $sql = "UPDATE shoes SET brand=\"".$_REQUEST["brand"]."\",colour=\"".$_REQUEST["colour"]."\",stock=".$_REQUEST["stock"].",size=".$_REQUEST["size"].",price=".$_REQUEST["cost"]." WHERE id=".$_REQUEST["id"];
  $result = mysqli_query($conn, $sql);
  $conn->close();
  $file = fopen("log.txt","a");
	date_default_timezone_set('Australia/Sydney');
  $txt = date('l jS \of F Y h:i:s A').": User updated record #".$_REQUEST['id']."\n";
	fwrite($file,$txt);
	fclose($file);
  echo "<script type=\"text/javascript\">
       document.addEventListener('DOMContentLoaded', function() {javascript:window.location.href = \"".$_REQUEST['refer']."\"}, false);
      </script>";
?>
