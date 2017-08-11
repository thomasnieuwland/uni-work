<?php

// Validate the login
require('includes/loginValidation.php');

// If it didn't redirect us then the following shall execute
header('Location: http://'.$_SERVER['HTTP_HOST'].'/dashboard');

?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Login</title>
		<?php require('includes/head.php'); ?>
	</head>
	<body>
		<div class="container">
			<h2>Hold on tight, we're redirecting you...</h2>
		</div>

		<?php require('includes/scripts.php'); ?>
	</body>
</html>