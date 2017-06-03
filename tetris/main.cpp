#include <SFML/Graphics.hpp>
#include <time.h>

int main() {
	// create the window for the tetris game
	sf::RenderWindow window(sf::VideoMode(320, 480), "Welcome to Tetris!");
	
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
		window.display();
	}
	return 0;
}