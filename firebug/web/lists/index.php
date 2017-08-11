<?php
	require('../includes/loginValidation.php');
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Owned Bugs</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
		<?php require("../includes/navbar.php"); ?>
		
		<div class="panel panel-default center-block">
			<div class="panel-heading">
				All
			</div>
			<div id="all-list-panel" class="panel-body dashboard-panel">
				<table class="table table-hover" id="all-list-table">
					<thead>
						<td>Bug ID</td>
						<td class="hidden-xs">Component</td>
						<td class="hidden-xs">Severity</td>
						<td>Description</td>
					</thead>
					<tr class="loading-icon">
						<td colspan="4" class="text-center">
							<p><i class="fa fa-spin fa-circle-o-notch center-block"></i> Loading</p>
						</td>
					</tr>
				</table>
			</div>
		</div>

		<?php require('../includes/scripts.php'); ?>
	</body>
</html>
