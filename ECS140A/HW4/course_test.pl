course(ecs10, [], 3).
course(ecs15, [], 2).
course(ecs20, [], 2).
course(ecs30, [ecs10], 3).
course(ecs40, [ecs30], 4).
course(ecs50, [ecs40], 4).
course(ecs60, [ecs20, ecs40], 4).
course(ecs113, [ecs15], 2).
course(ecs115, [ecs30], 2).
course(ecs116, [ecs10], 2).
course(ecs120, [ecs20], 3).
course(ecs122a, [ecs20, ecs60], 3).
course(ecs122b, [ecs122a], 4).
course(ecs124, [ecs30], 3).
course(ecs127, [ecs20], 3).
course(ecs129, [], 3).
course(ecs130, [ecs30], 4).
course(ecs132, [ecs40,ecs50], 3).
course(ecs140a, [ecs50, ecs60], 4).
course(ecs140b, [ecs140a], 4).
course(ecs142, [ecs20,ecs140a], 4).
course(ecs145, [ecs60], 3).
course(ecs150, [ecs40,ecs50], 4).
course(ecs152a, [ecs60,ecs132], 4).
course(ecs152b, [ecs152a], 4).
course(ecs152c, [ecs152b], 4).
course(ecs153, [ecs150,ecs152a], 4).
course(ecs154a, [ecs50], 4).
course(ecs154b, [ecs154a], 4).
course(ecs158, [ecs60,ecs150], 4).
course(ecs160, [ecs140a], 4).
course(ecs163, [ecs60], 4).
course(ecs165a, [ecs60], 4).
course(ecs165b, [ecs165a], 4).
course(ecs170, [ecs60], 4).
course(ecs171, [ecs60, ecs132], 4).
course(ecs173, [ecs60], 4).
course(ecs174, [ecs60, ecs132], 4).
course(ecs175, [ecs60], 4).
course(ecs177, [ecs175], 4).
course(ecs178, [ecs175], 4).


student(ann, [ecs60, ecs120], [ecs10, ecs20, ecs30, ecs40]).
student(bob, [ecs60, ecs120], [ecs10, ecs30, ecs40]).
student(chris, [ecs140a, ecs150], [ecs10, ecs20, ecs30, ecs40, ecs50, ecs60, ecs154a]).
student(diane, [ecs10, ecs20], []).
student(john, [ecs160, ecs120], [ecs140a]).
student(jane, [ecs120, ecs140a], [ecs10, ecs20, ecs30, ecs40, ecs50, ecs60, ecs122a]).
student(james, [ecs50], [ecs10, ecs30, ecs40]).
student(sally, [ecs153, ecs120], [ecs10, ecs20, ecs30, ecs40, ecs50, ecs150]).


instructor(nitta, [ecs140a, ecs150, ecs160]).
instructor(davis, [ecs60]).
instructor(gysel, [ecs20, ecs120]).
instructor(butner, [ecs10, ecs50]).
instructor(bishop, [ecs153]).

course3or4(X) :- course(X,_,3); course(X,_,4).
course5 :- course(X,_,3).




findStudent(X,Y) :- student(Y,[X],_).
findStudent(X,Y) :- student(Y,[X,_],_).
findStudent(X,Y) :- student(Y,[_,X],_).
findCourse(X,Y) :- course(X,_,_), findStudent(X,Y).


preReq(X,Y) :- course(X,[Y],_).
preReq(X,Y) :- course(X,[Y,_],_).
preReq(X,Y) :- course(X,[_,Y],_).
allreq(X,Y) :- preReq(X,Y).
allreq(X,Y) :- preReq(X,T), preReq(T,Y).
allreq(X,Y) :- preReq(X,T), preReq(T,W), preReq(W,Y).
allreq(X,Y) :- preReq(X,T), preReq(T,W), preReq(W,H), preReq(H,Y).
allreq(X,Y) :- preReq(X,A), preReq(A,B), preReq(B,C), preReq(C,D), preReq(D,Y).
allreq(X,Y) :- preReq(X,A), preReq(A,B), preReq(B,C), preReq(C,D), preReq(D,E), preReq(E,Y).


getAllStudent(X,Y,T) :- student(X,[Y],_), allprereq(Y,T).
getAllStudent(X,Y,T) :- student(X,[Y,_],_), allprereq(Y,T).
getAllStudent(X,Y,T) :- student(X,[_,Y],_), allprereq(Y,T).
getAllStudent(X,Y,T) :- student(X,[Y,_,_],_), allprereq(Y,T).
getAllStudent(X,Y,T) :- student(X,[_,Y,_],_), allprereq(Y,T).
getAllStudent(X,Y,T) :- student(X,[_,_,Y],_), allprereq(Y,T).

curPre(X,Y) :- student(X,_,Y).

thisAllStu(X,Y,T) :- getAllStudent(X,Y,T), curPre(X,W), \+sublist(T,W).

preReq1(Y,X) :- course(X,[Y],_).
preReq1(Y,X) :- course(X,[Y,_],_).
preReq1(Y,X) :- course(X,[_,Y],_).

thisSub :- sublist(student(X,_,Y),student(X,_,W)).
getPreStudent(X,Y) :- getAllStudent(X,Y), sublist(student(X,_,R), student(X,_,Y)).
studentPre(X,T) :- sublist(allprereq(X,T), getPreStudent(W,T)).

studentCourse(T,Y) :- student(T,X,_), instructor(Y,[W]), member(W,X).
studentCourse(T,Y) :- student(T,X,_), instructor(Y,[W,_]), member(W,X).
studentCourse(T,Y) :- student(T,X,_), instructor(Y,[_,W]), member(W,X).
studentCourse(T,Y) :- student(T,X,_), instructor(Y,[W,_,_]), member(W,X).
studentCourse(T,Y) :- student(T,X,_), instructor(Y,[_,W,_]), member(W,X).
studentCourse(T,Y) :- student(T,X,_), instructor(Y,[_,_,W]), member(W,X).


students_prereq(X) :- student(Y,_,_), findall(Y,students_prereq1(Y),T), sort(T,X), \+sublist(X,[]).
testMem(X) :- instructor(Y,X), course(T,_,_), member(T,X).

getImPre(X,Y) :- course(X,[Y],_).

fc_course(Y) :- findall(X,course3or4(X),Y).
imprereq(X,Z) :- course(X,_,_), findall(Y,preReq1(X,Y),Z),\+sublist(Z,[]).
students(T,X) :- course(X,_,_), findall(Y,findStudent(X,Y),T), \+sublist(T,[]).
students_prereq1(X) :- getAllStudent(X,Y,T), curPre(X,W), \+sublist(T,W).
allprereq(T,X) :- course(X,_,_), findall(Y,allreq(X,Y),Y), sort(Y,T).
student_teach(X,W) :- student(X,_,_), findall(Y,studentCourse(X,Y),T), sort(T,W).

