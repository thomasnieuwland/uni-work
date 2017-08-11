<?php
	require('../includes/loginValidation.php');
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Bug Detail</title>
		<?php require('../includes/head.php'); ?>
	</head>
	<body>
		<?php require("../includes/navbar.php"); ?>
		<div class="container">
			<div class="heading-div center-block" id="details-dropdown">
				<h2 id="details-title-heading"></h2>
				<div class="dropdown">
					<button class="btn btn-default dropdown-toggle" type="button" data-toggle="dropdown">Options
						<span class="caret"></span></button>
					<ul class="dropdown-menu">
						<li><button class="btn btn-link" type="button" onclick="deleteBug()"><i class="glyphicon glyphicon-trash"></i>  Delete</button></li>
						<li><button class="btn btn-link" type="button" onclick="editBug()"><i class="glyphicon glyphicon-pencil"></i>  Edit</button></li>
						<li><button class="btn btn-link" type="button" onclick="followBug()"><i class="glyphicon glyphicon-bell"></i>  Follow</button></li>
						<li><button class="btn btn-link" type="button" data-toggle="modal" data-target="#assign-dev-modal"><i class="glyphicon glyphicon-tasks"></i>  Assign</button></li>
					</ul>
				</div>
			</div>
			
			
			<!--Details Panel-->
			<div class="panel panel-default center-block" id="details-panel-main">
				<div class="panel-heading">
					<a href="javascript:void(0)" class="panelHeader" data-id="details-panel">Bug Details<span class="glyphicon glyphicon-chevron-up"></span></a>
				</div>
				<div id="details-panel" class="panel-body collapse in dashboard-panel">
					<table class="table" id="details-table">
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
			
			<!--Attachments Panel-->
			<div class='panel panel-default center-block' id='attachments-panel-main'>
				<div class="panel-heading">
					<a href="javascript:void(0)" class="panelHeader" data-id="attachments-panel">Attachments<span class="glyphicon glyphicon-chevron-up"></span></a>
				</div>
				<div id='attachments-panel' class='panel-body'></div>
			</div>
		</div>
		
		<!-- Modal for assigning devs -->
		<!-- Modal -->
		<div id="assign-dev-modal" class="modal fade" role="dialog">
			<div class="modal-dialog">
				<!-- Modal content-->
				<div class="modal-content">
					<div class="modal-header">
						<button type="button" class="close" data-dismiss="modal">&times;</button>
						<h4 class="modal-title">Assign to a Developer</h4>
					</div>
					<div class="">
						<form id="assign-dev-form" class="solo-form">
							<div class="input-group center-block">
								<div class="input-group">
									<input type="text" class="form-control" required id="dev-id" placeholder="Developer User ID">
									<span class="input-group-btn">
										<button type="button" id="assign-button" class="btn btn-default">Assign</button>
									</span>
								</div>
							</div>
						</form>
					</div>
					<div class="modal-footer">
						<span id="dev-name"></span>
						<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
					</div>
				</div>
			
			</div>
		</div>
		
		<?php require('../includes/scripts.php'); ?>
	</body>
</html>