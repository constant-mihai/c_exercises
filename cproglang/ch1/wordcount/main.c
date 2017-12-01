/***************************************** 

* File Name : main.c

* Creation Date : 20-07-2015

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

******************************************/


#include <stdio.h>

#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */
/* count lines, words, and characters in input */
main()
{
    int c, nl, nw, nc, state;
    char s[2000];

    int comma = (1,2);
    printf("comma=%d", comma);

    const char *pstart = "ProgStart";

    printf("%.*s\n", 4, pstart);

    state = OUT;
    nl = nw = nc = 0;
    while ((c = getchar()) != EOF) {
        s[nc] = c;
        ++nc;
        if (c == '\n')
            ++nl;
        if (c == ' ' || c == '\n' || c == '\t') {
            state = OUT;
        }
        else if (state == OUT) {
            state = IN;
            ++nw;
        }
    }
    printf(" New lines = %d\n Words =  %d\n Characters = %d\n", nl, nw, nc);

    int i = 0;
    state = OUT;
    while (i<=nc) {
        if (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
            if (state != OUT) printf("\n");
            state = OUT;
        } else {
            state = IN;
            printf("%c",s[i]);
        }

        i++;
    }
}

