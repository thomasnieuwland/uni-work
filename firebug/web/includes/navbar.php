<nav class="navbar navbar-default navbar-fixed-top">
	<div class="container-fluid">
		<div class="navbar-header">
			<button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1" aria-expanded="false">
				<span class="sr-only">Toggle Navigation</span>
				<span class="icon-bar"></span>
				<span class="icon-bar"></span>
				<span class="icon-bar"></span>
			</button>
			<a href="/dashboard" class="navbar-brand">Firebug</a>
		</div>
		
		<!-- Nav links, forms, and other content for toggling -->
		<div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
			<ul class="nav navbar-nav">
				<li><a href="/dashboard">Home<span class="sr-only">(current)</span></a></li>
				<li><a href="/report">Report</a></li>
				<li><a href="/search">Search</a></li>
				<li class="dropdown">
					<a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">Lists <span class="caret"></span></a>
					<ul class="dropdown-menu">
						<li><a href="/lists/users">All Users</a></li>
						<li><a href="/lists">All Bugs</a></li>
						<li><a href="/lists/owned">Owned Bugs</a></li>
						<li><a href="/lists/unassigned">Unassigned Bugs</a></li>
					</ul>
				</li>
			</ul>
			<form class="navbar-form navbar-right hidden-xs">
				<div class="form-group">
					<input type="text" class="form-control" placeholder="Search Bug ID">
				</div>
				<button type="submit" class="btn btn-default"><span class="glyphicon glyphicon-search"></span></button>
				<a href="/users" class="btn btn-default"><span class="glyphicon glyphicon-user" data-toggle="tooltip" data-placement="bottom" title="My Profile"></span></a>
				<a href="/logout" class="btn btn-default"><span class="glyphicon glyphicon-log-out" data-toggle="tooltip" data-placement="bottom" title="Logout"></span></a>
			</form>
		</div>
	</div>
</nav>