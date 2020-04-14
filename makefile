CC=gcc
FLAGS=-pthread

semaphore: sem.c
	$(CC) $(FLAGS) -o semaphore sem.c
