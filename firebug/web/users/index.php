<?php
	require('../includes/loginValidation.php');
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | User Detail</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
		<?php require("../includes/navbar.php"); ?>
		<div class="container">
			<div class="heading-div center-block">
				<h2 id="title-heading"></h2>
			</div>
			
			<!--Details Panel-->
			<div class="panel panel-default center-block" id="users-panel-main">
				<div class="panel-heading">
					<a href="javascript:void(0)" class="panelHeader" data-id="users-panel">User Details<span class="glyphicon glyphicon-chevron-up"></span></a>
				</div>
				<div id="users-panel" class="panel-body collapse in dashboard-panel">
					<table class="table" id="users-table">
						<tr class="loading-icon">
							<td colspan="4" class="text-center">
								<p><i class="fa fa-spin fa-circle-o-notch center-block"></i> Loading</p>
							</td>
						</tr>
					</table>
				</div>
			</div>
			
			<!--Comments Panel-->
			<div class='panel panel-default center-block' id='comments-panel-main'>
				<div class="panel-heading">
					<a href="javascript:void(0)" class="panelHeader" data-id="comments-panel">Comments<span class="glyphicon glyphicon-chevron-up"></span></a>
				</div>
				<div id='comments-panel' class='panel-body'></div>
			</div>
			
		<?php require('../includes/scripts.php'); ?>
	</body>
</html>