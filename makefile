FLAGS = -Wall -Werror -std=c99
% : %.c
	gcc -O $< -o $@ $(FLAGS)
	gcc $< -o $@ $(FLAGS)
