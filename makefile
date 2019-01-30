FLAGS = -Wall -Werror -std=c99 -g
% : %.c
	gcc -O $< -o $@.o $(FLAGS)
	gcc $< -o $@ $(FLAGS)
