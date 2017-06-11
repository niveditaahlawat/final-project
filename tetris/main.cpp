#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

const int M = 20;
const int N = 10;

int grid[M][N] = { 0 };	// tetris board is empty

class Point {
public:
	int x;
	int y;
} a[4], b[4];

int shapes[7][4] = {
	1, 3, 5, 7, // Line shape
	2, 4, 5, 7, // Z shape
	3, 5, 4, 6,	// S shape
	3, 5, 4, 7, // T shape
	2, 3, 5, 7, // L shape	
	3, 5, 7, 6,	// J shape
	2, 3, 4, 5,	// Square shape
};

// make sure the pieces are within the playing board

bool check_on_screen() {
	for (int i = 0; i < 4; ++i) {
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
			return false;
		else if (grid[a[i].y][a[i].x])
			return false;
	}
	return true;
}

int main() {

	srand(time(0));	// seed a random time to randomly populate tetris pieces when playing

	// create the window for the tetris game
	sf::RenderWindow window(sf::VideoMode(200, 450), "WELCOME TO TETRIS!");

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

	// draw text
	std::string output_start("______________________ \n LINES CLEARED: ");
	std::string output_on("\n____ PLAYING TETRIS ____");
	std::string output_over("\n______ GAME OVER ______");

	sf::Text text;
	text.setString(output_start + std::to_string(num_lines) + output_on);
	text.setFont(font);
	text.setCharacterSize(16);
	text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, window.getSize().y - 40 - text.getGlobalBounds().height / 2);

	int dx = 0;	// set dx to 0 by default, before user rotates the piece
	bool rotate = false;
	int color_num = 1;
	double timer = 0.0;
	double delay = 0.3;

	int n = rand() % 7;
	for (int i = 0; i < 4; ++i) {
		a[i].x = shapes[n][i] % 2;
		a[i].y = shapes[n][i] / 2;
	}

	while (window.isOpen()) {

		double game_time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += game_time;

		sf::Event game;
		while (window.pollEvent(game))
		{
			// if the game is closed, close the window
			if (game.type == sf::Event::Closed)
				window.close();

			// configure keyboard press signals
			if (game.type == sf::Event::KeyPressed)
				if (game.key.code == sf::Keyboard::Up)
					rotate = true;
				else if (game.key.code == sf::Keyboard::Left)
					dx = -1;
				else if (game.key.code == sf::Keyboard::Right)
					dx = 1;
		}
		// press down to drag piece to bottom of screen
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			delay = 0.05;

		// move pieces
		[&]() {
			for (int i = 0; i < 4; ++i) {
				b[i] = a[i];
				a[i].x += dx;
			}
			if (!check_on_screen()) {
				for (int i = 0; i < 4; ++i) {
					a[i] = b[i];
				}
			}
		}();

		// rotate pieces
		if (rotate == true) {
			Point p = a[1];	// point of rotation
			for (int i = 0; i < 4; ++i) {
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check_on_screen()) {
				for (int i = 0; i < 4; ++i) {
					a[i] = b[i];
				}
			}
		}

		// move the piece down with each iteration of the timer
		if (timer > delay) {
			for (int i = 0; i < 4; ++i) {
				b[i] = a[i];
				a[i].y += 1.0;
			}

			// randomly populate a tetris piece
			if (!check_on_screen()) {
				for (int i = 0; i < 4; ++i) {
					grid[b[i].y][b[i].x] = color_num;
				}
				color_num = rand() % 7 + 1;	// randomly choose one of the 7 colors from the tiles.png file
				int n = rand() % 7;
				for (int i = 0; i < 4; ++i) {
					a[i].x = shapes[n][i] % 2;
					a[i].y = shapes[n][i] / 2;
				}
			}
			timer = 0.0;
		}

		// erase completed lines
		[&]() {
			int k = M - 1;
			for (int i = M - 1; i > 0; --i) {
				int line_count = 0;
				for (int j = 0; j < N; ++j) {
					if (grid[i][j]) {
						line_count++;
					}
					grid[k][j] = grid[i][j];
				}
				if (line_count < N) {
					k--;
				}
				if (line_count == N) {
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

		// draw the tetris game
		// clear the window when the window is opened
		window.clear(sf::Color::Black);

		window.draw(text);

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

		for (int i = 0; i < 4; ++i) {
			sprite.setTextureRect(sf::IntRect(color_num * 20, 20, 20, 20));
			sprite.setPosition(a[i].x * 20, a[i].y * 20);
			window.draw(sprite);
		}
		if (!check_on_screen()) {
			text.setString(output_start + std::to_string(num_lines) + output_over);
		}

		window.display();		// display the window
	}
	return 0;
}