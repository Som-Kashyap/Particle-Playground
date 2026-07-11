#include <SFML/Graphics.hpp>
#include "Text.h"
#include <iostream>
#include <format>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

enum class particleType {

	magical,
	freeFall,
	followers,
	wave
};

class Particle {

public:
	Particle(Text& radiusText);
	sf::CircleShape particleShape;
	sf::Vector2f velocity;
	sf::Vector2f maxVelocity = (sf::Vector2f(800.f, 500.f));
	sf::Vector2f acceleration;
	vector<sf::Color>magicalPalette = { sf::Color(224,63,216,rand()%121+50) , sf::Color(255, 192, 203,rand() % 121 + 50) , sf::Color(0, 255, 255,rand() % 121 + 50)};

	float particleRadius = 2.f;
	
	float lifeTime = 0.f;
	bool draw = true;

	void update(float& deltaTime , float& gravity, particleType& type);
};

Particle::Particle(Text& radiusText) {

	particleShape.setRadius(particleRadius);
	particleShape.setFillColor(magicalPalette[rand() % 3]);
	velocity = sf::Vector2f(0.f, 0.f);
	radiusText.toString("Radius: " + to_string(particleRadius));

}

void Particle::update(float& deltaTime, float& gravity, particleType& type) {

	
	//velocity.y += gravity * deltaTime;

	lifeTime += deltaTime;

	if (type == particleType::magical) {

		if (lifeTime >= 1.5f && lifeTime < 3.f) {
			float opacity = particleShape.getFillColor().a;
			opacity -= deltaTime;
			if (opacity < 0) {
				opacity = 255;
				float radius = particleShape.getRadius();
				radius *= 1.5f;
				//if (radius >= 20.f) radius = 20.f;
				particleShape.setRadius(radius);
			}
			sf::Color color = particleShape.getFillColor();
			color.a = opacity;
			particleShape.setFillColor(color);
	}

		if (lifeTime >= 3.f) {
			lifeTime = 0.f;
			draw = false;
		}
	}

	if (type == particleType::freeFall) {
		particleShape.setFillColor(sf::Color::Cyan);
	}

	if (type == particleType::followers || type == particleType::wave) {
		acceleration = sf::Vector2f( rand()%400,  rand()%400);
		velocity.x += acceleration.x * deltaTime;
		velocity.y += acceleration.y * deltaTime;
	}
	
	particleShape.move(velocity*deltaTime);

}

class Game {

public:
	sf::RenderWindow window;
	sf::RectangleShape HUD;
	sf::CircleShape glowCentre;
	sf::CircleShape circleOne;
	sf::CircleShape circleTwo;
	sf::CircleShape circleThree;
	sf::CircleShape circleFour;
	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;
	float gravity = 980.f;
	float format = ("{:.2f}", gravity);

	bool isEmitting = false;
	float blinkTimer = 0.f;

	Game();
	particleType type;

	Text stateText;
	Text radiusText;
	Text gravityText;
	Text clearText;

	void update();
	void handleEvents();
	void render();
	void run();

	vector<Particle>particleVector;
};

Game::Game() : window(sf::VideoMode(800, 600), "Particle Generator") {

	type = particleType::magical;

	HUD.setSize(sf::Vector2f(150.f, 100.f));
	sf::Color HUDcolor = sf::Color::Red;
	HUDcolor.a = 100.f;
	HUD.setFillColor(HUDcolor);

	glowCentre.setRadius(3.f);
	glowCentre.setFillColor(sf::Color(255,255,255,255));
	glowCentre.setOrigin(glowCentre.getRadius(),glowCentre.getRadius());
	circleOne.setRadius(5.f);
	circleOne.setFillColor(sf::Color(255, 255, 255, 160));
	circleOne.setOrigin(circleOne.getRadius(), circleOne.getRadius());
	circleTwo.setRadius(7.f);
	circleTwo.setFillColor(sf::Color(255, 255, 255, 120));
	circleTwo.setOrigin(circleTwo.getRadius(), circleTwo.getRadius());
	circleThree.setRadius(11.f);
	circleThree.setFillColor(sf::Color(255, 255, 255, 80));
	circleThree.setOrigin(circleThree.getRadius(), circleThree.getRadius());
	circleFour.setRadius(15.f);
	circleFour.setFillColor(sf::Color(255, 255, 255, 40));
	circleFour.setOrigin(circleFour.getRadius(), circleFour.getRadius());
	window.setMouseCursorVisible(false);

	stateText.addDetails("Mode: Magical","resources/arial.ttf", 15 , sf::Color::White,sf::Vector2f(10., 10.));
	radiusText.addDetails("Radius: ", "resources/arial.ttf", 15, sf::Color::White, sf::Vector2f(10., 30.));
	gravityText.addDetails("Gravity: " + to_string((int)gravity), "resources/arial.ttf", 15, sf::Color::White, sf::Vector2f(10., 50.));
	clearText.addDetails("Clear!(NUM0) " , "resources/arial.ttf", 15, sf::Color::White, sf::Vector2f(10., 70.));
}

void Game::handleEvents() {

	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {

			isEmitting = true;

			for (size_t i = 0; i < 50; i++) {
				Particle particleOBJ(radiusText);
				sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				particleOBJ.particleShape.setPosition(mousepos);
				particleVector.emplace_back(particleOBJ);
			}
			
		}
		else {
			isEmitting = false;
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num0) {
			particleVector.clear();
		}

		if (type == particleType::freeFall) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
				gravity += 10;
				gravityText.toString("Gravity(^): " + to_string((int)gravity));
				cout << "Gravity: " << gravity << " px/s*s" << endl;
			}
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
			if (type == particleType::magical) {
				type = particleType::freeFall;
				stateText.toString("Mode: Free Fall");
			}
			else if (type == particleType::freeFall) {
				type = particleType::followers;
				stateText.toString("Mode: Followers");
			}
			else if (type == particleType::followers) {
				type = particleType::wave;
				stateText.toString("Mode: Wave");
			}
			else if (type == particleType::wave) {
				type = particleType::magical;
				stateText.toString("Mode: Magical");
			}
		}

	}
}

void Game::update() {

	sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	glowCentre.setPosition(mousepos);
	circleOne.setPosition(mousepos);
	circleTwo.setPosition(mousepos);
	circleThree.setPosition(mousepos);
	circleFour.setPosition(mousepos);

	for (auto& particle : particleVector) {
		particle.update(deltaTime,gravity,type);
	}

	for (size_t i = 0; i < particleVector.size(); i++) {

		if (particleVector[i].particleShape.getPosition().y >= window.getSize().y || particleVector[i].particleShape.getPosition().y <= 0) {
			particleVector[i].velocity.y = -particleVector[i].velocity.y;
		}
		if (particleVector[i].particleShape.getPosition().x <= 0 || particleVector[i].particleShape.getPosition().x >= window.getSize().x) {
			particleVector[i].velocity.x = -particleVector[i].velocity.x;
		}
	}

	if (type == particleType::magical) {
		for (size_t i = 0; i < particleVector.size(); i++) {

			particleVector[i].acceleration.x += (rand() % 100) - 50;
			particleVector[i].acceleration.y += (rand() % 100) - 50;
			particleVector[i].velocity.x += particleVector[i].acceleration.x * deltaTime;
			particleVector[i].velocity.y += particleVector[i].acceleration.y * deltaTime;
			if (particleVector[i].velocity.x > particleVector[i].maxVelocity.x) {
				particleVector[i].velocity.x = particleVector[i].maxVelocity.x;
			}
			if (particleVector[i].velocity.y > particleVector[i].maxVelocity.y) {
				particleVector[i].velocity.y = particleVector[i].maxVelocity.y;
			}
		}
	}

	if (type == particleType::freeFall) {
		for (size_t i = 0; i < particleVector.size(); i++) {

			if (particleVector[i].particleShape.getPosition().y >= window.getSize().y) {
				particleVector[i].particleShape.setPosition(particleVector[i].particleShape.getPosition().x, window.getSize().y - particleVector[i].particleShape.getScale().y);
				particleVector[i].velocity.y = 0;
			}
			if (particleVector[i].particleShape.getPosition().x <= 0 || particleVector[i].particleShape.getPosition().x >= window.getSize().x) {
				particleVector[i].velocity.x = 0;
			}

			particleVector[i].velocity.y += gravity * deltaTime;
		}

	}

	if (type == particleType::followers) {
		for (size_t i = 0; i < particleVector.size(); i++) {

			if (particleVector[i].particleShape.getPosition().x < window.getSize().x && particleVector[i].particleShape.getPosition().x >= 0 && particleVector[i].particleShape.getPosition().y >= 0 && particleVector[i].particleShape.getPosition().y < window.getSize().y) {
				if (particleVector[i].particleShape.getPosition().x < mousepos.x) {
					particleVector[i].particleShape.move(particleVector[i].velocity.x, 0);
				}
				if (particleVector[i].particleShape.getPosition().x > mousepos.x) {
					particleVector[i].particleShape.move(-particleVector[i].velocity.x, 0);
				}
				if (particleVector[i].particleShape.getPosition().y > mousepos.y) {
					particleVector[i].particleShape.move(0, -particleVector[i].velocity.y);
				}
				if (particleVector[i].particleShape.getPosition().y < mousepos.y) {
					particleVector[i].particleShape.move(0, particleVector[i].velocity.y);
				}
			}
			
		}
	}

	//if (isEmitting) {
	//	blinkTimer += deltaTime;
	//	if (blinkTimer >= 0.1) {
	//		blinkTimer = 0.f;
	//		//if (glowCentre.getRadius() == 10.f) glowCentre.setRadius(5.f);
	//		//else glowCentre.setRadius(10.f);
	//		if (glowCentre.getFillColor() == sf::Color::White) {
	//			glowCentre.setFillColor(sf::Color::Black);
	//		}
	//		else glowCentre.setFillColor(sf::Color::White);
	//	}
	//}

}

void Game::render() {

	window.clear();

	for (auto& particle : particleVector) {
		if(particle.draw) window.draw(particle.particleShape);
	}

	if (type == particleType::freeFall) {
		window.draw(gravityText.getText());
	}
		window.draw(HUD);
		window.draw(clearText.getText());
		window.draw(stateText.getText());
		window.draw(radiusText.getText());
		window.draw(circleFour);
		window.draw(circleThree);
		window.draw(circleTwo);
		window.draw(circleOne);
		window.draw(glowCentre);
	
	window.display();
}

void Game::run() {

	while (window.isOpen()) {

		deltaTime = deltaTimeClock.restart().asSeconds();

		update();
		handleEvents();
		render();
	}

}

int main() {

	Game game;
	game.run();

}