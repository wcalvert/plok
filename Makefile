all:
	gcc-4.9 -std=c11 vm.c stack_frame.c stack.c list.c types.c -o vm

debug:
	gcc-4.9 -DDEBUG -std=c11 vm.c stack_frame.c stack.c list.c types.c -o vm

leakcheck:
	valgrind --leak-check=yes ./vm

clean:
	rm -f vm
