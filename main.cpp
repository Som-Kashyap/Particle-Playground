#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

class Particle {

public:
	Particle();
	sf::CircleShape particleShape;
	sf::Vector2f velocity;
	sf::Vector2f maxVelocity = (sf::Vector2f(800.f, 500.f));
	sf::Vector2f acceleration;
	
	float lifeTime = 0.f;
	bool draw = true;

	void update(float& deltaTime , float& gravity);
};

Particle::Particle() {

	particleShape.setRadius(2.f);
	particleShape.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	velocity = sf::Vector2f(0.f, 0.f);
	

}

void Particle::update(float& deltaTime, float& gravity) {

	//velocity.x += acceleration.x*deltaTime;
	//velocity.y += acceleration.y*deltaTime;
	//velocity.y += gravity * deltaTime;

	lifeTime += deltaTime;

	if (lifeTime >= 1.5f && lifeTime < 3.f) {
		float opacity = particleShape.getFillColor().a;
		opacity -= deltaTime;
		if (opacity < 0) {
			opacity = 255;
			opacity -= 100*deltaTime;
			float radius = particleShape.getRadius();
			radius *= 1.5f;
			if (radius >= 20.f) radius = 20.f;
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
	
	particleShape.move(velocity*deltaTime);

}

class Game {

public:
	sf::RenderWindow window;
	sf::CircleShape glowCentre;
	sf::CircleShape circleOne;
	sf::CircleShape circleTwo;
	sf::CircleShape circleThree;
	sf::CircleShape circleFour;
	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;
	float gravity = 980.f;

	bool isEmitting = false;
	float blinkTimer = 0.f;

	Game();

	void update();
	void handleEvents();
	void render();
	void run();

	vector<Particle>particleVector;
};

Game::Game() : window(sf::VideoMode(800, 600), "Particle Generator") {

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
}

void Game::handleEvents() {

	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {

			isEmitting = true;

			Particle particleOBJ;
			sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			glowCentre.setPosition(mousepos);
			circleOne.setPosition(mousepos);
			circleTwo.setPosition(mousepos);
			circleThree.setPosition(mousepos);
			circleFour.setPosition(mousepos);
			particleOBJ.particleShape.setPosition(mousepos);
			particleVector.emplace_back(particleOBJ);
		}
		else {
			isEmitting = false;
		}


	}
}

void Game::update() {

	for (auto& particle : particleVector) {
		particle.update(deltaTime,gravity);
	}

	for (size_t i = 0; i < particleVector.size(); i++) {

		if (particleVector[i].particleShape.getPosition().y >= window.getSize().y || particleVector[i].particleShape.getPosition().y <= 0) {
			particleVector[i].velocity.y = -particleVector[i].velocity.y;
		}
		if (particleVector[i].particleShape.getPosition().x <= 0 || particleVector[i].particleShape.getPosition().x >= window.getSize().x) {
			particleVector[i].velocity.x = -particleVector[i].velocity.x;
		}
	}

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
	if (isEmitting) {
		window.draw(circleFour);
		window.draw(circleThree);
		window.draw(circleTwo);
		window.draw(circleOne);
		window.draw(glowCentre);
	}
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