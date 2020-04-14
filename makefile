CC=gcc
FLAGS=-pthread

semaphore: sem.c
	$(CC) $(FLAGS) -o semaphore sem.c

monitor: mon.c
	$(CC) $(FLAGS) -o monitor mon.c
