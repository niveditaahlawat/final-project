#include <SFML/Graphics.hpp>
#include <time.h>

const int M = 20;
const int N = 10;

int grid[M][N] = { 0 };


class Point {
public:
	int x;
	int y;
}
a[4], b[4];

int shapes[7][4] = {
	3, 5, 4, 6,	// S shape
	2, 4, 5, 7, // Z shape
	3, 5, 4, 7, // T shape
	2, 3, 5, 7, // L shape
	1, 3, 5, 7, // Line shape
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
	sf::RenderWindow window(sf::VideoMode(320, 480), "Welcome to Tetris!");

	sf::Texture texture;
	texture.loadFromFile("tiles.png");

	sf::Sprite sprite(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 20, 20));

	double timer = 0.0;
	double delay = 0.2;

	sf::Clock clock;

	int dx = 0;	// set dx to 0 by default, before user rotates the piece
	bool rotate = false;
	int color_num = 1;

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

		// move pieces
		for (int i = 0; i < 4; ++i) {
			a[i].x += dx;
			b[i] = a[i];
		}
		if (!check_on_screen()) {
			for (int i = 0; i < 4; ++i) {
				a[i] = b[i];
			}
		}

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
				a[i].y += 1.0;
				b[i] = a[i];

				// randomly populate a tetris piece
				if (!check_on_screen()) {
					for (int i = 0; i < 4; ++i) {
						grid[b[i].y][b[i].x] = color_num;
					}
					color_num = 1 + rand() % 7;	// randomly choose one of the 7 colors from the tiles.png file
					int n = rand() % 7;
					for (int i = 0; i < 4; ++i) {
						a[i].x = shapes[n][i] % 2;
						a[i].y = shapes[n][i] / 2;
					}
				}

				timer = 0.0;
			}
		}

		// reset the orientation of the piece
		dx = 0;
		rotate = 0;

		// draw the tetris game
		// clear the window when the window is opened
		window.clear(sf::Color::Black);

		

		for (int i = 0; i < 4; ++i) {
			sprite.setPosition(a[i].x * 18, a[i].y * 18);
			window.draw(sprite);	// test the texture
		}
		window.display();
	}
	return 0;
}