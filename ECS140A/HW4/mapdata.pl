is_adjacent(portugal,spain).
is_adjacent(spain,portugal).
is_adjacent(spain,andorra).
is_adjacent(spain,france).
is_adjacent(andorra,spain).
is_adjacent(andorra,france).
is_adjacent(france,spain).
is_adjacent(france,andorra).
is_adjacent(france,belgium).
is_adjacent(france,luxembourg).
is_adjacent(france,germany).
is_adjacent(france,switzerland).
is_adjacent(france,italy).
is_adjacent(belgium,netherlands).
is_adjacent(belgium,germany).
is_adjacent(belgium,luxembourg).
is_adjacent(belgium,france).
is_adjacent(luxembourg,belgium).
is_adjacent(luxembourg,germany).
is_adjacent(luxembourg,france).
is_adjacent(germany,france).
is_adjacent(germany,luxembourg).
is_adjacent(germany,belgium).
is_adjacent(germany,netherlands).
is_adjacent(germany,denmark).
is_adjacent(germany,poland).
is_adjacent(germany,czechia).
is_adjacent(germany,austria).
is_adjacent(germany,switzerland).
is_adjacent(switzerland,france).
is_adjacent(switzerland,germany).
is_adjacent(switzerland,austria).
is_adjacent(switzerland,italy).
is_adjacent(italy,france).
is_adjacent(italy,switzerland).
is_adjacent(italy,austria).
is_adjacent(italy,slovenia).
is_adjacent(netherlands,belgium).
is_adjacent(netherlands,germany).
is_adjacent(denmark,germany).
is_adjacent(poland,germany).
is_adjacent(poland,czechia).
is_adjacent(poland,slovakia).
is_adjacent(poland,ukraine).
is_adjacent(poland,belarus).
is_adjacent(poland,lithuania).
is_adjacent(czechia,germany).
is_adjacent(czechia,poland).
is_adjacent(czechia,slovakia).
is_adjacent(czechia,austria).
is_adjacent(austria,germany).
is_adjacent(austria,czechia).
is_adjacent(austria,slovakia).
is_adjacent(austria,hungary).
is_adjacent(austria,slovenia).
is_adjacent(austria,switzerland).
is_adjacent(austria,italy).
is_adjacent(slovenia,italy).
is_adjacent(slovenia,austria).
is_adjacent(slovenia,hungary).
is_adjacent(slovenia,croatia).
is_adjacent(slovakia,poland).
is_adjacent(slovakia,czechia).
is_adjacent(slovakia,austria).
is_adjacent(slovakia,hungary).
is_adjacent(slovakia,ukraine).
is_adjacent(ukraine,belarus).
is_adjacent(ukraine,poland).
is_adjacent(ukraine,slovakia).
is_adjacent(ukraine,hungary).
is_adjacent(ukraine,romania).
is_adjacent(ukraine,moldova).
is_adjacent(belarus,poland).
is_adjacent(belarus,lithuania).
is_adjacent(belarus,latvia).
is_adjacent(belarus,ukraine).
is_adjacent(lithuania,poland).
is_adjacent(lithuania,belarus).
is_adjacent(lithuania,latvia).
is_adjacent(romania,hungary).
is_adjacent(romania,ukraine).
is_adjacent(romania,moldova).
is_adjacent(romania,bulgaria).
is_adjacent(romania,serbia).
is_adjacent(moldova,ukraine).
is_adjacent(moldova,romania).
is_adjacent(latvia,lithuania).
is_adjacent(latvia,belarus).
is_adjacent(latvia,estonia).
is_adjacent(bulgaria,romania).
is_adjacent(bulgaria,serbia).
is_adjacent(bulgaria,macedonia).
is_adjacent(bulgaria,greece).
is_adjacent(serbia,romania).
is_adjacent(serbia,bulgaria).
is_adjacent(serbia,macedonia).
is_adjacent(serbia,albania).
is_adjacent(serbia,montenegro).
is_adjacent(serbia,bosnia).
is_adjacent(serbia,croatia).
is_adjacent(serbia,hungary).
is_adjacent(estonia,latvia).
is_adjacent(macedonia,bulgaria).
is_adjacent(macedonia,greece).
is_adjacent(macedonia,albania).
is_adjacent(macedonia,serbia).
is_adjacent(greece,albania).
is_adjacent(greece,macedonia).
is_adjacent(greece,bulgaria).
is_adjacent(croatia,slovenia).
is_adjacent(croatia,hungary).
is_adjacent(croatia,serbia).
is_adjacent(croatia,bosnia).
is_adjacent(albania,montenegro).
is_adjacent(albania,serbia).
is_adjacent(albania,macedonia).
is_adjacent(albania,greece).
is_adjacent(montenegro,bosnia).
is_adjacent(montenegro,serbia).
is_adjacent(montenegro,albania).
is_adjacent(bosnia,croatia).
is_adjacent(bosnia,serbia).
is_adjacent(bosnia,montenegro).
is_adjacent(hungary,austria).
is_adjacent(hungary,slovakia).
is_adjacent(hungary,ukraine).
is_adjacent(hungary,romania).
is_adjacent(hungary,serbia).
is_adjacent(hungary,croatia).
is_adjacent(hungary,slovenia).

color(red).
color(white).
color(blue).
color(gold).

getHead([H|T],H).
getTail([H|T],T).
get(X,T) :- getTail(X,Y), getHead(Y,T). /* Gets Color of item X and returns as T */

noDupColor(First,X,T) :- is_adjacent(First,Second), isMember(Second,X,Country2), secondColor(Country2,SC), color(T), T\==SC.
noDupColor(First,X,T) :- is_adjacent(First,Second), bogus(Second,X), color(T).

secondColor(Second,SC) :- get(Second,SC).
isMember(Country,X,T) :- member(T,X), getHead(T,W), W==Country.

bogus(Country, X) :- \+isMember(Country,X,T).


putColor(X,X1) :- is_adjacent(First,Second), bogus(First,X), isMember(Second,X,T), findNonColor(X,First,W), append([First],[W],TEMP), append([TEMP],X,X1).

findNonColor(X,Country,T) :- getMembers(X,Country,Colors),color(T), \+sublist([T],Colors).

mapcolor(X) :- buildMap([[serbia,gold]],1,H), sort(H,X).

buildMap(X,31,X).
buildMap(X,Count,W3) :- putColor(X,W), NewCount is Count + 1, buildMap(W,NewCount,W3).

allMembers(X,Country,List,List2) :- is_adjacent(Country,C), bogus(C,X).
allMembers(X,Country,List,NewList) :- is_adjacent(Country,C), isMember(C,X,Country2), secondColor(Country2,SC), append([SC],List,NewList), del(Country2,X,X1), allMembers(X1,Country,NewList,List1).

getMembers(X,Country,T) :- findall(C,is_adjacent(Country,C),This), getInfo(This,X,[],T).

getInfo([],X,New,New).
getInfo([Head|Tail],X,New,Copy) :- bogus(Head,X), getInfo(Tail,X,New,Copy).
getInfo([Head|Tail],X,New,Copy) :- isMember(Head,X,T), secondColor(T,SC), append([SC],New,Copy1), getInfo(Tail,X,Copy1,Copy).

del(X,[X|Tail],Tail).    
del(X,[Y|Tail],[Y|Tail1]):- del(X,Tail,Tail1).





