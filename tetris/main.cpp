#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

const int M = 20;
const int N = 10;

int grid[M][N] = { 0 };	// tetris board is empty
// M and N are the dimensions of the tetris board

int shapes[7][4] = {
	1, 3, 5, 7, // Line shape
	2, 4, 5, 7, // Z shape
	3, 5, 4, 6,	// S shape
	3, 5, 4, 7, // T shape
	2, 3, 5, 7, // L shape	
	3, 5, 7, 6,	// J shape
	2, 3, 4, 5,	// Square shape
};

class TetrisPiece {
public:
	TetrisPiece() {
		int n = rand() % 7;
		color = n + 1;	// randomly choose one of the 7 colors from the tiles.png file
		for (int i = 0; i < 4; ++i) {
			piece[i].x = shapes[n][i] % 2;
			piece[i].y = shapes[n][i] / 2;
			piece[i].x += N / 2 - 1;	// tetris piece falls from center of board
		}
	}
	// Accessor functions
	int get_x(int index) const {
		return piece[index].x;
	}
	int get_y(int index) const {
		return piece[index].y;
	}
	int get_color() const {
		return color;
	}
	// Mutator functions
	int set_x(int index, int x1) {
		piece[index].x = x1;
	}
	int set_y(int index, int y1) {
		piece[index].y = y1;
	}
	int set_color(int c) {
		color = c;
	}
private:
	int color;
	struct Block {
		int x;
		int y;
	} piece[4];
};

// make sure the pieces are within the playing board
// make sure pieces are not overlapping
bool check_on_screen(TetrisPiece const &a) {
	for (int i = 0; i < 4; ++i) {
		if (a.get_x(i) < 0 || a.get_x(i) >= N || a.get_y(i))
			return false;
		else if (grid[a.get_y(i)][a.get_x(i)])
			return false;
	}
	return true;
}

void copy_piece(TetrisPiece const &a, TetrisPiece &a_backup) {
	for (int i = 0; i < 4; ++i) {
		a_backup.set_x(i, a.get_x(i));
		a_backup.set_y(i, a.get_y(i));
	}
	a_backup.set_color(a.get_color);
}

int main() {

	srand(time(0));	// seed a random time to randomly populate tetris pieces when playing

	// create the window for the tetris game
	sf::RenderWindow window(sf::VideoMode(200, 450), "TETRIS!");

	sf::Texture texture;
	texture.loadFromFile("tiles.png");

	sf::Sprite sprite(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 20, 20));

	sf::Clock clock;

	// load the font
	sf::Font font;
	if (!font.loadFromFile("impact.ttf")) {
		std::cout << "Font file not found" << std::endl;
	}

	int num_lines = 0;

	bool player_alive = true;

	// draw text

	std::string output_start("______________________ \n\t\t\t LINES CLEARED: ");
	std::string output_on("\n_____ PLAYING TETRIS _____");

	sf::Text text;
	text.setString(output_start + std::to_string(num_lines) + output_on);
	text.setFont(font);
	text.setCharacterSize(16);
	text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, window.getSize().y - 40 - text.getGlobalBounds().height / 2);

	std::string output_over("GAME OVER!");

	sf::Text game_over_text;
	game_over_text.setString(output_over);
	game_over_text.setFont(font);
	game_over_text.setCharacterSize(40);
	game_over_text.setPosition(6, 100);

	int dx = 0;	// set dx to 0 by default, before user rotates the piece
	bool rotate = false;
	int color_num = 1;
	double timer = 0.0;
	double delay = 0.3;

	TetrisPiece *a, *a_backup;
	
	while (window.isOpen()) {

		a = new(TetrisPiece);
		a_backup = new(TetrisPiece);

		// create first tetris piece
		// create backup tetris piece


		timer += clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Event game;
		while (window.pollEvent(game)) {
			// if the game is closed, close the window
			if (game.type == sf::Event::Closed)
				window.close();

			// configure keyboard press signals
			if (game.type == sf::Event::KeyPressed) {
				if (game.key.code == sf::Keyboard::Up)
					rotate = true;
				else if (game.key.code == sf::Keyboard::Left)
					dx = -1;
				else if (game.key.code == sf::Keyboard::Right)
					dx = 1;
			}
		}
		// press down to drag piece to bottom of screen
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			delay = 0.05;

		// move pieces
		[&]() {
			// replace this code with assignment operator
			for (int i = 0; i < 4; ++i) {
				a_backup[i] = a[i];	// make a copy of the original piece here for future use (ex: checking valid transformations)
				a[i].x += dx;
			}
			if (!check_on_screen()) {
				for (int i = 0; i < 4; ++i) {
					a[i] = a_backup[i];	// if illegal transformation, restore the backup copy (Piece b)
				}
			}
		}();

		// rotate pieces
		if (rotate == true) {
			Piece p = a[1];	// point of rotation
			for (int i = 0; i < 4; ++i) {
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			// again, check for valid transformation. if not valid, restore Piece b
			if (!check_on_screen()) {
				for (int i = 0; i < 4; ++i) {
					a[i] = a_backup[i];
				}
			}
		}

		// move the piece down with each iteration of the timer
		if (timer > delay) {
			for (int i = 0; i < 4; ++i) {
				a_backup[i] = a[i];
				a[i].y += 1.0;
			}

			// randomly populate a tetris piece
			if (!check_on_screen()) {
				for (int i = 0; i < 4; ++i) {
					// freeze the colors of the grid with the color of the piece
					grid[a_backup[i].y][a_backup[i].x] = color_num;
				}
				color_num = rand() % 7 + 1;	// randomly choose one of the 7 colors from the tiles.png file
				int n = rand() % 7;
				for (int i = 0; i < 4; ++i) {
					a[i].x = shapes[n][i] % 2;
					a[i].y = shapes[n][i] / 2;
					a[i].x += N / 2 - 1;	// center all new pieces
				}
				// check for game over if a piece has been added but it is invalid (aka at top of screen)
				if (!check_on_screen()) {
					player_alive = false;
				}
			}
			timer = 0.0;
		}

		// erase completed lines
		[&]() {
			int k = M - 1;
			for (int i = M - 1; i > 0; --i) {
				int col_count = 0;
				for (int j = 0; j < N; ++j) {
					if (grid[i][j]) {
						col_count++;
					}
					grid[k][j] = grid[i][j];	// overwrite previous line with new line
				}
				if (col_count < N) {
					k--;
				}
				if (col_count == N) {
					num_lines++;
					text.setString(output_start + std::to_string(num_lines) + output_on);
				}
			}
		}();

		// could create a generic algorithm to figure out how many rows are occupied
		// and use count with a predicate, which could be specified by a lambda function
		// this, however, complicates the problem, but would implement 10C material

		// reset the orientation of the piece
		dx = 0;
		rotate = 0;

		// reset the delay
		delay = 0.3;

		if (player_alive) {
			// draw the tetris game window
			window.clear(sf::Color::Black);

			// draw the previous pieces
			[&]() {
				for (int i = 0; i < M; ++i) {
					for (int j = 0; j < N; ++j) {
						if (grid[i][j] == 0)
							continue;
						sprite.setTextureRect(sf::IntRect(grid[i][j] * 20, 0, 20, 20));
						sprite.setPosition(j * 20, i * 20);
						window.draw(sprite);
					}
				}
			}();

			// draw current piece
			for (int i = 0; i < 4; ++i) {
				sprite.setTextureRect(sf::IntRect(color_num * 20, 0, 20, 20));
				sprite.setPosition(a[i].x * 20, a[i].y * 20);
				window.draw(sprite);
			}
		}

		if (!player_alive) {
			window.draw(game_over_text);
		}
		else {
			window.draw(text);
		}

		window.display();		// display the window
	}
	return 0;
}