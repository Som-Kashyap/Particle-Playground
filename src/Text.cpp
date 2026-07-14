#include<SFML/Graphics.hpp>
#include "Text.h"

void Text::addDetails(sf::String string, std::string filename, float size, sf::Color color, sf::Vector2f position) {

	this->font.loadFromFile(filename);
	this->text.setFont(font);
	this->text.setString(string);
	this->text.setCharacterSize(size);
	this->text.setFillColor(color);
	this->text.setPosition(position);

}

sf::Text Text::getText() {
	return text;
}

void Text::toString(sf::String string) {
	this->text.setString(string);
}