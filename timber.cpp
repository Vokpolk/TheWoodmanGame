#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "timber.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//where is the player/branch?
enum class side {
	LEFT,
	RIGHT,
	NONE
};
side branchPositions[NUM_BRANCHES];

int main()
{
	VideoMode vm(960, 480);
    RenderWindow window(vm, "Timber");

    window.setVerticalSyncEnabled(true);
	
    //background
	std::string dir = "";
    //create a texture to hold a graphic on the GPU
    Texture textureBackground;
    //load a graphic into the texture
    if (!textureBackground.loadFromFile(dir + "graphics/background.png")) {
		return EXIT_FAILURE;
	}
    //create a sprite
    Sprite spriteBackground;
    //attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);
    //set the spriteBackground to cover the screen
    spriteBackground.setPosition(0, 0);

	//tree
	Texture textureTree;
	if (!textureTree.loadFromFile(dir + "graphics/tree.png")) {
		return EXIT_FAILURE;
	}
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(400, -100);

	//bird
	Texture textureBird;
	Texture textureBird2;
	if (!textureBird.loadFromFile(dir + "graphics/bird.png")) {
		std::cout << "FAIL" << std::endl;
		return EXIT_FAILURE;
	}
	if (!textureBird2.loadFromFile(dir + "graphics/bird2.png")) {
		std::cout << "FAIL" << std::endl;
		return EXIT_FAILURE;
	}
	Sprite spriteBird;
	spriteBird.setTexture(textureBird);
	spriteBird.setPosition(-240, 120);
	Sprite spriteBird2;
	spriteBird2.setTexture(textureBird2);
	spriteBird2.setPosition(-240, 120);
	//is the bird moving
	bool birdActive = false;
	//how fast can the bird fly
	float birdSpeed = 0.0f;
	//wing flap
	int wingDelay = 0;
	bool wingFlap = false;

	//clouds
	Texture textureCloud;
	if (!textureCloud.loadFromFile(dir + "graphics/cloud.png")) {
		return EXIT_FAILURE;
	}
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(-250, -100);
	spriteCloud2.setPosition(-250, -50);
	spriteCloud3.setPosition(-250, -30);
	//are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	//how fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	
	//control time
	Clock clock;
	
	//timebar
	RectangleShape timeBar;
	float timeBarStartWidth = 100;
	float timeBarHeight = 15;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::White);
	timeBar.setPosition(890.f - timeBarStartWidth / 2, 445.f);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timebarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//track whether the game is running
	bool paused = true;
	
	//draw some text
	int score = 0;
	Text messageText;
	Text scoreText;
	//chosing a font
	Font font;
	font.loadFromFile(dir + "fonts/consola.ttf");
	//set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);
	//assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	//text size
	messageText.setCharacterSize(50);
	scoreText.setCharacterSize(30);
	//chose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	//text position
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f,
						  textRect.top + textRect.height / 2.0f);
	messageText.setPosition(960 / 2.0f, 480 / 2.0f);
	scoreText.setPosition(20, 430);

	//prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	//set the texture for 6 branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		//set the sprite's origin to dead centre
		//we can then spin it round without changing its position
		branches[i].setOrigin(80, 80);
	}
	
	//updateBranches(0);
	//updateBranches(1);
	//updateBranches(2);
	//updateBranches(3);
	//updateBranches(4);
	//updateBranches(5);

	//prepare the player
	Texture texturePlayerDefault;
	texturePlayerDefault.loadFromFile(dir + "graphics/hero_default.png");
	Sprite spritePlayerDefault;
	spritePlayerDefault.setTexture(texturePlayerDefault);
	spritePlayerDefault.setPosition(300, 250);
	Texture texturePlayerHits;
	texturePlayerHits.loadFromFile(dir + "graphics/hero_hits.png");
	Sprite spritePlayerHits;
	spritePlayerHits.setTexture(texturePlayerHits);
	spritePlayerHits.setPosition(300, 250);
	
	//the player starts on the left
	side playerSide = side::LEFT;

	//prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(370, 280);

	//prepare the flying log;
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(400, 300);

	//same other useful log related variables
	bool logActive = false;
	float logSpeed = 1000;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//control the player input
	bool acceptInput = false;
	
	//player punch
	bool playerPunch = false;

	//prerpare sounds
	//the player chopping sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile(dir + "sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	//the playe's death sound
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile(dir + "sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	//out of time sound
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile(dir + "sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen())
    {
        /*
        ************************************************************
        Handle the players input
        ************************************************************
        */
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
	
			if (event.type == Event::KeyReleased && !paused) {
				//listen for key presses again
				acceptInput = true;
				//
				playerPunch = false;
			}
		}


		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
	
		//start the game
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;
			playerPunch = false;
			//reset the time and the score
			score = 0;
			timeRemaining = 6;

			//make all branches disappear -
			//starting in the second position
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			//make sure the gravestone is hidden
			spriteRIP.setPosition(370, 2000);

			//move the player into position
			spritePlayerDefault.setScale(1, 1);
			spritePlayerHits.setScale(1, 1);
			spritePlayerDefault.setPosition(300, 250);
			spritePlayerHits.setPosition(300, 250);

			acceptInput = true;
		}
		
		//wrap the player controls to make sure we are accepting input
		if (acceptInput) {
			//firts handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				playerSide = side::RIGHT;
				score++;

				//add to the amount of time remaining
				timeRemaining += .15;

				spritePlayerDefault.setPosition(660, 250);
				spritePlayerHits.setPosition(660, 250);
				spritePlayerDefault.setScale(-1, 1);
				spritePlayerHits.setScale(-1, 1);

				playerPunch = true;
				//update branches
				updateBranches(score);
				//set the log flying to the left
				spriteLog.setPosition(400, 300);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				//play a chop sound
				chop.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				playerSide = side::LEFT;
				score++;

				timeRemaining += .15;

				spritePlayerDefault.setPosition(300, 250);
				spritePlayerHits.setPosition(300, 250);
				spritePlayerDefault.setScale(1, 1);
				spritePlayerHits.setScale(1, 1);

				playerPunch = true;

				updateBranches(score);
				spriteLog.setPosition(400, 300);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				
				//play a chop sound
				chop.play();
			}
		}

        /*
        ************************************************************
        Update the scene
        ************************************************************
        */
		if (!paused) {	
			//measure time
			Time dt = clock.restart();
			//time remaining
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timebarWidthPerSecond * timeRemaining,
				timeBarHeight));

			if (timeRemaining <= 0.0f) {
				//pause the game
				paused = true;

				//new message
				messageText.setString("Out of time!");

				//new pos for text
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(960 / 2.0f, 480 / 2.0f);
				
				//play the out of time sound
				outOfTime.play();
			}

			//setup the bird
			if (!birdActive) {
				//how fast is the bird
				srand(int(time(0)));
				birdSpeed = (rand() % 100) + 50;
				//how high is the bird
				srand(int(time(0)) * 10);
				float height = (rand() % 100) + 100;
				spriteBird.setPosition(-200, height);
				spriteBird2.setPosition(-200, height);
				birdActive = true;
			} else { //move the bird
				spriteBird.setPosition(spriteBird.getPosition().x + (birdSpeed * dt.asSeconds()),
						spriteBird.getPosition().y);
				spriteBird2.setPosition(spriteBird2.getPosition().x + (birdSpeed * dt.asSeconds()),
					spriteBird2.getPosition().y);

				if (wingDelay <= 10) {
					++wingDelay;
				} else {
					wingDelay = 0;
					wingFlap = !wingFlap;
				}

				//has the bird reached the left-hand edge of the screen?
				if (spriteBird.getPosition().x > 1000) {
					birdActive = false;
				}
			}
	
			//manage the clouds
			//cloud 1
			if (!cloud1Active) {
				//how fast is the cloud
				srand(int(time(0)) * 10);
				cloud1Speed = (rand() % 50);
				//how high is the cloud
				srand(int(time(0)) * 10);
				float height = (rand() % 100 - 100);
				spriteCloud1.setPosition(-240, height);
				cloud1Active = true;
			} else {
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
					   spriteCloud1.getPosition().y);
				//has the cloud reached the right hand edge of the screen?
				if (spriteCloud1.getPosition().x > 960) {
					cloud1Active = false;
				}
			}
			//cloud 2
			if (!cloud2Active) {
				srand(int(time(0)) * 20);
				cloud2Speed = (rand() % 50);
				srand(int(time(0)) * 20);
				float height = (rand() % 100 - 100);
				spriteCloud2.setPosition(-240, height);
				cloud2Active = true;
			} else {
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
						spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 960) {
					cloud2Active = false;
				}
			}
			//cloud 3
			if (!cloud3Active) {
				srand(int(time(0)) * 30);
				cloud3Speed = (rand() % 50);
				srand(int(time(0)) * 30);
				float height = (rand() % 100 - 100);
				spriteCloud3.setPosition(-240, height);
				cloud3Active = true;
			} else {
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
						spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 960) {
					cloud3Active = false;
				}
			}

			//update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			//update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 80 - 40;
				if (branchPositions[i] == side::LEFT) {
					//move sprite to the left side
					branches[i].setPosition(380, height);
					//flip sprite round the other way
					branches[i].setRotation(180);
				} else if (branchPositions[i] == side::RIGHT) {
					//move sprite to the right side
					branches[i].setPosition(580, height);
					//set sprite rotation to normal
					branches[i].setRotation(0);
				} else {
					//hide the branch
					branches[i].setPosition(2000, height);
				}
			}

			//handle a flying log
			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
									  spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				//has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 ||
						spriteLog.getPosition().x > 1000) {
					logActive = false;
					spriteLog.setPosition(400, 300);
				}
			}

			//has the player been squished by a branch?
			if (branchPositions[5] == playerSide) {
				//death
				paused = true;
				acceptInput = false;

				//draw the gravestone
				spriteRIP.setPosition(370, 280);
					
				//draw the log
				spriteLog.setPosition(400, 300);

				//hide the player
				spritePlayerDefault.setPosition(1200, 250);
				spritePlayerHits.setPosition(1200, 250);

				//change the text of the message
				messageText.setString("SQUISHED!");

				//center it to the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
						textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);

				messageText.setPosition(960 / 2.0f, 480 / 2.0f);
				
				//play the death sound
				death.play();
			}

		} //end if (!paused)
        /*
        ************************************************************
        Draw the scene
        ************************************************************
        */

        //clear everything from the last frame
        window.clear();

        //draw our game scene here
        window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}
		window.draw(spriteTree);
		window.draw(spriteLog);
		if (playerPunch)
			window.draw(spritePlayerHits);
		else
			window.draw(spritePlayerDefault);
		window.draw(spriteRIP);

		if (wingFlap)
			window.draw(spriteBird);
		else
			window.draw(spriteBird2);
		
		//draw the score
		window.draw(scoreText);
		//draw the time bar
		window.draw(timeBar);
		if (paused) {
			//draw our message
			window.draw(messageText);
		}

        //show everything we just drew
        window.display();
    }
    return 0;
}

//
void updateBranches(int seed) {
	//move all the branches down one place
	for (int i = NUM_BRANCHES - 1; i > 0; i--) {
		branchPositions[i] = branchPositions[i - 1];
	}

	//spawn a new branch at 0 position
	//LEFT, RIGHT or NONE
	srand(int(time(0)) + seed);
	int r = (rand() % 5);
	switch (r) {
		case 0:
			branchPositions[0] = side::LEFT;
			break;
		case 1:
			branchPositions[0] = side::RIGHT;
			break;
		default:
			branchPositions[0] = side::NONE;
			break;
	}
}
