#include <SFML/Graphics.hpp>
#include <iostream>
#include "add.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "Algorithmic Life", sf::Style::Titlebar | sf::Style::Close);
    sf::Event evnt;

    while (window.isOpen())
    {
        //Event polling
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (evnt.key.code == sf::Keyboard::Escape)
                        window.close();
                    break;
                default:
                    break;
            }
        }

        //Update
        
        //Render
        window.clear(); //Clear old frame

        //Draw my game

        window.display(); //Tell app that window is done drawing

    }
}
