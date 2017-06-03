#include <SFML/Graphics.hpp>
#include <time.h>

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
		// clear the window when the window is opened
		window.clear(sf::Color::Black);
		window.draw(sprite);	// test the texture
		window.display();
	}
	return 0;
}