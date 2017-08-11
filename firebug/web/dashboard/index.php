<?php
	require('../includes/loginValidation.php');
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Create Attachment</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
		<?php require("../includes/navbar.php"); ?>
		
		<div class="btn-toolbar center-block">
			<button class="btn btn-default" id="collapse">Collapse</button>
		</div>

		<!--Owned Panel-->
		<div class="panel panel-default center-block">
			<div class="panel-heading">
				<a href="javascript:void(0)" class="panelHeader" data-id="owned-panel">Owned<span class="glyphicon glyphicon-chevron-up"></span></a>
			</div>
			<div id="owned-panel" class="panel-body collapse in dashboard-panel">
				<table class="table table-hover" id="owned-table">
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
			<div class="panel-footer collapse in">
				<a href="/lists/owned">See more...</a>
			</div>
		</div>
		
		<!--Following Panel-->
		<div class="panel panel-default center-block">
			<div class="panel-heading">
				<a href="javascript:void(0)" class="panelHeader" data-id="following-panel">Following<span class="glyphicon glyphicon-chevron-up"></span></a>
			</div>
			<div id="following-panel" class="panel-body collapse in dashboard-panel">
				<table class="table table-hover" id="following-table">
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
			<div class="panel-footer collapse in">
				<a href="/lists/following">See more...</a>
			</div>
		</div>

		<!--Unassigned Panel-->
		<div class="panel panel-default center-block">
			<div class="panel-heading">
				<a href="javascript:void(0)" class="panelHeader" data-id="unassigned-panel">Unassigned<span class="glyphicon glyphicon-chevron-up"></span></a>
			</div>
			<div id="unassigned-panel" class="panel-body collapse in dashboard-panel">
				<table class="table table-hover" id="unassigned-table">
					<thead>
						<td>Bug ID</td>
						<td class="hidden-xs">Component</td>
						<td class="hidden-xs">Severity</td>
						<td>Description</td>
					</thead>
					<tr>
						<td colspan="4" class="text-center loading-icon">
							<p><i class="fa fa-spin fa-circle-o-notch center-block"></i> Loading</p>
						</td>
					</tr>
				</table>
			</div>
			<div class="panel-footer collapse in">
				<a href="/lists/unassigned">See more...</a>
			</div>
		</div>
		
		<!--All Panel-->
		<div class="panel panel-default center-block">
			<div class="panel-heading">
				<a href="javascript:void(0)" class="panelHeader" data-id="all-panel">All<span class="glyphicon glyphicon-chevron-up"></span></a>
			</div>
			<div id="all-panel" class="panel-body collapse in dashboard-panel">
				<table class="table table-hover" id="all-table">
					<thead>
						<td>Bug ID</td>
						<td class="hidden-xs">Component</td>
						<td class="hidden-xs">Severity</td>
						<td>Description</td>
					</thead>
					<tr>
						<td colspan="4" class="text-center loading-icon">
							<p><i class="fa fa-spin fa-circle-o-notch center-block"></i> Loading</p>
						</td>
					</tr>
				</table>
			</div>
			<div class="panel-footer collapse in">
				<a href="/lists">See more...</a>
			</div>
		</div>

		<?php require('../includes/scripts.php'); ?>
	</body>
</html>
