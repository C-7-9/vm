check:
	cc -Wall *.h *.c -lm
debug:
	cc -o test *.c -lm -g
clean:
	rm -f *.gch a.out
