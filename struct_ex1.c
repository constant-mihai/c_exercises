#include <stdio.h>

struct school *returnStructPt(struct school);

struct students {
	char *name;
	int age;
};

struct school {
	int n_classes;
	struct students st[10];
};

int main () {
	int i = 0;
	struct school sc1;
	// Back to school
	while (i++ < 2) { // School not full
		printf("Enroll student \t %d\n", (i - 1));
		int j = 0;
		sc1.st[i-1].name = malloc(20);
		if (sc1.st[i-1].name == NULL) { printf("Malloc Err");return 0; }
		while((sc1.st[i-1].name[j++] = getchar()) != '\n')
		//	putchar(sc1.st[i-1].name[j - 1]);
			;
		sc1.st[i-1].name[j+1] = '\0';
		//sc1.st[i-1].name = buff;		
	}
	
	printf("School is full with %d students\n", --i);
	char c;
	//printf();
	while((c = getchar()) != 'p')
		;
	
	while (i-- > 0) {
	printf("Studentul %d : %s \n", i, sc1.st[i].name);
	}
	
	struct school *pointer_to_school;
	pointer_to_school = returnStructPt(sc1);
	printf("Printf %s", pointer_to_school->st[1].name);
	
	return 1;
}

struct school *returnStructPt(struct school sc) {
	struct school *ptr_sc = 0;
	ptr_sc = &sc;
	return ptr_sc;
}
