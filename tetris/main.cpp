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

int main() {
	// create the window for the tetris game
	sf::RenderWindow window(sf::VideoMode(320, 480), "Welcome to Tetris!");
	
	sf::Texture texture;
	texture.loadFromFile("tiles.png");

	sf::Sprite sprite(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 20, 20));
	while (window.isOpen()) {
		sf::Event game;
		while (window.pollEvent(game))
		{
			// if the game is closed, close the window
			if (game.type == sf::Event::Closed)
				window.close();
		}

		int n = 3;
		for (int i = 0; i < 4; ++i) {
			a[i].x = shapes[n][i] % 2;
			a[i].y = shapes[n][i] / 2;
		}

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