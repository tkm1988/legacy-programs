word(article, a).
word(article, the).
word(noun, lecturer).
word(noun, squid).
word(verb, likes).
word(verb, eats).

sentence(Word1, Word2, Word3, Word4, Word5) :-
  word(article, Word1),
  word(noun, Word2),
  word(verb, Word3),
  word(article, Word4),
  word(noun, Word5).
