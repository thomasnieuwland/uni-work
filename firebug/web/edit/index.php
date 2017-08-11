<?php
	require('../includes/loginValidation.php');
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Firebug | Edit Bug</title>
		<?php require('../includes/head.php'); ?>
	</head>
<body>
	<?php require("../includes/navbar.php"); ?>
    <div class="container">
        <div class="login-box">
            <div class="login-header">Editing Bug</div>
            <div class="login-content">

                <!--
                POST inputs: bug_id, short_desc, classification, product,
                 component, version, op_sys, bug_severity
                 Maintain bug_id somehow, rest should be below -->

                <form method="POST" id="edit-bug-form">
                    <input readonly name="bug_id" value="(BUG ID)">
                    <input type="text" required name="description" value="">
                    <input type="text" name="classification" value="">
                    <input type="text" name="product" value="">
                    <input type="text" name="component" value="">
                    <input type="text" name="version" value="">
                    <input type="text" name="op_sys" value="">
                    <select name="bug_severity" class="form-control editSeverity">
                        <option value="normal">Normal</option>
                        <option value="enhancement">Enhancement</option>
                        <option value="minor">Minor</option>
                        <option value="critical">Critical</option>
                        <option value="trivial">Trivial</option>
                        <option value="major">Major</option>
                        <option value="blocker">Blocker</option>
                    </select>
                    <button type="submit" id="register-button" class="btn btn-default">Submit Changes</button>
                </form>
            </div>
            <div class="register">
                <h6><a href="/dashboard">Cancel</a></h6>
            </div>
        </div>
    </div>
    <?php require('../includes/scripts.php'); ?>
</body>
