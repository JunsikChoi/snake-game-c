# Snake Game in C

This is a simple implementation of the classic snake game in C.
The game is played on a terminal and is controlled using the keyboard.
It's a great example of how to use basic C programming concepts to create a fun and playable game.

## Features & TODOs

- [ ] Simple and intuitive gameplay.
- [ ] Customizable game settings, including game speed and board size.
- [ ] Score tracking and display.
- [ ] Cross-platform compatibility.

## Installation

To play the game, you will need a C compiler installed on your system. The game has been tested with GCC on Ubuntu 22.04.

### Compiling the Game

1. Clone the repository:
   ```
   git clone https://github.com/junsikchoi/snake-game-c.git
   ```
2. Navigate to the cloned directory:
   ```
   cd snake-game-c
   ```
3. Compile the game using GCC or any C compiler:
   ```
   ./build.sh
   ```
   Note: The `-lncurses` flag is required for terminal handling (Linux/macOS).

### Running the Game

After compiling, you can start the game by running:

```
./build/snake-game-c
```

## Controls

- Use the `w` `s` `a` `d` to control the snake's direction.
- Press `q` to quit the game.

## Configuration

The game's settings, such as board size and game speed, can be adjusted in the `config.h` file before compiling.

## Contributing

Contributions to the project are welcome! Here are a few ways you can help:

- Reporting bugs
- Suggesting enhancements
- Adding new features
- Improving documentation

Please feel free to fork the repository and submit pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by the classic snake game.
