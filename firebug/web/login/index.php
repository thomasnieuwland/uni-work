<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Login</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
		<div class="container">
			<div class="login-box">
				<div class="login-header">Firebug</div>
				<div class="login-content">
					<form action="/cgi-bin/login.cgi" method="POST" id="login-form">
						<input type="text" name="username" placeholder="Username">
						<input type="password" name="password" placeholder="Password">
						<div class="alert alert-danger"></div>
						<input type="submit" class="btn btn-default" name="login" value="Login"/>
					</form>
				</div>
				<div class="register">
					<h6>New to Firebug? <a href="../register">Register</a></h6>
				</div>
			</div>
		</div>

		<?php require('../includes/scripts.php'); ?>
	</body>
</html>
<!--/cgi-bin/login.cgi-->