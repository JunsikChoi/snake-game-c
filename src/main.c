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

int quit = 0;
int gameover = 0;
int bestScore = INIT_SNAKE_LENGTH;

typedef struct Segment {
  int row; // Segment Row Position
  int col; // Segment Col Position
  struct Segment* next;
} Segment; // Snake body segment

typedef struct Snake {
  Segment* head;
  Segment* tail;
  int tailRow;
  int tailCol;
  int length;
  int newBorn;
  Direction currentDirection;
} Snake;

void placeFood(char board[BOARD_SIZE][BOARD_SIZE]) {
  int row, col;

  srand(time(NULL)); // seed rng

  do {
    row = rand() % BOARD_SIZE;
    col = rand() % BOARD_SIZE;
  } while (board[row][col] != EMPTY);

  board[row][col] = FOOD;
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

void renderHeader(Snake* snake) {
  printw("\n");
  printw("SNAKE GAME v0.0.1\n");
  printw("UP : W / DOWN : S / LEFT : A / RIGHT : D / Quit: Q\n");
  printw("SCORE: %d \n", snake->length);
  printw("BEST SCORE: %d \n\n", bestScore);
}

void updateBoard(char board[BOARD_SIZE][BOARD_SIZE], Snake* snake) {
  int shouldGrow = 0;
  Segment* currentSegment = snake->head;

  int headContact = board[snake->head->row][snake->head->col];
  if (headContact == FOOD) {
    shouldGrow = 1;
  } else if (headContact == SNAKE || headContact == WALL) {
    if ((snake->length) > bestScore) {
      bestScore = snake->length;
    }
    gameover = 1;
  }

  while(currentSegment->next != NULL) {
    board[currentSegment->row][currentSegment->col] = SNAKE;
    currentSegment = currentSegment->next;
  }

  if (snake->newBorn) {
    snake->newBorn = 0;
  } else {
    board[snake->tailRow][snake->tailCol] = EMPTY;
  }

  if (shouldGrow) {
    Segment* newSegment = (Segment*)malloc(sizeof(Segment));
    newSegment->row = snake->tailRow;
    newSegment->col = snake->tailCol;
    newSegment->next = NULL;
    snake->length++;
    snake->tail->next = newSegment;
    snake->tail = newSegment;
    placeFood(board);
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
  
  snake->head = head;
  snake->tail = curr;
  snake->length = INIT_SNAKE_LENGTH;
  snake->currentDirection = RIGHT;
  snake->newBorn = 1;

  return snake;
};

void moveSnake(Snake* snake, Direction direction) {
  // Update snake's current direction
  snake->currentDirection = direction;

  // Store previous segment
  int prevRow = snake->head->row;
  int prevCol = snake->head->col;
  int tmpRow, tmpCol;

  // Update Head Position
  switch(snake->currentDirection) {
    case UP:
      snake->head->row -= 1;
      break;

    case DOWN:
      snake->head->row += 1;
      break;

    case LEFT:
      snake->head->col -= 1;
      break;

    case RIGHT:
      snake->head->col += 1;
      break;
  }

  // Update following segments
  Segment* current = snake->head->next;
  
  while (current != NULL) {
    // Store last segment's position to clear
    snake->tailRow = current->row;
    snake->tailCol = current->col;

    // Temporarily store current position
    tmpRow = current->row;
    tmpCol = current->col;

    // Update current position to previous segment's position
    current->row = prevRow;
    current->col = prevCol;

    // Update previous segment position to current segment's position
    prevRow = tmpRow;
    prevCol = tmpCol;

    // move to next segment
    current = current->next;
  }

  return;
}

void startGame(char gameBoard[BOARD_SIZE][BOARD_SIZE]) {
  int ch;
  Snake* snake = initSnake(BOARD_SIZE/2, BOARD_SIZE/2);

  while (!gameover) {
    // Clear screen
    clear();

    // Render Header
    renderHeader(snake);

    // Get input from keyboard
    ch = getch();

    // Process Input
    switch(ch) {
      case 'w':
        // Prevent snake move to opposite direction
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

    // Update game status
    updateBoard(gameBoard, snake);

    // Render gameboard
    renderBoard(gameBoard);

    // Refresh screen
    refresh();

    // Set delay
    usleep(1000 * RENDER_SPEED_MS / GAME_SPEED);
  }

  return;
}

int main(void) {
  initscr(); // Start ncurses mode
  cbreak(); // No Buffering
  noecho(); // No Echo
  nodelay(stdscr, TRUE); // Non-blocking getch()
  keypad(stdscr, TRUE); // Enable Special Key 

  while(!quit) {
    gameover = 0;
    char gameBoard[BOARD_SIZE][BOARD_SIZE];
    initBoard(gameBoard);
    startGame(gameBoard);
  }
  endwin();
  return 1;
}
