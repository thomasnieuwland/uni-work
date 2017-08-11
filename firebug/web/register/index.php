<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Register</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
		<div class="container">
			<div class="login-box">
				<div class="login-header">Firebug</div>
				<div class="login-content">
					<form action="../cgi-bin/register.cgi" method="POST" id="register-form">
						<input type="text" required name="displayname" placeholder="Display Name">
						<input type="text" required name="username" placeholder="Username">
						<input type="email" required name="email" placeholder="Email">
						<input type="password" required name="password" placeholder="Password">
						<input type="password" required name="password-confirm" placeholder="Confirm Password">
						<div id="register-alert" class="alert"></div>
						<button type="submit" id="register-button" class="btn btn-default">Register</button>
					</form>
				</div>
				<div class="register">
					<h6>Already have an account? <a href="../login">Login</a></h6>
				</div>
			</div>
		</div>

		<?php require('../includes/scripts.php'); ?>
	</body>
</html>
