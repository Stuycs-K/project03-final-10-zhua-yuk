compile program: subprocess.o memory.o fdmcalc.o config.h
	@gcc -o program main.o subprocess.o memory.o fdmcalc.o
run: program
	@./program
main.o: main.c config.h
	@gcc -c main.c
subprocess.o: subprocess.c subprocess.h memory.h fdmcalc.h config.h
	@gcc -c subprocess.c
memory.o: memory.c memory.h config.h
	@gcc -c memory.c
fdmcalc.o: fdmcalc.c fdmcalc.h
	@gcc -c fdmcalc.c
clean: 
	@rm -f *.o