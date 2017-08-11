/* --
 * LOGIN
 */

// Login button creates post request to check details
if (window.location.pathname == "/login/") {
	$('#login-form').submit(function () {
		var loggedIn = false;
		
		$.ajax({
			type:    'POST',
			url:     '../cgi-bin/login-check.cgi',
			data:    $("form").serialize(),
			async:   false,
			success: function (data) {
				var parsedData = jQuery.parseJSON(data);
				
				if (parsedData.status == "VALID") {
					error_close($('.alert'));
					loggedIn = true;
				} else if (parsedData.status == "INVALID") {
					error($(".alert"), "Failed.", "Invalid username or password", "danger");
					loggedIn = false;
				} else {
					error($(".alert"), "Error.", "Something went wrong", "danger");
					loggedIn = false;
				}
			}
		});
		
		return loggedIn;
	});
}

// Error alert update. This is used for both login and register
function error(item, title, message, type) {
	
	item.html("<strong>" + title + "</strong> " + message);
	var oldClass = item.attr("class");
	item.removeClass();
	item.addClass("alert alert-" + type);
	
	if (item.is(':visible') && item.hasClass(oldClass)) {
		item.animate({'font-size': '18px'}, 150).animate({'font-size': '14px'}, 200);
	} else {
		item.slideDown();
	}
}

function error_close(item) {
	item.slideUp();
}

/* --
 * REGISTER
 */

// Register button creates post request to check details
$("#register-form").submit(function (event) {
	event.preventDefault();
	
	var inputText = $("form").serialize();
	
	if (inputText.search("%3C%2F") > -1 || inputText.search("%3Cscript%3E") > -1) {
		error($("#register-alert"), "Nope.", "Nice try though.", "danger");
		return;
	}
	
	//var ajaxRequest = $.post("../cgi-bin/register.cgi", $("form").serialize());
	
	// If the server replies success, reset the form.
	ajaxRequest.done(function (data) {
		var parsedData = jQuery.parseJSON(data);
		var alert = $("#register-alert");
		
		if (parsedData.status == "success") {
			error(alert, "Success.", "Registration successful. Taking you to login...", "success");
			setTimeout(function () {
				window.location.replace("/login");
			}, 1000);
			
		}
		else if (parsedData.status == "user_exists") {
			error(alert, "Failed.", "This user name is already registered.", "warning");
		}
		else if (parsedData.status == "email_registered") {
			error(alert, "Failed.", "This email is already registered.", "warning");
		}
		else if (parsedData.status == "password_mismatch") {
			error(alert, "Failed.", "Passwords do not match.", "warning");
		}
		else {
			error(alert, "Error.", "Something has gone wrong.", "danger");
		}
	});
});

/* --
 * DASHBOARD
 */

// Fills in dashboard panels with 5 latest bugs
if (window.location.pathname == "/dashboard/") {
	$.post("/cgi-bin/getAssigned.cgi", {n: "5"}, function (data) {
		fillTable($("#owned-table"), data);
	});
	
	$.post("/cgi-bin/getUnassigned.cgi", {n: "5"}, function (data) {
		fillTable($("#unassigned-table"), data);
	});
	
	$.post("/cgi-bin/getLatestNBugs.cgi", {n: "10"}, function (data) {
		fillTable($("#all-table"), data);
	});
	
	$.post("/cgi-bin/getFollowing.cgi", {n: "5"}, function (data) {
		fillTable($("#following-table"), data);
	});
}

// Fills the lists windows
if (window.location.pathname == "/lists/") {
	$.post("/cgi-bin/getLatestNBugs.cgi", {n: "5000"}, function (data) {
		fillTable($("#all-list-table"), data);
	});
}

if (window.location.pathname == "/lists/owned/") {
	$.post("/cgi-bin/getAssigned.cgi", {n: "50"}, function (data) {
		fillTable($("#owned-list-table"), data);
	});
}

if (window.location.pathname == "/lists/unassigned/") {
	$.post("/cgi-bin/getUnassigned.cgi", {n: "50"}, function (data) {
		fillTable($("#unassigned-list-table"), data);
	});
}

if (window.location.pathname == "/lists/following/") {
	$.post("/cgi-bin/getFollowing.cgi", {n: "50"}, function (data) {
		fillTable($("#following-list-table"), data);
	});
}

if (window.location.pathname == "/lists/users/") {
	$.post("/cgi-bin/getAllUsers.cgi", function (data) {
		fillUserTable($("#user-list-table"), data);
	});
}

function fillTable(inputTable, data) {
	var parsedBugs = jQuery.parseJSON(data);
	inputTable.find(".loading-icon").remove();
	
	var aLink = inputTable.parent().siblings(".panel-heading");
	if (aLink.has("a").length) {
		aLink = aLink.children("a");
	}
	
	var resCountRegex = /\([0-9]*\)/;
	
	if (aLink.text().search(resCountRegex) > -1) {
		aLink.text(aLink.text().replace(resCountRegex, ""));
	}
	
	if (parsedBugs.bugs == undefined) {
		aLink.text(aLink.text() + " (0)");
	}
	else {
		aLink.text(aLink.text() + " (" + parsedBugs.bugs.length + ")");
	}
	
	if (parsedBugs.status == "not found") {
		inputTable.append(
			"<tr>" +
			"<td colspan='4' class='text-center'><em>No bugs found</em></td>" +
			"</tr>"
		);
	}
	else {
		$.each(parsedBugs.bugs, function (i, item) {
			inputTable.append(
				"<tr>" +
				"<td><a href='/detail/?" + escapeHtml(item.bug_id) + "'>" + escapeHtml(item.bug_id) + "</a></td>" +
				"<td class='hidden-xs'>" + escapeHtml(item.component) + "</td>" +
				"<td class='hidden-xs'>" + escapeHtml(item.bug_severity) + "</td>" +
				"<td>" + escapeHtml(item.short_desc) + "</td>" +
				"</tr>"
			);
		});
	}
}

// Dashboard button actions
var dashboardCollapsed = false;
$("#collapse").click(function () {
	if (dashboardCollapsed) {
		$(this).text("Collapse");
		$(".collapse").stop().slideDown();
		dashboardCollapsed = false;
	} else {
		$(this).text("Expand");
		$(".collapse").stop().slideUp();
		dashboardCollapsed = true;
	}
	invertChevron(".panel-body");
});

$('.panelHeader').click(function () {
	
	var panel = "#" + ($(this).data('id'));
	
	if ($(panel).is(':visible')) {
		$(panel).stop().slideUp();
	} else {
		$(panel).stop().slideDown();
	}
	invertChevron(panel);
});

// Inverts the chevrons when the panels are collapsed/expanded
function invertChevron(panel) {
	$(panel).each(function () {
		
		var spanTag = $(this).siblings(".panel-heading").find("span");
		
		if ($(this).css("height") != "0px") {
			spanTag.removeClass("glyphicon-chevron-up");
			spanTag.addClass("glyphicon-chevron-down");
		} else {
			spanTag.removeClass("glyphicon-chevron-down");
			spanTag.addClass("glyphicon-chevron-up");
		}
	});
}

// Enables dashboard
$('[data-toggle="tooltip"]').tooltip();

/* --
 * DETAILS
 */

if (window.location.pathname == "/detail/") {
	var bug_ID = $(location).attr("search").slice(1);
	$("#details-title-heading").text("Details for bug #" + bug_ID);
	$.post("/cgi-bin/getBug.cgi", {bug_id: bug_ID}, function (data) {
		var parsedInfo = jQuery.parseJSON(data);
		var panel = $("#details-table");
		panel.find(".loading-icon").remove();
		if (parsedInfo.status == "found") {
			$.each(parsedInfo.bug, function (id, obj) {
				var recordData = obj;
				var key = id;
				
				if (key == "votes") {
					recordData = "<button type='button' onclick='decrementVotes()' id='decrement-votes' class='btn btn-link'><span class='glyphicon glyphicon-minus'></span></button>   " +
						"<span id='votes-value'>" + recordData + "</span>" +
						"   <button type='button' onclick='incrementVotes()' id='increment-votes' class='btn btn-link'><span class='glyphicon glyphicon-plus'></span></button>"
				}
				addTableRow(panel, key, recordData);
			});
			
			$.post("/cgi-bin/getComments.cgi", {id: bug_ID, src: "bug"}, function (commentData) {
				var parsedComments = jQuery.parseJSON(commentData);
				
				if (parsedComments.status == "found") {
					$.each(parsedComments.comments, function (id, topObj) {
						$("#comments-panel").append(
							"<div class='panel panel-default center-block'>" +
							"   <div class='panel-body'>" +
							"       <p>" + topObj.thetext + "</p>" +
							"       <p>- " + topObj.who + ", " + topObj.bug_when + "</p>" +
							"   </div>" +
							"</div>"
						);
					});
					
					$("#comments-panel").append(
						"<h4>New Comment</h4>" +
						"<form onsubmit='event.preventDefault(); postNewComment()'>" +
						"   <textarea class='form-control' rows='3' placeholder='Comment'></textarea>" +
						"   <button type='submit' class='btn btn-default pull-right'>Submit</button>" +
						"</form>"
					);
				}
			});
			
			$.post("/cgi-bin/getAttachments.cgi", {bug_id: bug_ID}, function (attachmentData) {
				var parsedAttachments = jQuery.parseJSON(attachmentData);
				if (parsedAttachments.status == "found") {
					$.each(parsedAttachments.attachments, function (id, topObj) {
						$("#attachments-panel").append(
							"<div class='panel panel-default center-block'>" +
							"   <div class='panel-body'>" +
							"       <p>Attachment #" + escapeHtml(topObj.attachid) + " - " + escapeHtml(topObj.attacher) + ", " + escapeHtml(topObj.date) + "</p>" +
							"       <p><a href='#'>" + escapeHtml(topObj.filename) + "</a></p>" +
							"       <p>" + escapeHtml(topObj.desc) + "</p>" +
							"   </div>" +
							"</div>"
						);
					});
				}
				$("#attachments-panel").append(
					"<h4>New Attachment</h4>" +
					"<form onsubmit='event.preventDefault(); postNewAttachment()'>" +
					"   <div class='input-group'>" +
					"   	<label class='input-group-btn'>" +
					"          <span class='btn btn-default'>" +
					"               Browse&hellip; <input type='file' style='display: none;'>" +
					"           </span>" +
					"   	</label>" +
					"   	<input id='filename-display' class='form-control' readonly>" +
					"   </div>" +
					"   <textarea class='form-control' rows='3' placeholder='Description'></textarea>" +
					"   <button type='submit' class='btn btn-default pull-right'>Submit</button>" +
					"</form>"
				);
			});
		}
		else {
			var bodyString = "Bug number " + escapeHtml(bug_ID) + " was not found in the database";
			panel.append("<tr>" +
				"<td colspan='2'>" + escapeHtml(bodyString) + "</td>" +
				"</tr>"
			);
		}
		
	});
	
	function deleteBug() {
		var bugID = $(location).attr("search").slice(1);
		
		$.post("/cgi-bin/deleteReport.cgi", {bug_id: bugID}, function (data) {
			var parsedReply = $.parseJSON(data);
			if (parsedReply.status == "success") {
				window.location.replace("/dashboard");
			}
			else {
				window.alert("Failed: " + parsedReply.message);
			}
		});
	}
	
	function editBug() {
		var bugID = $(location).attr("search").slice(1);
		location.replace('/edit/?' + bugID);
	}
	
	function followBug() {
		var bugID = $(location).attr("search").slice(1);
		
		$.post("/cgi-bin/followBug.cgi", {bug_id: bugID}, function (data) {
			var parsedReply = $.parseJSON(data);
			if (parsedReply.status == "success") {
				window.alert("Bug followed");
			}
			else {
				window.alert("Failed: " + parsedReply.message);
			}
		});
	}
	
	function decrementVotes() {
		var bugID = $(location).attr("search").slice(1);
		$.post("/cgi-bin/alterBugRep.cgi", {bug_id: bugID, vote: "-"}, function (data) {
			var parsedResponse = $.parseJSON(data);
			if (parsedResponse.status == "success") {
				var number = $("#votes-value").text();
				$("#votes-value").text(parseInt(number) - 1);
			}
			else {
				window.alert("Failed: " + parsedResponse.message);
			}
		});
	}
	
	function incrementVotes() {
		var bugID = $(location).attr("search").slice(1);
		$.post("/cgi-bin/alterBugRep.cgi", {bug_id: bugID, vote: "+"}, function (data) {
			var parsedResponse = $.parseJSON(data);
			if (parsedResponse.status == "success") {
				var number = $("#votes-value").text();
				$("#votes-value").text(parseInt(number) + 1);
			}
			else {
				window.alert("Failed: " + parsedResponse.message);
			}
		});
	}
	
	$("#dev-id").keyup(function () {
		var userID = $(this).val();
		
		$.post("/cgi-bin/getUser.cgi", {id: userID}, function (data) {
			
			var parsedReply = $.parseJSON(data);
			if (parsedReply.status == "found") {
				$("#dev-name").text(parsedReply.user.displayname + " (" + parsedReply.user.username + ") ");
			}
		});
	});
	
	$("#assign-button").click(function () {
		var userID = $("#dev-id").val();
		var bugID = $(location).attr("search").slice(1);
		$.post("/cgi-bin/assignBug.cgi", {id: userID, bug_id: bugID}, function (data) {
			var parsedReply = $.parseJSON(data);
			if (parsedReply.status == "success") {
				window.location.reload(true);
			}
			else {
				$("#dev-id").val("ERROR: " + parsedReply.message);
			}
		})
	});
}

$(document).on('change', "#attachments-panel input:file", function () {
	var input = $(this);
	var label = input.val().replace(/\\/g, '/').replace(/.*\//, '');
	
	input.parents('.input-group').find("#filename-display").val(label);
});

function postNewComment() {
	var bugID = $(location).attr("search").slice(1);
	var textAreaOutput = $("#comments-panel textarea").val();
	$.post("/cgi-bin/createComment.cgi", {bug_id: bugID, thetext: textAreaOutput}, function (data) {
		
		var parsedData = $.parseJSON(data);
		if (parsedData.status == "success") {
			window.location.reload(true);
		}
		else {
			$("textarea").val("Error: " + parsedData.message);
		}
	});
}

function postNewAttachment() {
	var bugID = $(location).attr("search").slice(1);
	var filenameOutput = $("#filename-display").val();
	var textAreaOutput = $("#attachments-panel textarea").val();
	$.post("/cgi-bin/createAttachment.cgi", {
		bug_id:   bugID,
		filename: filenameOutput,
		desc:     textAreaOutput
	}, function (data) {
		
		var parsedData = $.parseJSON(data);
		if (parsedData.status == "success") {
			window.location.reload(true);
		}
		else {
			$("#attachments-panel textarea").val("Error: " + escapeHtml(parsedData.message));
		}
	});
}

/* --
 * REPORT
 */
if (window.location.pathname == "/report/") {
	$("#createBug-form").on("submit", function (event) {
		event.preventDefault();
		$.post("/cgi-bin/createBug.cgi", $(this).serialize(), function (data) {
			var parsedMsg = jQuery.parseJSON(data);
			if (parsedMsg.status == "success") {
				window.location.replace("/dashboard");
			}
		})
	})
}

/* --
 * NAVBAR
 */

$(".navbar-form").on("submit", function (event) {
	event.preventDefault();
	window.location.replace("/detail/?" + $(this).find("input[type=text]").val());
});

/* --
 * USERS
 */

if (window.location.pathname == "/users/") {
	var userID = $(location).attr("search").slice(1);
	if (userID == "") {
		userID = document.cookie.substr(3);
		
	}
	$("#title-heading").text("Details for user #" + userID);
	$.post("/cgi-bin/getUser.cgi", {id: userID}, function (data) {
		var parsedInfo = jQuery.parseJSON(data);
		var panel = $("#users-table");
		panel.find(".loading-icon").remove();
		if (parsedInfo.status == "found") {
			$.each(parsedInfo.user, function (id, obj) {
				var key = id;
				var recordData = obj.toString();
				if (key.match(/^is[A-Z]/)) {
					
					if (recordData == 1) {
						recordData = key.substring(2);
						key = "role";
						addTableRow(panel, key, recordData);
					}
				}
				else {
					if (key == "reputation") {
						recordData = "<button type='button' onclick='decrementReputation()' id='decrement-rep' class='btn btn-link'><span class='glyphicon glyphicon-minus'></span></button>   " +
							"<span id='rep-value'>" + recordData + "</span>" +
							"   <button type='button' onclick='incrementReputation()' id='increment-rep' class='btn btn-link'><span class='glyphicon glyphicon-plus'></span></button>"
					}
					else if (key == "active") {
						recordData ? recordData = "active" : recordData = "inactive";
						key = "status";
					}
					
					addTableRow(panel, key, recordData);
				}
			});
		}
		else {
			var bodyString = "User " + userID + " was not found in the database";
			panel.append("<tr>" +
				"<td colspan='2'>" + escapeHtml(bodyString) + "</td>" +
				"</tr>"
			);
		}
	});
	$.post("/cgi-bin/getComments.cgi", {id: userID, src: "user"}, function (commentData) {
		var parsedComments = jQuery.parseJSON(commentData);
		
		if (parsedComments.status == "found") {
			$.each(parsedComments.comments, function (id, topObj) {
				$("#comments-panel").append(
					"<div class='panel panel-default center-block'>" +
					"   <div class='panel-body'>" +
					"       <p>" + topObj.thetext + "</p>" +
					"       <p>- <a href=/detail/?" + topObj.bug_id + ">" + topObj.bug_id + "</a>, " + topObj.bug_when + "</p>" +
					"   </div>" +
					"</div>"
				);
			});
		}
		else {
			$("#comments-panel").append("<em>No comments found</em>");
		}
	});
}

function addTableRow(inputObject, firstTd, secondTd) {
	inputObject.append(
		"<tr>" +
		"<td>" + escapeHtml(firstTd) + "</td>" +
		"<td>" + escapeHtml(secondTd) + "</td>" +
		"</tr>"
	);
}

function incrementReputation() {
	var userID = $(location).attr("search").slice(1);
	$.post("/cgi-bin/alterUserRep.cgi", {id: userID, vote: "+"}, function (data) {
		var parsedResponse = $.parseJSON(data);
		if (parsedResponse.status == "success") {
			var number = $("#rep-value").text();
			$("#rep-value").text(parseInt(number) + 1);
		}
		else {
			window.alert("Failed: " + parsedResponse.message);
		}
	});
}

function decrementReputation() {
	var userID = $(location).attr("search").slice(1);
	$.post("/cgi-bin/alterUserRep.cgi", {id: userID, vote: "-"}, function (data) {
		var parsedResponse = $.parseJSON(data);
		if (parsedResponse.status == "success") {
			var number = $("#rep-value").text();
			$("#rep-value").text(parseInt(number) - 1);
		}
		else {
			window.alert("Failed: " + parsedResponse.message);
		}
	});
}

/* --
 * SEARCH
 */

if (window.location.pathname == "/search/") {
	$("#search-form").on("submit", function (event) {
		event.preventDefault();
		var term = $("#search-term").val();
		$("tr.loading-icon").slideDown();
		$.post("/cgi-bin/search.cgi", {search_term: term, n: 500}, function (data) {
			var parsedReply = $.parseJSON(data);
			fillTable($("#search-results-table"), data);
		});
	});
}

/* --
 * EDIT
 */
if (window.location.pathname == "/edit/") {
	var bug_ID = $(location).attr("search").slice(1);
	
	$.post("/cgi-bin/getBug.cgi", {bug_id: bug_ID}, function (data) {
		var parsedResponse = $.parseJSON(data);
		
		if (parsedResponse.status == "found") {
			$('input[name=bug_id]').val(parsedResponse.bug.bug_id);
			$('input[name=description]').val(parsedResponse.bug.short_desc);
			$('input[name=classification]').val(parsedResponse.bug.classification);
			$('input[name=product]').val(parsedResponse.bug.product);
			$('input[name=component]').val(parsedResponse.bug.component);
			$('input[name=version]').val(parsedResponse.bug.version);
			$('input[name=op_sys]').val(parsedResponse.bug.op_sys);
			$('.editSeverity').val(parsedResponse.bug.bug_severity);
		} else if (parsedResponse.status == "not found") {
			$(".login-content").html("<h3>Bug not found</h3>");
		}
	});
	
	$("#edit-bug-form").submit(function () {
		var bug_ID = $(location).attr("search").slice(1);
		$.post("/cgi-bin/editBug.cgi", {bug_id: bug_ID}, function (data) {
			var parsedReply = $.parseJSON(data);
			
			if (parsedReply.status != "success") {
				window.alert("Failed: " + parsedReply.message);
			}
			window.location = "/detail/?" + bug_ID;
		});
	});
}

// Lets hope this helps
function escapeHtml(string) {
	if (string.toString().indexOf("<script>") > -1) {
		var entityMap = {
			"&": "&amp;",
			"<": "&lt;",
			">": "&gt;",
			'"': '&quot;',
			"'": '&#39;',
			"/": '&#x2F;'
		};
		return String(string).replace(/[&<>"'\/]/g, function (s) {
			return entityMap[s];
		});
	}
	else {
		return string;
	}
	
}

function fillUserTable(inputTable, data) {
	
	var parsedBugs = jQuery.parseJSON(data);
	//
	inputTable.find(".loading-icon").remove();
	
	var aLink = inputTable.parent().siblings(".panel-heading");
	if (aLink.has("a").length) {
		aLink = aLink.children("a");
	}
	
	if (parsedBugs.user == undefined) {
		aLink.text(aLink.text() + " (0)");
	}
	else {
		aLink.text(aLink.text() + " (" + parsedBugs.user.length + ")");
	}
	
	if (parsedBugs.status == "not found") {
		inputTable.append(
			"<tr>" +
			"<td colspan='4' class='text-center'><em>No bugs found</em></td>" +
			"</tr>"
		);
	}
	else {
		$.each(parsedBugs.user, function (i, item) {
			inputTable.append(
				"<tr>" +
				"<td><a href='/users/?" + escapeHtml(item.id) + "'>" + escapeHtml(item.id) + "</a></td>" +
				"<td class='hidden-xs'>" + escapeHtml(item.displayname) + "</td>" +
				"<td class='hidden-xs'>" + escapeHtml(item.email) + "</td>" +
				"<td>" + escapeHtml(item.reputation) + "</td>" +
				"</tr>"
			);
		});
	}
}
/* --
 * EDIT
 */












