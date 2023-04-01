#include <SFML/Graphics.hpp>

using namespace sf;

int WinMain()
{
    VideoMode vm(960, 480);
    RenderWindow window(vm, "Timber", Style::Titlebar);

    window.setVerticalSyncEnabled(true);

    //background
    //create a texture to hold a graphic on the GPU
    Texture texturebackground;
    //load a graphic into the texture
    texturebackground.loadFromFile("graphics/background.png");
    //create a sprite
    Sprite spriteBackground;
    //attach the texture to the sprite
    spriteBackground.setTexture(texturebackground);
    //set the spriteBackground to cover the screen
    spriteBackground.setPosition(0, 0);

    while (window.isOpen())
    {
        /*
        ************************************************************
        Handle the players input
        ************************************************************
        */

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        /*
        ************************************************************
        Update the scene
        ************************************************************
        */

        /*
        ************************************************************
        Draw the scene
        ************************************************************
        */

        //clear everything from the last frame
        window.clear(Color::Blue);

        //draw our game scene here
        window.draw(spriteBackground);
        
        //show everything we just drew
        window.display();
    }
    return 0;
}