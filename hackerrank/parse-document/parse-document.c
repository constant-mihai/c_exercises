#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LOG_ENABLE 1

#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) do { if (LOG_ENABLE == 1) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__); } while (0)

char* kth_word_in_mth_sentence_of_nth_paragraph(char**** document, int k, int m, int n) {
    return document[n-1][m-1][k-1];
}

char** kth_sentence_in_mth_paragraph(char**** document, int k, int m) {
    return document[m-1][k-1];
}

char*** kth_paragraph(char**** document, int k) {
    return document[k-1];
}

int parse_word(int *beg, int pos, const char *text, int *num, char **sentence) {
    char* word = 0;
    int end = pos - 1, size = (end - *beg) + 1;

    word=(char*)malloc(size + 1 /* '\0' */);
    // From "man strncpy"
    // Warning: If there is no null byte among the first n bytes of src, the string placed in
    // dest will not be null-terminated.
    // strncpy(word, text+beg, (size_t)size);
    memcpy(word, text+*beg, (size_t)size);
    word[size] = '\0';
    int index = *num;
    sentence[index] = word;
    (*num)++;
    *beg = pos+1; // jump over space
    pos++;

    return pos;
}

int parse_sentence(int pos, const char *text, char **sentence) {
    int num = 0;
    int beg = pos;
    while (pos < (int)strlen(text)) {
        switch(text[pos]) {
            case ' ':
                pos = parse_word(&beg, pos, text, &num, sentence);
                break;
            case '.':
                pos = parse_word(&beg, pos, text, &num, sentence);
                goto done;
                break;
            default:
                pos++;
                break;
        }
    }
done:
    return pos;
}

char**** get_document(char* text) {
    char**** doc = (char****)malloc(1000*sizeof(char****));
    char*** paragraph = 0;
    char** sentence = 0;

    paragraph = (char***)malloc(1000*sizeof(char***));
    int num_sentences = 0;
    int num_paragraphs = 0;

    for (size_t pos=0; pos<strlen(text);) {
        switch(text[pos]) {
            case '\n':
                doc[num_paragraphs++] = paragraph;
                num_sentences = 0;
                paragraph = (char***)malloc(1000*sizeof(char***));
                pos++;
                break;
            default:
                sentence = (char**)malloc(1000*(sizeof(char**)));
                pos = parse_sentence(pos, text, sentence);
                paragraph[num_sentences++] = sentence;
                break;
        }
    }
    doc[num_paragraphs++] = paragraph;

    return doc;
}

char* get_input_text() {	
    int paragraph_count;
    scanf("%d", &paragraph_count);

    char p[MAX_PARAGRAPHS][MAX_CHARACTERS], doc[MAX_CHARACTERS];
    memset(doc, 0, sizeof(doc));
    getchar();
    for (int i = 0; i < paragraph_count; i++) {
        scanf("%[^\n]%*c", p[i]);
        strcat(doc, p[i]);
        if (i != paragraph_count - 1)
            strcat(doc, "\n");
    }

    char* returnDoc = (char*)malloc((strlen (doc)+1) * (sizeof(char)));
    strcpy(returnDoc, doc);
    return returnDoc;
}

void print_word(char* word) {
    printf("%s", word);
}

void print_sentence(char** sentence) {
    int word_count;
    scanf("%d", &word_count);
    for(int i = 0; i < word_count; i++){
        printf("%s", sentence[i]);
        if( i != word_count - 1)
            printf(" ");
    }
} 

void print_paragraph(char*** paragraph) {
    int sentence_count;
    scanf("%d", &sentence_count);
    for (int i = 0; i < sentence_count; i++) {
        print_sentence(*(paragraph + i));
        printf(".");
    }
}

/**
 * Sample input:
 * 2
 * Learning C is fun.
 * Learning pointers is more fun.It is good to have pointers.
 * 3
 * 1 2
 * 2
 * 5
 * 6
 * 2 1 1
 * 4
 * 3 1 1 1
 *
 * Sample output:
 * Learning pointers is more fun.It is good to have pointers.
 * Learning C is fun
 * Learning
 */
int main() 
{
    char* text = get_input_text();
    char**** document = get_document(text);

    int q;
    scanf("%d", &q);

    while (q--) {
        int type;
        scanf("%d", &type);

        if (type == 3){
            int k, m, n;
            scanf("%d %d %d", &k, &m, &n);
            char* word = kth_word_in_mth_sentence_of_nth_paragraph(document, k, m, n);
            print_word(word);
        }

        else if (type == 2){
            int k, m;
            scanf("%d %d", &k, &m);
            char** sentence = kth_sentence_in_mth_paragraph(document, k, m);
            print_sentence(sentence);
        }

        else{
            int k;
            scanf("%d", &k);
            char*** paragraph = kth_paragraph(document, k);
            print_paragraph(paragraph);
        }
        printf("\n");
    }     
}
