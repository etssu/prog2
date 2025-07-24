#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MMAX 1000
#define N 100

const char *readline() {
    // staticke pole zostava platne aj po skonceni funkcie
    static char buffer[MMAX + 1] = {0};
    // nacitanie riadku
    fgets(buffer, sizeof(buffer), stdin);
    // odstranenie znaku noveho riadku
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}

int define_mode(char op[], char mem[], char **sp, char **ep) {
    while (1) {
        const char *input = readline();

        if (strncmp(input, "quit", 4) == 0) {
            return 0;
        }
        else if (strncmp(input, "in ", 3) == 0) {
            if (strlen(input+3) > N) {
                printf("ERR_OVERFLOW");
                return 0;
            } else {
                strcpy(op, input + 3);
                *sp = op;
                *ep = op + strlen(op); //setting on the last symbol (\0)
            }
        }
        else if (strncmp(input, "out op", 6) == 0) {
            printf(op);
            putchar('\n');
        }
        else if (strncmp(input, "out range", 9) == 0) {
            if (*sp > *ep) {
                printf("ERR_POSITION\n");
                return 0;
            }
            else if (*sp == *ep) {
                putchar('\n');
            }
            char *start = *sp;      //create a pointer for start
            while (start < *ep) {   //while start is
                putchar(*start);        //print character
                start++;            //move pointer
            }
            putchar('\n');
        }
        else if (strncmp(input, "out mem", 7) == 0) {
            if (mem[0] == '\0') {       //if mem is empty
                putchar('\n');          //empty line
            } else {
                printf(mem);
                putchar('\n');
            }
        }
        else if (strncmp(input, "move ", 5) == 0) {
            const char *num = input + 8;
            int n = atoi(num); //convert string to int
            if (strncmp(input, "move sp", 7) == 0) {  //start point
                *sp += n;
            }
            else{       //end point
                *ep += n;
            }

            if (*ep < op || *sp < op || *sp > op + strlen(op) || *ep > op + strlen(op)) { //searching for errors
                printf("ERR_OUT_OF_RANGE");
                return 0;
            }
        }
        else if (strncmp(input, "start ", 6) == 0) {
            if (strncmp(input, "start sp", 8) == 0) {
                *sp = op;   //sp on op[0]
            }
            else if (strncmp(input, "start ep", 8) == 0) {
               *ep = op;        //ep on op[0]
            }
        }
        else if (strncmp(input, "end ", 4) == 0) {
            int len = strlen(op);
            if (strncmp(input, "end sp", 6) == 0) {
                *sp = op + len;     //setting sp on \0
            }
            else if (strncmp(input, "end ep", 6) == 0) {
                *ep = op + len;     //setting ep on \0
            }
        }
        else if (strncmp(input, "first", 5) == 0) {
            char *start = op;   //beginning of the word
            while (*start && !isalnum(*start)) {    //skip all non-alphanumeric
                start++;
            }
            char *end = start;  //start to count from start point
            while (*end && isalnum(*end)) {  //while end isnt \0 and is alphanumeric
                end++;
            }
            *sp = start;
            *ep = end;
        }

        else if (strncmp(input, "last", 4) == 0) {
            char *end = op + strlen(op) - 1;
            while (end >= op && !isalnum(*end)) { //while *end is within OP and isnt alphanumeric
                end--;
            }
            if (end < op) {     //if we didnt find any word, put the pointers at \0
                *ep = op + strlen(op);
                *sp = *ep;
            }
            else {char *start = end;
                while (*start && isalnum(*(start-1))) { //start-1 will point to the previous character
                    //check if the character before the current one is alphanumeric, if yes - continue moving backwards along the line.
                    start--;
                }
                *sp = start;
                *ep = end+1; //sets the ep pointer immediately after the current word
            }
        }
        else if (strncmp(input, "prev", 4) == 0) {
            if (*sp == op) {    //if we are in the beginning
                *sp = op;       //set immediately on the start
                *ep = op;
            }
            else if (isalnum(**sp)) {     //if we are in a word
                while (*sp > op && isalnum(**sp)) {      //skipping the word
                    (*sp)--;
                }
            }

            while (*sp > op && !isalnum(**sp)) {        //skip all non-alphanumeric
                (*sp)--;
            }
            if (*sp == op) {    //if we are in the beginning
                *sp = op;
                *ep = op;
            }
            else {
                *ep = *sp+1; //so it would point on the next symbol
                while (*sp > op && isalnum(*(*sp-1))) { //checking the prev symbol
                    (*sp)--;
                }
            }
        }
        else if (strncmp(input, "next", 4) == 0) {
            if (*sp == op + strlen(op)) {
                *sp = op + strlen(op);
                *ep = *sp;
            }
            else if (isalnum(**sp)) {
                while (*sp < op + strlen(op) && isalnum(**sp)) {     //if we are in a word - skip it
                    (*sp)++;
                }
            }
            while (*sp < op + strlen(op) && !isalnum(**sp)) {   //skip all non-alphanumeric
                    (*sp)++;
            }
            *ep = *sp;
            while (*ep < op + strlen(op) && isalnum(**ep)) { //if a symbol is alphanumeric and is within OP
                (*ep)++;
            }
        }
        //OPERACIE S RETAZCAMI
        else if (strncmp(input, "del", 3) == 0) {
            if (*sp > *ep) {
                printf("ERR_POSITION\n");
                return 0;
            }
            memmove(*sp, *ep, strlen(*ep) + 1); // deleting text between sp and ep
                                                    //strlen(*ep) - text after EP, +1 - '\0'
            *ep = *sp; //put them on the same place
        }
        else if (strncmp(input, "crop", 4)==0) {
            if (*sp > *ep) {
                printf("ERR_POSITION\n");
                return 0;
            }
            if (*sp == *ep) {
                op[0] = '\0'; //the op is empty
            }
            else {
                memmove(op, *sp, *ep - *sp);
                op[*ep - *sp] = '\0'; //the last symbol
                *sp = op;
                *ep = op + strlen(op); //setting on the last symbol
            }
        }
        else if (strncmp(input, "copy", 4) == 0) {
            if (*sp > *ep) {
                printf("ERR_POSITION");
                putchar('\n');
                return 0;
            }
            else {
                memmove(mem, *sp, *ep - *sp); //deep copy to mem, starting from *sp, len is (*ep - *sp)
                mem[*ep - *sp] = '\0';
            }
        }
        else if (strncmp(input, "insert ", 7) == 0) {
            if (strncmp(input, "insert mem", 10) == 0) {
                int mem_len = strlen(mem);
                int len = strlen(op);
                if (len + mem_len > N) {
                    printf("ERR_OVERFLOW\n");
                    return 0;
                }
                else {
                    memmove(*sp + mem_len, *sp, len - (*sp - op) + 1); //moving text in op, to insert mem there
                    for (int i = 0; i < mem_len; i++) {
                        (*sp)[i] = mem[i];        //inserting mem
                    }
                    *ep += mem_len;
                }
            }
            else if (strncmp(input, "insert ", 7) == 0) {
                const char *num = input + 7;
                int n = atoi(num); //convert string to int
                int len = strlen(op);
                if (len + n > N) {
                    printf("ERR_OVERFLOW\n");
                    return 0;
                }
                else {
                    memmove(*sp+n, *sp , strlen(op) - (*sp-op) +1); //move to *sp+n, from *sp
                    //(strlen(op) - (*sp-op) +1) - remainder of the row
                    memset(*sp, ' ', n); //inserting gaps
                    *ep += n;
                }
            }
        }
        else if (strncmp(input, "replace", 7) == 0) {
            if (*sp > *ep) {
                printf("ERR_POSITION\n");
                return 0;
            }
            else if (*sp == *ep || mem[0]=='\0') {} //nothing happens
            else {
                int mem_len = strlen(mem);
                int replace_len = (*ep - *sp);
                int tail_len = strlen(*ep);

                if (mem_len < replace_len){ //if len of mem is smaller than replace len
                    int leftover_len = replace_len - mem_len;  //leftovers

                    memmove(*sp + mem_len + leftover_len, *ep, tail_len+1); //to *sp + mem_len from *ep
                    memcpy(*sp, mem, mem_len); //copy from mem to op (starting from sp)
                }
                else if (mem_len > replace_len){
                    memmove(*sp + replace_len, *ep, tail_len+1); //
                    memcpy(*sp, mem, replace_len); //copy from mem to op (starting from sp),
                }
                else {
                    memcpy(*sp, mem, replace_len); //just copy from mem to op (starting form sp)
                }
            }
        }
    }
}

int main(void) {
    char op[N + 1];
    op[0] = '\0'; //1st value is \0

    char mem[N + 1];
    mem[0] = '\0';

    char *sp = op; //start pointer
    char *ep = op; //end pointer

    define_mode(op, mem, &sp, &ep);

    return 0;
}