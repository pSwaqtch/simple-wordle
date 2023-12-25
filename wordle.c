#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <ctype.h>

int try = 12;

#define WORD_LENGTH 5

void board(char words[try*2][6], int move, int o);
int checkPlay(char key[6], char word[6], int move, char words[try*2][6]);
int checkWord(char word[6]);
int checkCharacter(char key[6],char keyi, char x);
void getRandomWord(char *key);
void clearBuffer();
void clearScreen();

int main() {
    int o=0,x=0,y=0,z=1;

    do {
        printf("Enter the no. of tries (min=6, max=15) : ");
        scanf("%d", &try);
        if (try < 6 || try > 15) {
            printf("Invalid input. Please enter a number between 6 and 15.\n");
        }
    } while (try < 6 || try > 15);
    char words[try*2][6],key[6]; 
    getRandomWord(key);

    for (int i = 0; i < try*2; ++i) {
        for (int j = 0; j < 6 && (i%2)==0; ++j) {
            words[i][j]='_';
        }
        for (int k = 0; k < 6 && (i%2)==1; ++k) {
            words[i][k]='.';
        }        
    }

    int move=0;

while(!o && move<=try){

    for (int i = 0; i < try*2; i+=2) {
        move++;
        board(words, move, o); 
        //puts(key);
        printf("Enter your guess: \n");
        for (int j = 0; j < WORD_LENGTH ; ++j) {

            char ch='x';

            while ((ch = getchar()) != '\n' && !isalpha(ch)) {
                // Ignore non-alphabetic characters
            }

            if (isalpha(ch)) {
                if (ch>=65 && ch<=90){}else {ch-=32;};
                words[i][j] = ch;
                if (j==4){clearBuffer();}
            } else {
                //printf("not an alphabet\n");
                j--; 
            }
        }

        words[i][WORD_LENGTH]='\0';
        z = checkWord(words[i]);

    for (int i = 0;z==0 && i < try*2; i+=2) {
        for (int j = 0; j < WORD_LENGTH; ++j) {
            x=checkCharacter(key,key[j],words[i][j]);
            //printf("%d\n",x);
            if (x==1){
                words[i+1][j]='=';
            } else if(x==2){
                words[i+1][j]='-';
            }
        }
    }
        if(z==0){ //word exits
            o=checkPlay(key,words[i],move,words);
            if (o==1){break;}
        } else if (z==1){ //word does not exist
            printf("word not found in data base !\ntry again >>>\n");
            for (int f = 0; f < 6 ; ++f) {
                words[i][f] = '_';
            }
            i=i-2;
            move--;
        }
    }
}
    board(words, move, o);
    return 0;
}

void getRandomWord(char *key) {
    
    FILE* file = fopen("5_letter_words.txt", "r");

    rewind(file);

    srand(time(NULL));      // Seed the random number generator

    int randomIndex = rand() % 10422;       // Generate a random number between 0 and wordCount - 1

    for (int i = 0; i <= randomIndex; i++) {    // Get the word at the random index
        fgets(key, sizeof(key), file);
    }

    key[strcspn(key, "\n")] = '\0';     // Remove the newline character at the end

    fclose(file);
}


void board(char words[try*2][6], int move, int o) {
    //clearScreen();

    int m=29;
    int n=m/3;

    printf(" ++");for (int i = 0; i < m; ++i)printf("=");printf("++\n");
    printf(" || ");for (int i = 0; i < n; ++i)printf(" ");printf(" Wordle ");for (int i = 0; i < n; ++i)printf(" ");printf("%d",move);;printf(" ||\n");
    printf(" ++");for (int i = 0; i < m; ++i)printf("=");printf("++\n");
    
    for (int i = 0; i < 1-o+move*2; ++i) {
        if ((i%2)==0 && i<try*2){printf(" ||");for (int i = 0; i < m; ++i)printf(" ");printf("||\n");}
        printf(" ||  ");
        for (int j = 0; j < WORD_LENGTH; ++j) {
            printf("  %c  ",words[i][j]);
        }
        printf("  ||\n");
        if (((i%2)==1 && i<((try*2)-1))||(try*2==(i+1))){printf(" ||");for (int k = 0; k < m; ++k)printf("-");printf("||\n");}
    }
    if (o==1){printf(" ||  ---C--O--R--R--E--C--T---  ||\n");}
    printf(" ++");for (int i = 0; i < m; ++i)printf("=");printf("++\n");
}

int checkWord(char word[6]) {

    FILE* file = fopen("5_letter_words.txt", "r");

    char skey[6];
    
    while (fscanf(file, "%5s", skey) == 1) {
        // Check if the current word matches the target word
        if (strcmp(skey, word) == 0) {
            // Close the file before returning
            fclose(file);
            return 0; // Return 0 if the word is found
        }
    }

    // Close the file
    fclose(file);

    // Return 1 if the word is not found
    return 1;
}

int checkCharacter(char key[6], char keyi, char x){
    for (int i = 0; i < 6; ++i) {
        if(x==key[i]){
            if (x==keyi) {
                return 1;
            }
            else {
                return 2;
            }
        }
    }
    return 0;
}

int checkPlay(char key[6], char word[6], int move, char words[try*2][6]) {

    int result = strncmp(key, word, 6), o=0;

    // Check the result
    if (result == 0) {
        board(words,move,o);
        printf("You Won by %d moves!\n", try-move);
        return 1;
    } else {
        printf("Try Again\n");
        return 0;
    }
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Use "cls" on Windows
    #else
        system("clear"); // Use "clear" on Unix-like systems
    #endif
}