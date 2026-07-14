#pragma once

#include <SFML/Graphics.hpp>

class Text {

	sf::Text text;
	sf::Font font;

public:

	void addDetails(sf::String string, std::string, float size, sf::Color color, sf::Vector2f position);

	sf::Text getText();

	void toString(sf::String);
};
