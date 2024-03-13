COMPILE = gcc $(COMPILE_SWITCHES)
OFILES = minesweep.o board.o main.o
HFILES = minesweep.h board.h cell.h

game: $(OFILES)
	gcc -o game $(OFILES)

main.o: main.c $(HFILES)
	$(COMPILE) -c main.c

minesweep.o: minesweep.c $(HFILES)
	$(COMPILE) -c minesweep.c

board.o: board.c $(HFILES)
	$(COMPILE) -c board.c

clean:
	/bin/rm -f *.o
