//Author:Yahir Miranda
//Date:5/1/2026
//Project 10 Extra Credit

#include <stdio.h>
#include <stdbool.h>

#define WORD_LEN     5
#define MAX_GUESSES  6
#define BUF_LEN      100
#define MAX_WORDS    1000
#define MAX_SCORES   5
#define NAME_LEN     20

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
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

void copyString(char dest[], char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

unsigned int getRandomSeed(void) {
    unsigned int seed = 0;
    FILE *fp = fopen("/dev/urandom", "rb");
    if (fp != NULL) {
        unsigned char bytes[4];
        if (fread(bytes, 1, 4, fp) == 4) {
            seed = ((unsigned int)bytes[0]) |
                   (((unsigned int)bytes[1]) << 8) |
                   (((unsigned int)bytes[2]) << 16) |
                   (((unsigned int)bytes[3]) << 24);
        }
        fclose(fp);
    }
    if (seed == 0) {
        seed = 12345;
    }
    return seed;
}

int loadWordList(char words[][WORD_LEN + 1]) {
    FILE *fp = fopen("words.txt", "r");
    if (fp == NULL) {
        return 0;
    }

    int count = 0;
    char buffer[BUF_LEN];

    while (count < MAX_WORDS) {
        int len = 0;
        char c;
        bool gotChar = false;

        while (fscanf(fp, "%c", &c) == 1) {
            gotChar = true;
            if (c == '\n' || c == '\r') {
                break;
            }
            if (len < BUF_LEN - 1) {
                buffer[len] = c;
                len++;
            }
        }
        buffer[len] = '\0';

        if (len == WORD_LEN) {
            bool allLetters = true;
            for (int i = 0; i < WORD_LEN; i++) {
                if (!isLetter(buffer[i])) {
                    allLetters = false;
                }
            }
            if (allLetters) {
                for (int i = 0; i < WORD_LEN; i++) {
                    words[count][i] = toLowerChar(buffer[i]);
                }
                words[count][WORD_LEN] = '\0';
                count++;
            }
        }

        if (!gotChar) {
            break;
        }
    }

    fclose(fp);
    return count;
}

void pickRandomWord(char words[][WORD_LEN + 1], int wordCount, char chosen[]) {
    unsigned int seed = getRandomSeed();
    int index = (int)(seed % (unsigned int)wordCount);
    copyString(chosen, words[index]);
}

bool loadSingleWord(char word[]) {
    FILE *fp = fopen("word.txt", "r");
    if (fp == NULL) {
        return false;
    }
    for (int i = 0; i < WORD_LEN; i++) {
        if (fscanf(fp, "%c", &word[i]) != 1) {
            fclose(fp);
            return false;
        }
        word[i] = toLowerChar(word[i]);
    }
    word[WORD_LEN] = '\0';
    fclose(fp);
    return true;
}

bool getMysteryWord(char word[]) {
    char wordList[MAX_WORDS][WORD_LEN + 1];
    int wordCount = loadWordList(wordList);

    if (wordCount > 0) {
        pickRandomWord(wordList, wordCount, word);
        return true;
    }

    return loadSingleWord(word);
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
    bool wordUsed[WORD_LEN];

    for (int i = 0; i < WORD_LEN; i++) {
        wordUsed[i] = false;
        hints[i] = ' ';
    }

    for (int i = 0; i < WORD_LEN; i++) {
        if (guess[i] == word[i]) {
            display[i] = toUpperChar(guess[i]);
            wordUsed[i] = true;
        } else {
            display[i] = guess[i];
        }
    }
    display[WORD_LEN] = '\0';

    for (int i = 0; i < WORD_LEN; i++) {
        if (guess[i] == word[i]) {
            continue;
        }
        for (int j = 0; j < WORD_LEN; j++) {
            if (!wordUsed[j] && word[j] == guess[i]) {
                hints[i] = '^';
                wordUsed[j] = true;
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

void getPlayerName(char name[]) {
    printf("Enter your name (no spaces): ");
    int count = 0;
    char c;
    while (scanf("%c", &c) == 1 && c != '\n') {
        if (count < NAME_LEN - 1 && c != ' ') {
            name[count] = c;
            count++;
        }
    }
    if (count == 0) {
        name[0] = '?';
        count = 1;
    }
    name[count] = '\0';
}

int loadScores(char names[][NAME_LEN], int scores[]) {
    FILE *fp = fopen("scores.txt", "r");
    if (fp == NULL) {
        return 0;
    }

    int count = 0;
    while (count < MAX_SCORES) {
        char nameBuf[NAME_LEN];
        int score;
        int nameLen = 0;
        char c;
        bool gotChar = false;

        while (fscanf(fp, "%c", &c) == 1) {
            gotChar = true;
            if (c == ' ' || c == '\t') {
                break;
            }
            if (c == '\n') {
                nameLen = 0;
                gotChar = false;
                break;
            }
            if (nameLen < NAME_LEN - 1) {
                nameBuf[nameLen] = c;
                nameLen++;
            }
        }
        nameBuf[nameLen] = '\0';

        if (!gotChar || nameLen == 0) {
            break;
        }

        if (fscanf(fp, "%d", &score) != 1) {
            break;
        }
        fscanf(fp, "%c", &c);

        copyString(names[count], nameBuf);
        scores[count] = score;
        count++;
    }

    fclose(fp);
    return count;
}

void saveScores(char names[][NAME_LEN], int scores[], int count) {
    FILE *fp = fopen("scores.txt", "w");
    if (fp == NULL) {
        printf("Warning: could not save scores.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %d\n", names[i], scores[i]);
    }
    fclose(fp);
}

void insertScore(char names[][NAME_LEN], int scores[], int *count,
                 char newName[], int newScore) {
    int pos = *count;
    for (int i = 0; i < *count; i++) {
        if (newScore < scores[i]) {
            pos = i;
            break;
        }
    }

    if (pos >= MAX_SCORES) {
        return;
    }

    int end = *count;
    if (end >= MAX_SCORES) {
        end = MAX_SCORES - 1;
    }

    for (int i = end; i > pos; i--) {
        copyString(names[i], names[i - 1]);
        scores[i] = scores[i - 1];
    }

    copyString(names[pos], newName);
    scores[pos] = newScore;

    if (*count < MAX_SCORES) {
        (*count)++;
    }
}

void displayScoreboard(char names[][NAME_LEN], int scores[], int count) {
    printf("================================\n");
    printf("       TOP %d SCOREBOARD\n", MAX_SCORES);
    printf("================================\n");
    if (count == 0) {
        printf("    (no scores yet - be the first!)\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("    %d. %-15s %d %s\n", i + 1, names[i], scores[i],
                   (scores[i] == 1) ? "guess" : "guesses");
        }
    }
    printf("================================\n");
}

int main(void) {
    char word[WORD_LEN + 1];
    char guesses[MAX_GUESSES][WORD_LEN + 1];
    char names[MAX_SCORES][NAME_LEN];
    int scores[MAX_SCORES];
    int numGuesses = 0;
    bool won = false;

    int scoreCount = loadScores(names, scores);
    displayScoreboard(names, scores, scoreCount);

    if (!getMysteryWord(word)) {
        printf("Error: could not load a mystery word from words.txt or word.txt.\n");
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

    if (won) {
        char playerName[NAME_LEN];
        getPlayerName(playerName);
        insertScore(names, scores, &scoreCount, playerName, numGuesses);
        saveScores(names, scores, scoreCount);
        displayScoreboard(names, scores, scoreCount);
    } else {
        printf("You lost, better luck next time!\n");
    }

    return 0;
}
