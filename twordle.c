//Author:Yahir Miranda
//Date:5/1/2026
//Project 10

#include <stdio.h>
#include <stdbool.h>

#define WORD_LEN     5
#define MAX_GUESSES  6
#define BUF_LEN      100

bool loadWord(char word[]) {
    FILE *fp = fopen("word.txt", "r");
    if (fp == NULL) {
        return false;
    }

    for (int i = 0; i < WORD_LEN; i++) {
        if (fscanf(fp, "%c", &word[i]) != 1) {
            fclose(fp);
            return false;
        }
    }
    word[WORD_LEN] = '\0';
    fclose(fp);
    return true;
}

char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

char toUpperChar(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void readLine(char buffer[], int *length) {
    int count = 0;
    char c;
    while (scanf("%c", &c) == 1 && c != '\n') {
        if (count < BUF_LEN - 1) {
            buffer[count] = c;
            count++;
        }
    }
    buffer[count] = '\0';
    *length = count;
}

bool isValidGuess(char buffer[], int length) {
    if (length != WORD_LEN) {
        return false;
    }
    for (int i = 0; i < WORD_LEN; i++) {
        if (!isLetter(buffer[i])) {
            return false;
        }
    }
    return true;
}

void getGuess(char guess[], int guessNum) {
    char buffer[BUF_LEN];
    int length;

    if (guessNum == MAX_GUESSES) {
        printf("FINAL GUESS : ");
    } else {
        printf("GUESS %d! Enter your guess: ", guessNum);
    }

    readLine(buffer, &length);

    while (!isValidGuess(buffer, length)) {
        printf("Your guess must be 5 letters long.\n");
        printf("Please try again: ");
        readLine(buffer, &length);
    }

    for (int i = 0; i < WORD_LEN; i++) {
        guess[i] = toLowerChar(buffer[i]);
    }
    guess[WORD_LEN] = '\0';
}

bool isCorrect(char guess[], char word[]) {
    for (int i = 0; i < WORD_LEN; i++) {
        if (guess[i] != word[i]) {
            return false;
        }
    }
    return true;
}

void displayGuess(char guess[], char word[]) {
    char display[WORD_LEN + 1];
    char hints[WORD_LEN + 1];

    for (int i = 0; i < WORD_LEN; i++) {
        if (guess[i] == word[i]) {
            display[i] = toUpperChar(guess[i]);
        } else {
            display[i] = guess[i];
        }
    }
    display[WORD_LEN] = '\0';

    for (int i = 0; i < WORD_LEN; i++) {
        hints[i] = ' ';

        if (guess[i] == word[i]) {
            continue;
        }

        for (int j = 0; j < WORD_LEN; j++) {
            if (word[j] == guess[i]) {
                if (guess[j] != word[j]) {
                    hints[i] = '^';
                }
                break;
            }
        }
    }
    hints[WORD_LEN] = '\0';

    printf("%s\n", display);
    printf("%s\n", hints);
}

void displayAllGuesses(char guesses[][WORD_LEN + 1], int numGuesses, char word[]) {
    printf("================================\n");
    for (int i = 0; i < numGuesses; i++) {
        displayGuess(guesses[i], word);
    }
}

void displayWinScreen(char word[], int numGuesses) {
    printf("================================\n");
    printf("        ");
    for (int i = 0; i < WORD_LEN; i++) {
        printf("%c", toUpperChar(word[i]));
    }
    printf("\n");
    printf("    You won in %d %s!\n",
           numGuesses, (numGuesses == 1) ? "guess" : "guesses");
    printf("        GOATED!\n");
}

int main(void) {
    char word[WORD_LEN + 1];
    char guesses[MAX_GUESSES][WORD_LEN + 1];
    int numGuesses = 0;
    bool won = false;

    if (!loadWord(word)) {
        printf("Error: could not open word.txt.\n");
        return 1;
    }

    while (numGuesses < MAX_GUESSES && !won) {
        getGuess(guesses[numGuesses], numGuesses + 1);
        numGuesses++;

        if (isCorrect(guesses[numGuesses - 1], word)) {
            won = true;
            displayWinScreen(word, numGuesses);
        } else {
            displayAllGuesses(guesses, numGuesses, word);
        }
    }

    if (!won) {
        printf("You lost, better luck next time!\n");
    }

    return 0;
}
