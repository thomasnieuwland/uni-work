<?php

if(isset($_COOKIE['id'])) {
	// Delete the existing cookie
	unset($_COOKIE['id']);
	setcookie('id', null, -1, '/');
} else {
	// Redirect to the login page
	header('Location: http://'.$_SERVER['HTTP_HOST'].'/login');
}

?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Log out</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
	<div class="container">
		<div class="login-box">
			<div class="login-header">Firebug</div>
			<div class="login-content">
				<h4>You have been logged out</h4>
				<h5><a href="../login">Click here</a> to log in</h5>
			</div>
		</div>
	</div>

	<?php require('../includes/scripts.php'); ?>
	</body>
</html>
