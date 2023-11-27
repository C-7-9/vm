check:
	cc -Wall *.c -lm
debug:
	cc -o *.c -lm -g
clean:
	rm -f *.gch a.out
