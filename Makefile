r:
	clang -Wall -Wno-unknown-pragmas -Wextra -Wno-pragmas -O2 -ftree-vectorize conway.c -o conway && ./conway
