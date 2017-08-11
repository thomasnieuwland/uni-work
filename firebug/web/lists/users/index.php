<?php
	require('../../includes/loginValidation.php');
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Owned Bugs</title>
		<?php require('../../includes/head.php'); ?>
	</head>
	<body>
		<?php require("../../includes/navbar.php"); ?>
		
		<div class="panel panel-default center-block">
			<div class="panel-heading">
				Unassigned
			</div>
			<div id="user-list-panel" class="panel-body dashboard-panel">
				<table class="table table-hover" id="user-list-table">
					<thead>
						<td>User ID</td>
						<td>Username</td>
						<td>Email</td>
						<td>Reputation</td>
					</thead>
					<tr class="loading-icon" style="display:none">
						<td colspan="4" class="text-center">
							<p><i class="fa fa-spin fa-circle-o-notch center-block"></i> Loading</p>
						</td>
					</tr>
				</table>
			</div>
		</div>

		<?php require('../../includes/scripts.php'); ?>
	</body>
</html>
