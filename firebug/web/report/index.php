<?php
	require('../includes/loginValidation.php');
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Report Bug</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
		<?php require("../includes/navbar.php"); ?>
		
		<div class="container">
			<div class="login-box">
				<div class="login-header">Report a New Bug</div>
				<div class="login-content">
					<form action="" method="POST" id="createBug-form">
						<input type="text" required name="short_desc" placeholder="Snippet (Title)">
						<input type="text" required name="description" placeholder="Description">
						<input type="text" name="classification" placeholder="Classification">
						<input type="text" name="product" placeholder="Product Name">
						<input type="text" name="component" placeholder="Component">
						<input type="text" name="version" placeholder="Product Version">
						<input type="text" name="op_sys" placeholder="Operating System">
						<select name="bug_severity" class="form-control">
							<option value="placeholder" disabled selected>Severity</option>
							<option value="normal">Normal</option>
							<option value="enhancement">Enhancement</option>
							<option value="minor">Minor</option>
							<option value="critical">Critical</option>
							<option value="trivial">Trivial</option>
							<option value="major">Major</option>
							<option value="blocker">Blocker</option>
						</select>
						<button type="submit" id="register-button" class="btn btn-default">Create Bug</button>
					</form>
				</div>
				<div class="register">
					<h6><a href="/dashboard">Cancel</a></h6>
				</div>
			</div>
		</div>
		<?php require('../includes/scripts.php'); ?>
	</body>
</html>
