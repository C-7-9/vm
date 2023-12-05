check:
	cc -std=c99 -Wall *.h *.c -lm
debug:
	cc -std=c99 -o test *.c -lm -g
clean:
	rm -f *.gch a.out
