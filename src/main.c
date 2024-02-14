#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>

const int BOARD_SIZE = 40;
const int RENDER_SPEED_MS = 150;
const int GAME_SPEED = 1;
const int FOOD_CNT = 3;
const int INIT_SNAKE_LENGTH = 3;
enum boardElements { EMPTY, SNAKE, FOOD, WALL };
typedef enum Direction { UP, DOWN, RIGHT, LEFT } Direction;

typedef struct Segment {
  int row; // Segment Row Position
  int col; // Segment Col Position
  struct Segment* next;
} Segment; // Snake body segment

typedef struct Snake {
  Segment* head;
  int tailRow;
  int tailCol;
  int length;
  int newBorn;
  Direction currentDirection;
} Snake;

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

void updateBoard(char board[BOARD_SIZE][BOARD_SIZE], Snake* snake) {
  Segment* currentSegment = snake->head;

  while(currentSegment->next != NULL) {
    board[currentSegment->row][currentSegment->col] = SNAKE;
    currentSegment = currentSegment->next;
  }

  if (snake->newBorn) {
    snake->newBorn = 0;
  } else {
    board[snake->tailRow][snake->tailCol] = EMPTY;
  }

  return;
}

Snake* initSnake(int row, int col) {
  Snake* snake = (Snake*)malloc(sizeof(Snake));
  Segment* head = NULL;
  Segment* curr = NULL;
  Segment* prev = NULL;

  for (int i = 0; i < INIT_SNAKE_LENGTH; i++) {
    curr = (Segment*)malloc(sizeof(Segment));
    curr->row = row;
    curr->col = col-i;
    curr->next = NULL;

    if (i == 0) {
      head = curr;
    } else {
      prev->next = curr;
    }

    prev = curr;
  }
  
  snake->length = INIT_SNAKE_LENGTH;
  snake->currentDirection = RIGHT;
  snake->head = head;
  snake->newBorn = 1;

  return snake;
};

void moveSnake(Snake* snake, Direction direction) {
  snake->currentDirection = direction;
  int currentHeadPosRow = snake->head->row;
  int currentHeadPosCol = snake->head->col;


  Segment* currentSegment = snake->head;
  int tmpRow;
  int tmpCol;
  int first = 1;
  while(currentSegment->next != NULL) {
    snake->tailRow = currentSegment->next->row;
    snake->tailCol = currentSegment->next->col;
    if (first) {
      tmpRow = currentSegment->next->row;
      tmpCol = currentSegment->next->col;
      currentSegment->next->row = currentSegment -> row;
      currentSegment->next->col = currentSegment -> col;
      first = 0;
    } else {
      currentSegment->next->row = tmpRow;
      currentSegment->next->col = tmpCol;
      tmpRow = currentSegment->next->row;
      tmpCol = currentSegment->next->col;
    }
    currentSegment = currentSegment->next;
  }

  switch(snake->currentDirection) {
    case UP:
      snake->head->row = currentHeadPosRow - 1;
      break;

    case DOWN:
      snake->head->row = currentHeadPosRow + 1;
      break;

    case LEFT:
      snake->head->col = currentHeadPosCol - 1;
      break;

    case RIGHT:
      snake->head->col = currentHeadPosCol + 1;
      break;
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
  Snake* snake = initSnake(BOARD_SIZE/2, BOARD_SIZE/2);

  while (!quit) {
    clear();

    ch = getch();

    switch(ch) {
      case 'w':
        (snake->currentDirection == DOWN) ? moveSnake(snake, snake->currentDirection) : moveSnake(snake, UP);
        break;

      case 's':
        (snake->currentDirection == UP) ? moveSnake(snake, snake->currentDirection) : moveSnake(snake, DOWN);
        break;

      case 'a':
        (snake->currentDirection == RIGHT) ? moveSnake(snake, snake->currentDirection) : moveSnake(snake, LEFT);
        break;

      case 'd':
        (snake->currentDirection == LEFT) ? moveSnake(snake, snake->currentDirection) : moveSnake(snake, RIGHT);
        break;

      case 'q':
        printw("Quit Game");
        quit = 1;
        break;

      default:
        moveSnake(snake, snake->currentDirection);
        break;
    }

    updateBoard(gameBoard, snake);

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
