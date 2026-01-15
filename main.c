#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define RESET  "\x1b[0m"
#define GREEN  "\x1b[1;32m"
#define RED    "\x1b[1;31m"
#define YELLOW "\x1b[1;33m"
#define CYAN   "\x1b[1;36m"

int player_score = 0;
int computer_score = 0;

void winnerSound() {
    Beep(523, 200); Beep(659, 200); Beep(784, 400);
}

void loserSound() {
    Beep(300, 500);
}

void typeText(const char *text, int delay) {
    while (*text) { printf("%c", *text); fflush(stdout); Sleep(delay); text++; }
}

void computerThinking() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf("\rComputer is choosing");
        for (int j = 0; j <= i; j++) printf(".");
        fflush(stdout); Sleep(400);
    }
    printf("\n");
}

void shootAnimation() {
    typeText("Rock...\n", 60); Sleep(200);
    typeText("Paper...\n", 60); Sleep(200);
    typeText("Scissor...\n", 60); Sleep(200);
    typeText("SHOOT!!!\n\n", 30);
}

char getChoice(int player) {
    char ch;
    while (1) {
        printf("Player %d: Enter choice (R=Rock, P=Paper, S=Scissor): ", player);
        scanf(" %c", &ch);
        ch = tolower(ch);
        if (ch == 'r' || ch == 'p' || ch == 's') return ch;
        printf(RED "Invalid! Enter R, P, or S.\n" RESET);
    }
}

char getComputerChoice() {
    int r = rand() % 3;
    return (r == 0 ? 'r' : (r == 1 ? 'p' : 's'));
}

void printChoiceName(char c) {
    if (c == 'r') printf(YELLOW "Rock" RESET);
    else if (c == 'p') printf(CYAN "Paper" RESET);
    else printf(RED "Scissor" RESET);
}

int winnerBetweenTwo(char a, char b) {
    if (a == b) return 0;
    if ((a == 'r' && b == 's') || (a == 'p' && b == 'r') || (a == 's' && b == 'p')) return 1;
    return 2;
}

int main() {
    srand(time(NULL));
    char menuInput[10], sideInput[10], againInput[10];
    int menuChoice;

    while (1) {
        printf(CYAN "\n==== ROCK PAPER SCISSORS ====\n" RESET);
        printf("1. 3 Player Mode\n2. Human vs Computer\n3. Exit\nEnter choice: ");
        scanf("%s", menuInput);

        menuChoice = atoi(menuInput);
        if (menuChoice == 3) break;
        if (menuChoice != 1 && menuChoice != 2) {
            printf(RED "Invalid choice! Enter 1, 2 or 3.\n" RESET);
            continue;
        }

        if (menuChoice == 1) {
            shootAnimation();
            char p1 = getChoice(1), p2 = getChoice(2), p3 = getChoice(3);

            printf("\nResults -> P1: "); printChoiceName(p1);
            printf(" | P2: "); printChoiceName(p2);
            printf(" | P3: "); printChoiceName(p3);
            printf("\n");

            if ((p1 == p2 && p2 == p3) || (p1 != p2 && p2 != p3 && p1 != p3)) {
                printf(YELLOW "It's a Tie between all three!\n" RESET);
            } 
            else {
                int win_p1 = 0, win_p2 = 0, win_p3 = 0;
                if (winnerBetweenTwo(p1, p2) == 1) win_p1++; else if (winnerBetweenTwo(p1, p2) == 2) win_p2++;
                if (winnerBetweenTwo(p2, p3) == 1) win_p2++; else if (winnerBetweenTwo(p2, p3) == 2) win_p3++;
                if (winnerBetweenTwo(p1, p3) == 1) win_p1++; else if (winnerBetweenTwo(p1, p3) == 2) win_p3++;

                if (win_p1 == 2) { printf(GREEN "Player 1 is the Solo Winner!\n" RESET); winnerSound(); }
                else if (win_p2 == 2) { printf(GREEN "Player 2 is the Solo Winner!\n" RESET); winnerSound(); }
                else if (win_p3 == 2) { printf(GREEN "Player 3 is the Solo Winner!\n" RESET); winnerSound(); }
                else {
                    int id_a, id_b;
                    if (win_p1 == 1 && win_p2 == 1) { id_a = 1; id_b = 2; }
                    else if (win_p1 == 1 && win_p3 == 1) { id_a = 1; id_b = 3; }
                    else { id_a = 2; id_b = 3; }

                    printf(YELLOW "\nPlayer %d and Player %d Tie! Starting SUB-ROUND...\n" RESET, id_a, id_b);
                    char sub_a = getChoice(id_a), sub_b = getChoice(id_b);
                    int sub_res = winnerBetweenTwo(sub_a, sub_b);
                    if (sub_res == 0) printf(YELLOW "Sub-round Tie!\n" RESET);
                    else printf(GREEN "Final Winner: Player %d!\n" RESET, (sub_res == 1 ? id_a : id_b));
                    winnerSound();
                }
            }
        }
        else if (menuChoice == 2) {
            int hp;
            while (1) {
                printf("Choose your side (1 for P1, 2 for P2): ");
                scanf("%s", sideInput);
                hp = atoi(sideInput);
                if (hp == 1 || hp == 2) break;
                printf(RED "Invalid! Enter 1 or 2 only.\n" RESET);
            }

            char hc = getChoice(hp);
            char cc = getComputerChoice();
            computerThinking();

            // এখানে কম্পিউটারের চয়েস দেখানো হচ্ছে
            printf("\nYour Choice: "); printChoiceName(hc);
            printf("\nComputer's Choice: "); printChoiceName(cc);
            printf("\n");

            int res = winnerBetweenTwo(hc, cc);
            if (res == 0) printf(YELLOW "It's a Tie!\n" RESET);
            else if (res == 1) { printf(GREEN "Hurray! You WIN!\n" RESET); winnerSound(); player_score++; }
            else { printf(RED "Computer Wins!\n" RESET); loserSound(); computer_score++; }

            printf(CYAN "Score -> You: %d | Comp: %d\n" RESET, player_score, computer_score);
        }

        while (1) {
            printf("\nPlay again? (y/n): ");
            scanf("%s", againInput);
            char a = tolower(againInput[0]);
            if (strlen(againInput) == 1 && (a == 'y' || a == 'n')) {
                if (a == 'n') { printf(GREEN "Goodbye!\n" RESET); return 0; }
                break;
            }
            printf(RED "Invalid! Enter 'y' or 'n'.\n" RESET);
        }
    }
    return 0;
}
