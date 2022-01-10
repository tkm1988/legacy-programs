%------------
% Question 1
%------------
head([H|_], X) :- X = H.

%------------
% Question 2
%------------
tail([_|T], X) :- X = T.

%------------
% Question 3
%------------
print_every_second([]).    %Base case
print_every_second([_|[]]).%Base case

print_every_second([_, Y|T]):- %Step case
    format('~w\n',Y),
    print_every_second(T).

%------------
% Question 4
%------------
print_numbers([]).    %Base case

print_numbers([H|T]):-%Step case
    \+number(H),
    print_numbers(T).

print_numbers([H|T]):-%Step case
    format('~w\n', H),
    print_numbers(T).

%--------------------
% Challenge Question
%--------------------
unifies([], _).    %Base case

unifies([H|T], X):-%Step case
    X \= H,
    unifies(T, X).

unifies([H|T], X):-%Step case
    format('~w\n', H),
    unifies(T, X).
