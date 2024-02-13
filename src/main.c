#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>

const int BOARD_SIZE = 40;
const int RENDER_SPEED_MS = 50;
const int GAME_SPEED = 1;
const int FOOD_CNT = 3;
const int INIT_SNAKE_LENGTH = 3;
enum boardElements { EMPTY, SNAKE, FOOD, WALL };

void placeFood(char board[BOARD_SIZE][BOARD_SIZE]) {
  int x, y;

  srand(time(NULL)); // seed rng

  do {
    x = rand() % BOARD_SIZE;
    y = rand() % BOARD_SIZE;
  } while (board[x][y] != EMPTY);

  board[x][y] = FOOD;
}

void initBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if ((i == 0) || (i == BOARD_SIZE - 1) || (j == 0) || (j == BOARD_SIZE -1)) {
        board[i][j] = WALL;
      }
      else {
        board[i][j] = EMPTY;
      }
    }
  }

  // Place Snake
  board[BOARD_SIZE/2][BOARD_SIZE/2] = SNAKE;
  board[BOARD_SIZE/2][BOARD_SIZE/2 - 1] = SNAKE;
  board[BOARD_SIZE/2][BOARD_SIZE/2 - 2] = SNAKE;
  
  // Place Food
  for (int i = 0; i < FOOD_CNT; i++) {
    placeFood(board);
  }
}

void renderBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      switch (board[i][j]) {
        case EMPTY:
          printw("   ");
          break;
        case SNAKE:
          printw(" @ ");
          break;
        case WALL:
          printw("###");
          break;
        case FOOD:
          printw(" X ");
          break;
      }
    }
    printw("\n");
  }
}

void startGame(char gameBoard[BOARD_SIZE][BOARD_SIZE]) {
  initscr(); // Start ncurses mode
  cbreak(); // No Buffering
  noecho(); // No Echo
  nodelay(stdscr, TRUE); // Non-blocking getch()
  keypad(stdscr, TRUE); // Enable Special Key 

  int quit = 0;
  int ch;

  while (!quit) {
    clear();

    ch = getch();

    switch(ch) {
      case 'w':
        printw("KEY_UP");
        break;

      case 's':
        printw("KEY_DOWN");
        break;

      case 'a':
        printw("KEY_LEFT");
        break;

      case 'd':
        printw("KEY_RIGHT");
        break;

      case 'q':
        printw("Quit Game");
        quit = 1;
        break;

      default:
        break;
    }

    renderBoard(gameBoard);
    refresh();

    usleep(1000 * RENDER_SPEED_MS / GAME_SPEED);
  }

  endwin();

  return;
}

int main(void) {
  char gameBoard[BOARD_SIZE][BOARD_SIZE];
  initBoard(gameBoard);

  startGame(gameBoard);
}
