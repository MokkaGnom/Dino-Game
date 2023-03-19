#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>
using namespace sf;

int random(int);

int main()
{
	RenderWindow window(VideoMode(900, 400), "Dino-Game");
	window.setFramerateLimit(60);

	SoundBuffer bufferDinoJumpSound;
	bufferDinoJumpSound.loadFromFile("sounds/jump_short.wav");
	Sound dinoJumpSound;
	dinoJumpSound.setBuffer(bufferDinoJumpSound);


	Texture dinoTexture[4], kaktusETx, kaktusGTx;
	dinoTexture[0].loadFromFile("img/charaktere/dino_normal.png"); //X 49; Y 53
	dinoTexture[1].loadFromFile("img/charaktere/dino_sword.png");
	dinoTexture[2].loadFromFile("img/charaktere/dino_shuriken.png");
	dinoTexture[3].loadFromFile("img/charaktere/dino_heli.png");
	kaktusETx.loadFromFile("img/kaktusEinzeln.png"); //X 15; Y 33
	kaktusGTx.loadFromFile("img/kaktusGruppe.png"); //X 73; Y 47

	Sprite auswahlDinoNormal, auswahlDinoHeli, auswahlDinoSword, auswahlDinoShuriken;
	auswahlDinoNormal.setTexture(dinoTexture[0]);
	auswahlDinoSword.setTexture(dinoTexture[1]);
	auswahlDinoShuriken.setTexture(dinoTexture[2]);
	auswahlDinoHeli.setTexture(dinoTexture[3]);

	auswahlDinoNormal.setPosition(50, 240);
	auswahlDinoSword.setPosition(250, 240);
	auswahlDinoShuriken.setPosition(450, 240);
	auswahlDinoHeli.setPosition(650, 240);

	Sprite dinoSprite, kaktusESpr, kaktusGSpr, kaktusESpr2, kaktusGSpr2;
	kaktusESpr.setTexture(kaktusETx);
	kaktusGSpr.setTexture(kaktusGTx);
	kaktusESpr2.setTexture(kaktusETx);
	kaktusGSpr2.setTexture(kaktusGTx);

	dinoSprite.setPosition(100, 300); //351-51=300
	kaktusESpr.setPosition(900, 318); //351-33=318
	kaktusGSpr.setPosition(900, 302); //351-47=302
	kaktusESpr2.setPosition(1350, 318);
	kaktusGSpr2.setPosition(1350, 302);

	int dinoJump = 0, dinoJumpCoolDown = 2, dinoJumpHeight = 50, score = 0, kaktusSpeed = 10, kaktusDist = 150;
	int kaktusMoveE = 150, kaktusMoveG = 150, kaktusMoveE2 = 150, kaktusMoveG2 = 150;
	int auswahl = 0;
	bool dinoIsJumping = 0, dinoJumpDown = 0, verloren = 0, ausgewaehlt = 0;

	RectangleShape auswahlZeiger(Vector2f(100, 10));
	auswahlZeiger.setFillColor(Color::Red);
	auswahlZeiger.setPosition(50, 300);

	Font font;
	font.loadFromFile("fonts/arial.ttf");

	Text text_score("", font), text_scoreEnd("", font);
	Text text_verloren("VERLOREN!", font), text_neustart("Drücke 'Enter' zum Neustarten oder 'Esc' zum beenden", font);
	Text text_auswahl("Wähle deinen Dino mit 'Enter' aus", font);
	text_score.setCharacterSize(15);
	text_verloren.setCharacterSize(35);
	text_neustart.setCharacterSize(20);
	text_scoreEnd.setCharacterSize(20);
	text_auswahl.setCharacterSize(20);

	text_verloren.setPosition(350, 100);
	text_neustart.setPosition(200, 150);
	text_scoreEnd.setPosition(370, 200);
	text_auswahl.setPosition(300, 350);

	while (window.isOpen())
	{
		//Fenster schließen, wenn das X oben rechts gedrückt wird
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		//Auswahl des Dinos
		if (!ausgewaehlt)
		{
			if (!verloren)
			{
				window.clear();
				window.draw(text_auswahl);
				window.draw(auswahlZeiger);
				window.draw(auswahlDinoNormal);
				window.draw(auswahlDinoHeli);
				window.draw(auswahlDinoSword);
				window.draw(auswahlDinoShuriken);
				window.display();
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				auswahl++;
				if (auswahl > 3)
					auswahl = 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				auswahl--;
				if (auswahl < 0)
					auswahl = 0;
			}
			switch (auswahl)
			{
			case 0: auswahlZeiger.setPosition(50, 300);
				break;
			case 1: auswahlZeiger.setPosition(250, 300);
				break;
			case 2: auswahlZeiger.setPosition(450, 300);
				break;
			case 3: auswahlZeiger.setPosition(650, 300);
				break;
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				dinoSprite.setTexture(dinoTexture[auswahl]);
				kaktusSpeed = auswahl * 2 + 9;
				ausgewaehlt = 1;
			}
		}
		else
		{
			//Score
			text_score.setString("Score: " + std::to_string(score / 4));
			score++;

			//Dino Sprung
			if (((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space)) && !dinoIsJumping) && !dinoJumpDown)
			{
				dinoSprite.move(Vector2f(0, -dinoJumpHeight));
				dinoIsJumping = 1;
				if (!verloren)
					dinoJumpSound.play();
			}
			else if (dinoIsJumping)
				dinoJump++;

			if ((dinoJump > 40 || Keyboard::isKeyPressed(Keyboard::Down)) && dinoIsJumping)
			{
				dinoSprite.move(Vector2f(0, dinoJumpHeight));
				dinoIsJumping = 0;
				dinoJump = 0;
				dinoJumpDown = 1;
			}

			if (dinoJumpDown)
				dinoJumpCoolDown--;
			if (dinoJumpCoolDown <= 0)
			{
				dinoJumpCoolDown = 10;
				dinoJumpDown = 0;
			}

			//Kakteen bewegen
			Vector2f PosKaE = kaktusESpr.getPosition();
			Vector2f PosKaG = kaktusGSpr.getPosition();
			Vector2f PosKaE2 = kaktusESpr2.getPosition();
			Vector2f PosKaG2 = kaktusGSpr2.getPosition();

			kaktusESpr.move(-kaktusSpeed, 0);
			kaktusGSpr.move(-kaktusSpeed, 0);
			kaktusESpr2.move(-kaktusSpeed, 0);
			kaktusGSpr2.move(-kaktusSpeed, 0);

			if (PosKaE.x <= -15)
			{
				kaktusMoveE = random(kaktusDist);
				if (kaktusMoveE > 1000)
					kaktusMoveE %= 1000;
				if (kaktusMoveE < 150)
					kaktusMoveE + 150;
				//std::cout << "E1: " << kaktusMoveE << "\n";
				kaktusESpr.setPosition(900 + kaktusMoveE + kaktusDist, 318);
			}
			if (PosKaG.x <= -73)
			{
				kaktusMoveG = random(kaktusMoveE);
				if (kaktusMoveG > 1000)
					kaktusMoveG %= 1000;
				if (kaktusMoveG < 150)
					kaktusMoveG + 150;
				//std::cout << "G1: " << kaktusMoveG << "\n";
				kaktusGSpr.setPosition(900 + kaktusMoveG + kaktusDist, 303);
			}
			if (PosKaE2.x <= -15)
			{
				kaktusMoveE2 = random(kaktusMoveG);
				if (kaktusMoveE2 > 1000)
					kaktusMoveE2 %= 1000;
				if (kaktusMoveE2 < 150)
					kaktusMoveE2 + 150;
				//std::cout << "E2: " << kaktusMoveE2 << "\n";
				kaktusESpr2.setPosition(900 + kaktusMoveE2 + kaktusDist, 318);
			}
			if (PosKaG2.x <= -73)
			{
				kaktusMoveG2 = random(kaktusMoveE2);
				if (kaktusMoveG2 > 1000)
					kaktusMoveG2 %= 1000;
				if (kaktusMoveG2 < 150)
					kaktusMoveG2 + 150;
				//std::cout << "G2: " << kaktusMoveG2 << "\n";
				kaktusGSpr2.setPosition(900 + kaktusMoveG2 + kaktusDist, 303);
			}

			//Verloren
			if (((PosKaE.x < 100 && PosKaE.x + 15 > 100) || (PosKaG.x < 100 && PosKaG.x + 73 > 100) ||
				(PosKaE2.x < 100 && PosKaE2.x + 15 > 100) || (PosKaG2.x < 100 && PosKaG2.x + 73 > 100)) && !dinoIsJumping)
			{
				verloren = 1;
				kaktusSpeed = 0;
				kaktusESpr.setPosition(200, 200);
				kaktusGSpr.setPosition(200, 200);
				kaktusESpr2.setPosition(200, 200);
				kaktusGSpr2.setPosition(200, 200);
				text_scoreEnd.setString("Dein " + text_score.getString());
			}

			if (verloren && Keyboard::isKeyPressed(Keyboard::Escape))
			{
				return 0;
			}
			if (verloren && Keyboard::isKeyPressed(Keyboard::Enter))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				verloren = 0;
				kaktusSpeed = 10;
				score = 0;
				ausgewaehlt = 0;
				kaktusESpr.setPosition(900, 318);
				kaktusGSpr.setPosition(900, 302);
				kaktusESpr2.setPosition(1350, 318);
				kaktusGSpr2.setPosition(1350, 302);
			}

			//Fenster Ausgabe
			window.clear();
			if (!verloren)
			{
				window.draw(dinoSprite);
				window.draw(kaktusESpr);
				window.draw(kaktusGSpr);
				window.draw(kaktusESpr2);
				window.draw(kaktusGSpr2);
				window.draw(text_score);
			}
			else
			{
				window.draw(text_verloren);
				window.draw(text_neustart);
				window.draw(text_scoreEnd);
				//verloren = 0;
			}
			window.display();

		}
	}

	return 0;
}

int random(int min)
{
	srand((unsigned)time(NULL));
	return (rand() % 50 + (min / 10)) * 10;
}