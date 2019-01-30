FLAGS = -Wall -Werror -std=c99
% : %.c
	gcc -g -O $< -o $@.o $(FLAGS)
	gcc -g $< -o $@ $(FLAGS)
