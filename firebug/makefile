CC = g++
IFLAGS = -I/usr/include/cppconn -I/usr/include/json
LFLAGS = -L/usr/lib
LIBFLAGS = -lmysqlcppconn -lcgicc
OPTFLAG = -std=c++11

TARGETS = getAllUsers login-check banUser editBugStatus createAttachment createBug createComment deleteComment deleteAttachment deleteReport deleteUser deleteFollowing editAttachment editBug editBugStatus editProfile getBug getComments getLatestNBugs getUser giveReputation login register getAssigned getAttachments assignBug search followBug getUnassigned getFollowing getReported alterUserRep alterBugRep

all: ${TARGETS}

getAllUsers:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getAllUsers.cgi cpp/getAllUsers.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

login-check:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/login-check.cgi cpp/login-check.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

alterBugRep:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/alterBugRep.cgi cpp/alterBugRep.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

alterUserRep:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/alterUserRep.cgi cpp/alterUserRep.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

banUser:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/banUser.cgi cpp/banUser.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

createAttachment:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/createAttachment.cgi cpp/createAttachment.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

createBug:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/createBug.cgi cpp/createBug.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

createComment:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/createComment.cgi cpp/createComment.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

deleteComment:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/deleteComment.cgi cpp/deleteComment.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

deleteAttachment:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/deleteAttachment.cgi cpp/deleteAttachment.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

deleteReport:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/deleteReport.cgi cpp/deleteReport.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

deleteUser:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/deleteUser.cgi cpp/deleteUser.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

deleteFollowing:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/deleteFollowing.cgi cpp/deleteFollowing.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

editAttachment:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/editAttachment.cgi cpp/editAttachment.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

editBug:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/editBug.cgi cpp/editBug.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

editBugStatus:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/editBugStatus.cgi cpp/editBugStatus.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

editProfile:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/editProfile.cgi cpp/editProfile.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getBug:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getBug.cgi cpp/getBug.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getComments:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getComments.cgi cpp/getComments.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getLatestNBugs:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getLatestNBugs.cgi cpp/getLatestNBugs.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getUser:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getUser.cgi cpp/getUser.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

giveReputation:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/giveReputation.cgi cpp/giveReputation.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

login:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/login.cgi cpp/login.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

register:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/register.cgi cpp/register.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getAssigned:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getAssigned.cgi cpp/getAssigned.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getAttachments:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getAttachments.cgi cpp/getAttachments.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

assignBug:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/assignBug.cgi cpp/assignBug.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

search:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/search.cgi cpp/search.cpp cpp/Bug.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

followBug:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/followBug.cgi cpp/followBug.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getUnassigned:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getUnassigned.cgi cpp/getUnassigned.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getFollowing:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getFollowing.cgi cpp/getFollowing.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

getReported:
	${CC} -Wall ${IFLAGS} -o web/cgi-bin/getReported.cgi cpp/getReported.cpp ${LFLAGS} ${LIBFLAGS} ${OPTFLAG}

clean:
	rm web/cgi-bin/*.cgi
