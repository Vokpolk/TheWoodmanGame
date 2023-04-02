#include <SFML/Graphics.hpp>
#include "timber.h"
#include <iostream>
#include <string>
using namespace sf;

int main()
{
    VideoMode vm(960, 480);
    RenderWindow window(vm, "Timber");

    window.setVerticalSyncEnabled(true);
	
    //background
	std::string dir = "/Users/vokpolk/Documents/Programming/C++/TheWoodmanGame/";
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
	spriteTree.setPosition(400, 0);

	//bird
	Texture textureBird;
	if (!textureBird.loadFromFile(dir + "graphics/bird.png")) {
		std::cout << "FAIL" << std::endl;
		return EXIT_FAILURE;
	}
	Sprite spriteBird;
	spriteBird.setTexture(textureBird);
	spriteBird.setPosition(0, 120);
	//is the bird moving
	bool birdActive = false;
	//how fast can the bird fly
	float birdSpeed = 0.0f;

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
	spriteCloud1.setPosition(0, -100);
	spriteCloud2.setPosition(0, -50);
	spriteCloud3.setPosition(0, -30);
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

	while (window.isOpen())
    {
        /*
        ************************************************************
        Handle the players input
        ************************************************************
        */

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        /*
        ************************************************************
        Update the scene
        ************************************************************
        */
		
		//measure time
		Time dt = clock.restart();
		
		//setup the bird
		if (!birdActive) {
			//how fast is the bird
			srand(int(time(0)));
			birdSpeed = (rand() % 100) + 50;
			//how high is the bird
			srand(int(time(0)) * 10);
			float height = (rand() % 100) + 100;
			spriteBird.setPosition(-200, height);
			birdActive = true;
		} else { //move the bird
			spriteBird.setPosition(spriteBird.getPosition().x + (birdSpeed * dt.asSeconds()),
					spriteBird.getPosition().y);
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
		window.draw(spriteTree);
		window.draw(spriteBird);

        //show everything we just drew
        window.display();
    }
    return 0;
}
