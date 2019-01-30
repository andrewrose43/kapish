FLAGS = -Wall -Werror -std=c99
% : %.c
	gcc -O $< -o $@.o $(FLAGS)
	gcc $< -o $@ $(FLAGS)
