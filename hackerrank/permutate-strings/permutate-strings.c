#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LOG_ENABLE 0

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) do { if (LOG_ENABLE == 1) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__); } while (0)

int find_largest_k(int n, char ** s) {
    int k = -1;
    for (int i=0; i<n-1; i++) {
        if (strcmp(s[i], s[i+1]) < 0) {
            k = i;
        }
    }
    
    return k;
}

int find_largest_l(int k, int n, char ** s) {
    int l = -1;
    for (int i=k; i<n; i++) {
        if (strcmp(s[k], s[i]) < 0) {
            l = i;
        }
    }
    return l; 
}

void reverse_sequence(int beg, int end, char ** s) {
    if (beg >= end) return;
    int mid = (end-beg)/2 + beg;
    LOG("beg=%d end=%d mid=%d", beg, end, mid);
    for (int i=beg, j=end; i<=mid; i++, j--) {
        char *tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

int next_permutation(int n, char **s)
{
    int k = find_largest_k(n, s);
    if (k == -1) {
        return 0;
    }
    int l = find_largest_l(k, n, s);

    char *tmp = s[k];
    s[k] = s[l];
    s[l] = tmp;

    reverse_sequence(k+1, n-1, s);

    return 1;
}

/**
 * This will generate all the permutation of the given strings, based on lexicographic ordering.
 * This is based on the following algorithm:
 * https://en.wikipedia.org/wiki/Permutation#Generation_in_lexicographic_order
 *
 * Example input:
 * 3
 * ab
 * cd 
 * ef 
 *
 * Example output:
 * ab cd ef
 * ab ef cd
 * cd ab ef
 * cd ef ab
 * ef ab cd
 * ef cd ab
 */
int main()
{
	char **s;
	int n;
	scanf("%d", &n);
	s = calloc(n, sizeof(char*));
	for (int i = 0; i < n; i++)
	{
		s[i] = calloc(11, sizeof(char));
		scanf("%s", s[i]);
	}
	do
	{
		for (int i = 0; i < n; i++)
			printf("%s%c", s[i], i == n - 1 ? '\n' : ' ');
	} while (next_permutation(n, s));
	for (int i = 0; i < n; i++)
		free(s[i]);
	free(s);
	return 0;
}
