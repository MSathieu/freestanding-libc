format:
	clang-format -i $(filter-out printf/printf.c, $(wildcard */*.c)) $(wildcard include/*.h)
