#include <stdio.h>

int main ()
{
	int v[6] = {10, 15, 15, 13, 16, 20};
	int temp = 0, i;

	for(i=0; i<=5; i++ ) {
		if (temp < v[i])
			temp = v[i];
	}
	printf("Valoarea maxima din vector %d\n", temp);
	return 0;
}

