# all:
# 	gcc -o printchars printchars.c
# 	gcc sched.c scheduler.c main.c -o main
#
# sched:
# 	gcc sched.c -o sched
#
# printchars:
# 	gcc -o printchars printchars.c

all : sched.o scheduler.o main.o printchars
	clang -Wall -Wextra main.o sched.o scheduler.o -o run

scheduler.o : scheduler.c scheduler.h
	clang -Wall -Wextra -c scheduler.c

sched.o : sched.c sched.h
	clang -Wall -Wextra -c sched.c

main.o : main.c sched.h scheduler.h
	clang -Wall -Wextra -c main.c

printchars:
	clang -Wall -Wextra printchars.c -o printchars

clean:
	rm -f printchars
	rm -f sched
	rm -f run
