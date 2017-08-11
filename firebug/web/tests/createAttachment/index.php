<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Create Attachment</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
		<div class="container">
			<div class="login-box">
				<div class="login-header">This page allows the user to create an attachment on a comment.</div>
				<div class="login-content">
					<form action="../../cgi-bin/createAttachment.cgi" method="POST" id="createBug-form">
						<input type="text" required name="bug_id" placeholder="Bug ID">
						<input type="text" required name="filename" placeholder="Filepath">
						<input type="text" required name="desc" placeholder="Description">
						<button type="submit" id="register-button" class="btn btn-default">Create Attachment</button>
					</form>
				</div>
			</div>
		</div>

		<?php require('../includes/scripts.php'); ?>
	</body>
</html>
