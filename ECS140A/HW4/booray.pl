rank(2).
rank(3).
rank(4).
rank(5).
rank(6).
rank(7).
rank(8).
rank(9).
rank(t).
rank(j).
rank(q).
rank(k).
rank(a).

player(1).
player(2).
value(2, 2).
value(3, 3).
value(4, 4).
value(5, 5).
value(6, 6).
value(7, 7).
value(8, 8).
value(9, 9).
value(10, 10).
value(11, 11).
value(12, 12).
value(13, 13).
value(1, 14).

suite(d).
suite(h).
suite(c).
suite(s).
assignW(T,W) :- W is 1.
assignW(T,W) :- W is 2.

getHead([H|T],H).
getTail([H|T],T).
getAllStart([H|[M|T]],H,M,T). /*gets Trump card as L, P1 cards as M, and P2 cards as S*/

getCardCounter(X,H,C) :- get(X,T),write(T), get(H,C),write(C), T=C. 

getOfSuite(X,P,T) :- member(T,P), get(T,K), K==X. /* X is player1 suite, P is player 2 hand, T is returned card player2 */
getOfNonSuiteNonTrump(X,P,T,E) :- member(T,P), get(T,K), K\==X, K\==E. /* Get nonSuite of nonTrumpSuite of E. X is P1 suite, P is P2 hand, T is returned hand P2, E is Trump Suite */
getOfNonSuiteTrump(X,P,T,E) :- member(T,P), get(T,K), K\==X, K==E. /* Get nonSuite of TrumpSuite of E. X is P1 hand, P is P2 hand, T is returned hand P2, E is Trump Suite */

rank_bigger(X,Y,RE) :- getHead(X,T), getHead(Y,W), value(T, A), value(W, B), A > B, RE is 1.
rank_bigger(X,Y,RE) :- getHead(X,T), getHead(Y,W), value(T, A), value(W, B), A < B, RE is 2.

getThis(X,Y,W) :- \+rank_bigger(X,Y), W is 1.

player1Won(X,Y,Q,E,T,P,TF,WIN) :- getLeadHand(X,T,Q,E,TFG), get(T,G), getOfSuite(G,Y,P), rank_bigger(T,P,RE), RE==1, WIN is 1, TF is TFG. /* If P1 wins: X is P1 hand, Y is P2 hand, W is outcome, Q is Trump flag */
player1Won(X,Y,Q,E,T,P,TF,WIN) :- getLeadHand(X,T,Q,E,TFG), get(T,G), getOfSuite(G,Y,P), rank_bigger(T,P,RE), RE==2, WIN is 2, TF is TFG. /* If P2 wins: X is P1 hand, Y is P2 hand, W is outcome, Q is Trump flag  */
player1Won(X,Y,Q,E,T,P,TF,WIN) :- getLeadHand(X,T,Q,E,TFG), get(T,G), checkHand(Y,[],Ret), \+sublist([G],Ret), getOfNonSuiteNonTrump(G,Y,P,E), WIN is 1, TF is TFG. /* If P1 wins: X is P1 hand, Y is P2 hand, W is outcome. Auto win P1 since nonSuite nonTrump, Q is Trump Flag */
player1Won(X,Y,Q,E,T,P,TF,WIN) :- getLeadHand(X,T,Q,E,TFG), get(T,G), checkHand(Y,[],Ret), \+sublist([G],Ret), getOfNonSuiteTrump(G,Y,P,E), WIN is 2, TF is 1. /* If P1 wins: X is P1 hand, Y is P2 hand, W is outcome, Q is Trump Flag, P2 played Trump wins */


higherCard(X,Y) :- rank_bigger(X,Y).


ifTwoTrump(J,A) :- getHead(J,R), R==2, append([2,g],[],A). /*If Trump suit is 2, then no Trump suit. Make bogus suit */
ifTwoTrump(J,A) :- getHead(J,R), R\==2, append(J,[],A). /*If Trump suit is not 2, then Trump suit. Keep suit */


player2Won(X,Y,Q,E,T,P,TF,WIN) :- getLeadHand(Y,P,Q,E,TFG), get(P,G), getOfSuite(G,X,T), rank_bigger(P,T,RE), RE==1, WIN is 2, TF is TFG. /* If P2 wins: X is P1 hand, Y is P2 hand, W is outcome, Q is Trump flag */
player2Won(X,Y,Q,E,T,P,TF,WIN) :- getLeadHand(Y,P,Q,E,TFG), get(P,G), getOfSuite(G,X,T), rank_bigger(P,T,RE), RE==2, WIN is 1, TF is TFG. /* If P1 wins: X is P1 hand, Y is P2 hand, W is outcome, Q is Trump flag */
player2Won(X,Y,Q,E,T,P,TF,WIN) :- getLeadHand(Y,P,Q,E,TFG), get(P,G), checkHand(X,[],Ret), \+sublist([G],Ret), getOfNonSuiteNonTrump(G,X,T,E), WIN is 2, TF is TFG. /* If P2 wins: X is P1 hand, Y is P2 hand, W is outcome. Auto win P2 since nonSuite nonTrump, Q is Trump Flag */
player2Won(X,Y,Q,E,T,P,TF,WIN) :- getLeadHand(Y,P,Q,E,TFG), get(P,G), checkHand(X,[],Ret), \+sublist([G],Ret), getOfNonSuiteTrump(G,X,T,E), WIN is 1, TF is 1. /* If P1 wins: X is P1 hand, Y is P2 hand, W is outcome, Q is Trump Flag, P1 played Trump wins */

testOut(X,Y) :- X==c, Y is 7.

getLeadHand(X,T,Q,E,TFG) :- member(T,X), get(T,Y), Q==0, Y\==E, TFG is 0. /* If Trump card not played, get card of non Trump Suite. X is hand, T is card, Q is Trump flag, E is Trump Suite */
getLeadHand(X,T,Q,E,TFG) :- member(T,X), get(T,Y), Q==1, Y==E, TFG is 1. /* If Trump not played, but no other cards, set Trump Flag and get card of Trump Suite. X is hand, T is card, Q is Trump flag, E is Trump Suite */
getLeadHand(X,T,Q,E,TFG) :- member(T,X), get(T,Y), Q==1, Y==E, TFG is 1. /* If Trump card played, get card of Trump Suite. X is hand, T is card, Q is Trump flag, E is Trump Suite */
getLeadHand(X,T,Q,E,TFG) :- member(T,X), get(T,Y), Q==1, Y\==E, TFG is 1. /* If Trump card played, cannot get card of Trump Suite if want. X is hand, T is card, Q is Trump flag, E is Trump Suite */
getLeadHand(X,T,Q,E,TFG) :- member(T,X), get(T,Y), checkHand(X,[],Ret),same(Ret), Q==0, Y==E, TFG is 1. /* If Trump not played, but no other cards, set Trump Flag and get card of Trump Suite. X is hand, T is card, Q is Trump flag, E is Trump Suite */

get(X,T) :- getTail(X,Y), getHead(Y,T). /* Gets suite of card X and returns as T */
appendGame(ONE,TWO,RESULT,LIST) :- RESULT==1, append([t],[ONE],TEMP), append(TEMP,[TWO],LIST1), append([LIST1],[],LIST).
appendGame(ONE,TWO,RESULT,LIST) :- RESULT==2, append([f],[ONE],TEMP), append(TEMP,[TWO],LIST1), append([LIST1],[],LIST).


playGame(X,Y,W,Q,E,T,P,TF,WIN) :- W==1, player1Won(X,Y,Q,E,T,P,TF,WIN).
playGame(X,Y,W,Q,E,T,P,TF,WIN) :- W==2, player2Won(X,Y,Q,E,T,P,TF,WIN).

bourree(J,V,Y,X) :- untilWin(J,V,Y,X1,0,1,NEWP1,NEWP2,Q2,W2,DW1), DW1==1, untilWin(J,NEWP1,NEWP2,X2,Q2,W2,NEWP3,NEWP4,Q3,W3,DW2), DW2==1, untilWin(J,NEWP3,NEWP4,X3,Q3,W3,NEWP5,NEWP6,Q4,W4,DW3), DW3==1, append(X1,X2,SOM), append(SOM,X3,SOM1), append(SOM1,[[3,0]],X),!.

bourree(J,V,Y,X) :- untilWin(J,V,Y,X1,0,1,NEWP1,NEWP2,Q2,W2,DW1), DW1==0, untilWin(J,NEWP1,NEWP2,X2,Q2,W2,NEWP3,NEWP4,Q3,W3,DW2), DW2==1, untilWin(J,NEWP3,NEWP4,X3,Q3,W3,NEWP5,NEWP6,Q4,W4,DW3), DW3==1, untilWin(J,NEWP5,NEWP6,X4,Q4,W4,NEWP7,NEWP8,Q5,W5,DW4), DW4==1, append(X1,X2,SOM), append(SOM,X3,SOM1), append(SOM1,X4,SOM2), append(SOM2,[[3,1]],X),!.

bourree(J,V,Y,X) :- untilWin(J,V,Y,X1,0,1,NEWP1,NEWP2,Q2,W2,DW1), DW1==1, untilWin(J,NEWP1,NEWP2,X2,Q2,W2,NEWP3,NEWP4,Q3,W3,DW2), DW2==0, untilWin(J,NEWP3,NEWP4,X3,Q3,W3,NEWP5,NEWP6,Q4,W4,DW3), DW3==1, untilWin(J,NEWP5,NEWP6,X4,Q4,W4,NEWP7,NEWP8,Q5,W5,DW4), DW4==1, append(X1,X2,SOM), append(SOM,X3,SOM1), append(SOM1,X4,SOM2), append(SOM2,[[3,1]],X),!.

bourree(J,V,Y,X) :- untilWin(J,V,Y,X1,0,1,NEWP1,NEWP2,Q2,W2,DW1), DW1==1, untilWin(J,NEWP1,NEWP2,X2,Q2,W2,NEWP3,NEWP4,Q3,W3,DW2), DW2==1, untilWin(J,NEWP3,NEWP4,X3,Q3,W3,NEWP5,NEWP6,Q4,W4,DW3), DW3==0, untilWin(J,NEWP5,NEWP6,X4,Q4,W4,NEWP7,NEWP8,Q5,W5,DW4), DW4==1, append(X1,X2,SOM), append(SOM,X3,SOM1), append(SOM1,X4,SOM2), append(SOM2,[[3,1]],X),!.

bourree(J,V,Y,X) :- untilWin(J,V,Y,X1,0,1,NEWP1,NEWP2,Q2,W2,DW1), DW1==0, untilWin(J,NEWP1,NEWP2,X2,Q2,W2,NEWP3,NEWP4,Q3,W3,DW2), DW2==0, untilWin(J,NEWP3,NEWP4,X3,Q3,W3,NEWP5,NEWP6,Q4,W4,DW3), DW3==1, untilWin(J,NEWP5,NEWP6,X4,Q4,W4,NEWP7,NEWP8,Q5,W5,DW4), DW4==1, untilWin(J,NEWP8,NEWP9,X5,Q5,W5,NEWP10,NEWP11,Q6,W6,DW5), DW5==1, append(X1,X2,SOM), append(SOM,X3,SOM1), append(SOM1,X4,SOM2), append(SOM2,X5,SOM3), append(SOM3,[[3,2]],X),!.

bourree(J,V,Y,X) :- untilWin(J,V,Y,X1,0,1,NEWP1,NEWP2,Q2,W2,DW1), DW1==1, untilWin(J,NEWP1,NEWP2,X2,Q2,W2,NEWP3,NEWP4,Q3,W3,DW2), DW2==0, untilWin(J,NEWP3,NEWP4,X3,Q3,W3,NEWP5,NEWP6,Q4,W4,DW3), DW3==0, untilWin(J,NEWP5,NEWP6,X4,Q4,W4,NEWP7,NEWP8,Q5,W5,DW4), DW4==1, untilWin(J,NEWP8,NEWP9,X5,Q5,W5,NEWP10,NEWP11,Q6,W6,DW5), DW5==1, append(X1,X2,SOM), append(SOM,X3,SOM1), append(SOM1,X4,SOM2), append(SOM2,X5,SOM3), append(SOM3,[[3,2]],X),!.

bourree(J,V,Y,X) :- untilWin(J,V,Y,X1,0,1,NEWP1,NEWP2,Q2,W2,DW1), DW1==1, untilWin(J,NEWP1,NEWP2,X2,Q2,W2,NEWP3,NEWP4,Q3,W3,DW2), DW2==0, untilWin(J,NEWP3,NEWP4,X3,Q3,W3,NEWP5,NEWP6,Q4,W4,DW3), DW3==1, untilWin(J,NEWP5,NEWP6,X4,Q4,W4,NEWP7,NEWP8,Q5,W5,DW4), DW4==0, untilWin(J,NEWP8,NEWP9,X5,Q5,W5,NEWP10,NEWP11,Q6,W6,DW5), DW5==1, append(X1,X2,SOM), append(SOM,X3,SOM1), append(SOM1,X4,SOM2), append(SOM2,X5,SOM3), append(SOM3,[[3,2]],X),!.

del(X,[X|Tail],Tail).    
del(X,[Y|Tail],[Y|Tail1]):- del(X,Tail,Tail1).


 /* untilWin(J,V,Y,X,Q,W,1)*/

untilWin(J,V,Y,X,Q,W,NEWP1,NEWP2,Q2,W2,DW) :- ifTwoTrump(J,A), get(A,E),TempWin is W, playGame(V,Y,W,Q,E,T,P,TF,WIN), Q2 is TF, W2 is WIN, appendGame(T,P,TempWin,X), del(T,V,NEWP1), del(P,Y,NEWP2), WIN==1, DW is 1.
untilWin(J,V,Y,X,Q,W,NEWP1,NEWP2,Q2,W2,DW) :- ifTwoTrump(J,A), get(A,E),TempWin is W, playGame(V,Y,W,Q,E,T,P,TF,WIN), Q2 is TF, W2 is WIN, appendGame(T,P,TempWin,X), del(T,V,NEWP1), del(P,Y,NEWP2), WIN==2, DW is 0.

addTo([],W2).
addTo(X,W,W2) :- getHead(X,T), get(T,Y), append([Y],[W],W2),getTail(X,R), write(W2), addTo(R,W2).
thisU(X) :- member(T,X), get(T,Y), append(Y,List,W), same(W).


same([_]).
same([X,X|T]) :- same([X|T]).

checkHand([],List,List).
checkHand([H|T],List,Ret) :- get(H,Suit), append([Suit],List,Deck), checkHand(T,Deck,Ret).

