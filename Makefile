all:
	gcc-4.9 -std=c11 src/vm.c src/stack_frame.c src/list.c src/types.c -o vm

debug:
	gcc-4.9 -DDEBUG -std=c11 src/vm.c src/stack_frame.c src/list.c src/types.c -o vm

leakcheck:
	valgrind --leak-check=yes ./vm

clean:
	rm -f vm
