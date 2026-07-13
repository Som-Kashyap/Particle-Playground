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
	constrained,
	wave
};

class Particle {

public:
	Particle(Text& radiusText, particleType& type);
	sf::CircleShape particleShape;
	sf::Vector2f velocity;
	sf::Vector2f maxVelocity = (sf::Vector2f(800.f, 500.f));
	sf::Vector2f acceleration;
	vector<sf::Color>magicalPalette = { sf::Color(224,63,216,rand()%121+50) , sf::Color(255, 192, 203,rand() % 121 + 50) , sf::Color(0, 255, 255,rand() % 121 + 50)};
	vector<sf::Color>constrainedPalette = { sf::Color(0,0,255,rand() % 121 + 50), sf::Color(173, 216, 230,rand() % 121 + 50), sf::Color(255,255,255,rand() % 121 + 50)};
	vector<sf::Color>wavePalette = { sf::Color(255, 165, 0,rand() % 121 + 50),sf::Color(255, 255, 0,rand() % 121 + 50),sf::Color(255,0,0,rand() % 121 + 50) };

	float particleRadius;
	
	float lifeTime = 0.f;
	bool draw = true;

	int low;
	int high;

	void update(float& deltaTime , float& gravity, particleType& type);
};

Particle::Particle(Text& radiusText, particleType& type) {

	low= 5;
	high = 6;
	particleRadius = rand() % high + low;
	particleShape.setRadius(particleRadius);
	velocity = sf::Vector2f(0.f, 0.f);
	radiusText.toString("Radius: " + to_string(low) + " to " + to_string(high+low) + " px");

	
}

void Particle::update(float& deltaTime, float& gravity, particleType& type) {

	
	//velocity.y += gravity * deltaTime;

	lifeTime += deltaTime;

	if (type == particleType::magical) {

		particleShape.setFillColor(magicalPalette[rand() % 3]);

		if (lifeTime >= 1.5f && lifeTime < 3.f) {
			float radius = particleShape.getRadius();
			radius *= 1.5f;
			if (radius >= 20.f) radius = 20.f;
	}

		if (lifeTime >= 3.f) {
			lifeTime = 0.f;
			draw = false;
		}
	}

	if (type == particleType::freeFall) {
		particleShape.setFillColor(sf::Color::Cyan);
	}

	if (type == particleType::constrained) {
		particleShape.setFillColor(constrainedPalette[rand() % 3]);
	}

	if (type == particleType::constrained) {
		acceleration = sf::Vector2f(rand()%400, rand() % 400);
		velocity.x += acceleration.x * deltaTime;
		velocity.y += acceleration.y * deltaTime;
		if (lifeTime >= 3) {
			lifeTime = 0.f;
			draw = false;

		}
	}
	
	if (type == particleType::wave) {

		acceleration = sf::Vector2f(rand() % 101 + 100, rand() % 101 + 100);
		velocity.x += acceleration.x * deltaTime;
		velocity.y += acceleration.y * deltaTime;
		particleShape.setFillColor(wavePalette[rand() % 3]);

		if (lifeTime > 3 && lifeTime < 6) {
			sf::Color color = particleShape.getFillColor();
			float opacity = color.a;
			if (opacity < 200) {
				opacity = 200;
			}
			else opacity = rand() % 121 + 50;

			particleShape.setFillColor(color);
		}

		if (lifeTime >= 6) {
			lifeTime = 0.f;
			draw = false;

		}
	}
	particleShape.move(velocity*deltaTime);

}

class Game {

public:
	sf::RenderWindow window;
	sf::RectangleShape HUD;
	sf::RectangleShape controlsDisplay;
	sf::CircleShape glowCentre;
	sf::CircleShape circleOne;
	sf::CircleShape circleTwo;
	sf::CircleShape circleThree;
	sf::CircleShape circleFour;
	sf::CircleShape outerCircle;

	sf::Clock deltaTimeClock;
	float deltaTime = 0.f;
	float FPS = 0.f;
	float gravity = 980.f;
	float format = ("{:.2f}", gravity);

	bool showControls = false;
	bool isEmitting = false;
	float blinkTimer = 0.f;
	
	vector<Particle>particleVector;

	Game();
	particleType type;
	int spawnCount = 100;

	Text stateText;
	Text radiusText;
	Text gravityText;
	Text sizeText;
	Text FPStext;
	Text spawnCountText;

	Text spawnHelpText;
	Text modeHelpText;
	Text clearHelpText;
	Text controlsHelpText;
	Text helpText;
	Text gravityHelpText;

	void update();
	void handleEvents();
	void render();
	void run();

	
};

Game::Game() : window(sf::VideoMode(800, 600), "Particle Generator") {

	window.setFramerateLimit(60);

	particleVector.reserve(20000);

	type = particleType::magical;

	HUD.setSize(sf::Vector2f(150.f, 180.f));
	sf::Color HUDcolor = sf::Color::Red;
	HUDcolor.a = 100.f;
	HUD.setFillColor(HUDcolor);

	controlsDisplay.setSize(sf::Vector2f(400.f, 400.f));
	sf::Color controlsDisplayColor = sf::Color::Blue;
	controlsDisplayColor.a = 200.f;
	controlsDisplay.setFillColor(controlsDisplayColor);
	controlsDisplay.setPosition(window.getSize().x/2 - controlsDisplay.getSize().x/2, window.getSize().y/2 - controlsDisplay.getSize().y/2);

	glowCentre.setRadius(3.f);
	glowCentre.setFillColor(sf::Color(255,255,255,255));
	glowCentre.setOrigin(glowCentre.getRadius(),glowCentre.getRadius());
	circleOne.setRadius(5.f);
	circleOne.setFillColor(sf::Color(255, 255, 255, 150));
	circleOne.setOrigin(circleOne.getRadius(), circleOne.getRadius());
	circleTwo.setRadius(8.f);
	circleTwo.setFillColor(sf::Color(255, 255, 255, 50));
	circleTwo.setOrigin(circleTwo.getRadius(), circleTwo.getRadius());
	circleThree.setRadius(12.f);
	circleThree.setFillColor(sf::Color(255, 255, 255, 40));
	circleThree.setOrigin(circleThree.getRadius(), circleThree.getRadius());
	circleFour.setRadius(25.f);
	circleFour.setFillColor(sf::Color(255, 255, 255, 30));
	circleFour.setOrigin(circleFour.getRadius(), circleFour.getRadius());
	outerCircle.setRadius(50.f);
	outerCircle.setFillColor(sf::Color(255, 255, 255, 20));
	outerCircle.setOrigin(outerCircle.getRadius(), outerCircle.getRadius());

	window.setMouseCursorVisible(false);

	stateText.addDetails("Mode: Magical","resources/arial.ttf", 15 , sf::Color::White,sf::Vector2f(10., 10.));
	radiusText.addDetails("Radius: ", "resources/arial.ttf", 15, sf::Color::White, sf::Vector2f(10., 30.));
	gravityText.addDetails("Gravity: " + to_string((int)gravity), "resources/arial.ttf", 15, sf::Color::White, sf::Vector2f(10., 50.));
	sizeText.addDetails("Particles: 0", "resources/arial.ttf", 15, sf::Color::White, sf::Vector2f(10., 90.));
	FPStext.addDetails("FPS: ", "resources/arial.ttf", 15, sf::Color::White, sf::Vector2f(10., 110.));
	spawnCountText.addDetails("Spawn Count: 100 ", "resources/arial.ttf", 15, sf::Color::White, sf::Vector2f(10., 130.));
	spawnHelpText.addDetails("Increase Spawn-Rate: Up \nDecrease Spawn-Rate: Down ", "resources/arial.ttf", 20, sf::Color::White, sf::Vector2f(controlsDisplay.getPosition().x + 20., controlsDisplay.getPosition().y + 160.));
	modeHelpText.addDetails("....Modes.... \nMagical: 1\nFree Fall: 2\nConstrained: 3\nWave: 4", "resources/arial.ttf", 20, sf::Color::White, sf::Vector2f(controlsDisplay.getPosition().x + 20., controlsDisplay.getPosition().y + 20.));
	clearHelpText.addDetails("Clear Particles: C","resources/arial.ttf", 20, sf::Color::White, sf::Vector2f(controlsDisplay.getPosition().x + 20., controlsDisplay.getPosition().y + 210.));
	controlsHelpText.addDetails("Emit Particles: LCtrl", "resources/arial.ttf", 20, sf::Color::White, sf::Vector2f(controlsDisplay.getPosition().x + 20., controlsDisplay.getPosition().y + 240.));
	helpText.addDetails("Toggle Help: H", "resources/arial.ttf", 20, sf::Color::White, sf::Vector2f(controlsDisplay.getPosition().x + 20., controlsDisplay.getPosition().y + 270.));
	gravityHelpText.addDetails("Toggle Gravity: G", "resources/arial.ttf", 20, sf::Color::White, sf::Vector2f(controlsDisplay.getPosition().x + 20., controlsDisplay.getPosition().y + 300.));
}

void Game::handleEvents() {

	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {

			isEmitting = true;

			if (type != particleType::freeFall) {
				for (size_t i = 0; i < spawnCount; i++) {
					Particle particleOBJ(radiusText, type);
					sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					particleOBJ.particleShape.setPosition(mousepos);
					particleVector.emplace_back(particleOBJ);
				}
			}
			else {
					Particle particleOBJ(radiusText,type);
					sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					particleOBJ.particleShape.setPosition(mousepos);
					particleVector.emplace_back(particleOBJ);
				
			}

			sizeText.toString("Particles: " + to_string(particleVector.size()));
					
		}
		else {
			isEmitting = false;

		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
			particleVector.clear();
			sizeText.toString("Particles: " + to_string(particleVector.size()));
		}

		if (type == particleType::freeFall) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
				gravity += 10;
				gravityText.toString("Gravity(^): " + to_string((int)gravity));
				cout << "Gravity: " << gravity << " px/s*s" << endl;
			}
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) {
			type = particleType::magical;
			stateText.toString("Mode: Magical");
			particleVector.clear();
			sizeText.toString("Particles: " + to_string(particleVector.size()));
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) {
			type = particleType::freeFall;
			stateText.toString("Mode: Free Fall");
			particleVector.clear();
			sizeText.toString("Particles: " + to_string(particleVector.size()));
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3) {
			type = particleType::constrained;
			stateText.toString("Mode: Constrained");
			particleVector.clear();
			sizeText.toString("Particles: " + to_string(particleVector.size()));
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num4) {
			type = particleType::wave;
			stateText.toString("Mode: Wave");
			particleVector.clear();
			sizeText.toString("Particles: " + to_string(particleVector.size()));
		}

		if (type != particleType::freeFall) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
				if (spawnCount <= 90 && spawnCount >= 10) spawnCount += 10;
				else if (spawnCount < 10) spawnCount = 10;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
				if (spawnCount > 10 && spawnCount <= 100) spawnCount -= 10;
				else if (spawnCount <= 10) spawnCount = 1;
			}
			spawnCountText.toString("Spawn Count: " + to_string(spawnCount));
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H) {
			if (!showControls) showControls = true;
			else showControls = false;
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
	outerCircle.setPosition(mousepos);

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

	if (type == particleType::constrained) {
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
	//else glowCentre.setFillColor(sf::Color::White);

}

void Game::render() {

	window.clear();

	for (auto& particle : particleVector) {
		if(particle.draw) window.draw(particle.particleShape);
	}

	if (type == particleType::freeFall) {
		window.draw(gravityText.getText());
	}

	if (showControls) {
		window.draw(controlsDisplay);
		window.draw(spawnHelpText.getText());
		window.draw(modeHelpText.getText());
		window.draw(clearHelpText.getText());
		window.draw(controlsHelpText.getText());
		window.draw(helpText.getText());
		window.draw(gravityHelpText.getText());
	}
		
		window.draw(HUD);
		window.draw(spawnCountText.getText());
		window.draw(FPStext.getText());
		window.draw(sizeText.getText());
		window.draw(stateText.getText());
		window.draw(radiusText.getText());
		window.draw(outerCircle);
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
		FPS = 1.f / deltaTime;
		FPStext.toString("FPS: " + to_string(FPS));

		update();
		handleEvents();
		render();
	}

}

int main() {

	Game game;
	game.run();

}