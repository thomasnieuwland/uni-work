<?php
	require('../includes/loginValidation.php');
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Search</title>
		<?php require("../includes/head.php"); ?>
	</head>
	<body>
		<?php require("../includes/navbar.php"); ?>
		<div class="container search-form-box">
			<form method="POST" id="search-form" class="solo-form">
				<div class="input-group">
					<input type="text" class="form-control" required id="search-term" placeholder="Search term/keyword">
					<span class="input-group-btn">
						<button type="submit" id="register-button" class="btn btn-default">Search</button>
					</span>
				</div>
			</form>
		</div>
		<div class="panel panel-default center-block">
			<div class="panel-heading">
				All
			</div>
			<div id="search-results-panel" class="panel-body dashboard-panel">
				<table class="table" id="search-results-table">
					<thead>
						<td>Bug ID</td>
						<td class="hidden-xs">Component</td>
						<td class="hidden-xs">Severity</td>
						<td>Description</td>
					</thead>
					<tr class="loading-icon" style="display:none">
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
