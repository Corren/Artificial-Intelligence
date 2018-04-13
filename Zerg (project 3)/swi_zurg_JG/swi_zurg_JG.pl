% Jonathan Gregory
% CS 461 Project 3
% Fall 2017
% Prof. Brian Hare

% This program can be executed by typing "fastest(Route)." into the console
% 
% Referenced "https://youtu.be/SykxWpFwMGs" for inspiration
% Referenced "http://telegraphics.com.au/svn/puzzles/trunk/prolog/zurg.pl" for inspiration
% Referenced "https://web.engr.oregonstate.edu/~erwig/papers/Zurg_JFP04.pdf" for inspiration
% Referenced "https://www.metalevel.at/zurg/" for inspiration

% all of the relationships. 
% ie. buzz speed = 5
speed(buzz,5).
speed(woody,10).
speed(rex,20).
speed(hamm,25).

% cut/1 effectively tells Prolog to freeze all the decisions made so far in this predicate
% [zurg_will_get_you],
times([],0) :-  !.

% times() is true if --> speed=true AND times=true AND Time is max.
times([Toy1|LeftToys],Time) :- 
     speed(Toy1,S), 
     times(LeftToys,D), 
	 
	 % Time is assigned the max value of (S or D)
     Time is max(S,D).

runAcross(LeftToys,[Toy1,Toy2],Route) :- 
	% true if Toy2 is in list LeftToys
     member(Toy2,LeftToys),
	 % true if Toy1 is in list LeftToys
     member(Toy1,LeftToys),
	 % true if Toy1 < Toy2 - gets rid of duplicates
	 Toy1 @< Toy2,	 
	 % Route is the result of removing [Toy1,Toy2] from list LeftToys
     subtract(LeftToys,[Toy1,Toy2],Route).

move(print('',LeftToys1),print('--',LeftToys2),'Cross to the right side'(Route),D) :- 
     runAcross(LeftToys1,Route,LeftToys2),
     times(Route,D).

move(print('--',LeftToys1),print('',LeftToys2),'Brings flashlight back to the left side'(Toy1),D) :-
	 % R is the result of removing LeftToys1 from list T
     subtract([buzz,hamm,rex,woody],LeftToys1,R),
	 % true if Toy1 is in list R
     member(Toy1,R),
	 % merges the ordered sets [Toy1] and LeftToys1 into set LeftToys2 without any duplicates
     merge_set([Toy1],LeftToys1,LeftToys2),
     speed(Toy1,D).

continue(print('--',[]),[],[],0).   
 
continue(LeftToys1,LeftToys2,LeftToys,D) :-
     move(LeftToys1,T,Route,Time1),
     continue(T,LeftToys2,N,Time2),
	 % Route is the head of the list, N and LeftToys are the tail of the list
     append([Route],N,LeftToys),
	 % assigns the varriable D to the value of Toy1 + Toy2
     D is Time1 + Time2.

start(Route,D) :-
     %toys(T),
     continue(print('',[buzz,hamm,rex,woody]),[],Route,TestTime),
     TestTime=<D.

fastest(Route) :- start(Route,60).
% if start(Route,60) is true, then it is fastest(Route).