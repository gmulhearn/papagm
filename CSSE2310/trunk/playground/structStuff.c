#include <stdio.h>
#include <stdlib.h>

struct test {
	int id;
};


void structFunc(struct test test) {
	test.id = 69;
}

void structPointerFunc(struct test* test) {
	test->id = 69;

}


int main(int argc, char** argv) {
	struct test test;
	test.id = 1;
	printf("%d \n", test.id);
	structFunc(test);
	printf("%d \n", test.id);
	structPointerFunc(&test);
	printf("%d \n", test.id);
	return 0;
}
