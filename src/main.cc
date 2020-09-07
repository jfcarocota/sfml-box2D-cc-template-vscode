#include<iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#define WIDTH 1600
#define HEIGHT 1200
#define GAME_NAME "First SFML Game"
#define FPS 60
#define GRAVITY 9.8f
#define SCALE 30.f

enum GameState
{
    PLAY,
    WAITING
};

void CreateGround(b2World&, float, float);
void CreateBox(b2World&, int, int);

/*int main()
{
    //sf significa que viene de la clase de SFML, sf es como se abrevia.
    sf::RenderWindow* window{new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), GAME_NAME)};

    GameState gameState{PLAY};

    window->setFramerateLimit(FPS);

    b2Vec2* gravity{new b2Vec2(0.f, GRAVITY)};
    b2World* world{new b2World(*gravity)};

    sf::Texture boxTexture;
    sf::Texture groundTexture;
    groundTexture.loadFromFile("assets/sprites/ground.png");
    boxTexture.loadFromFile("assets/sprites/box.png");

    //window->setKeyRepeatEnabled(false);

    while (gameState == PLAY)
    {

        sf::Event e;

        while (window->pollEvent(e))
        {

            if(e.type == sf::Event::KeyPressed)
            {
                std::cout << "Key: " << e.key.code << std::endl;
            }

            if(e.type == sf::Event::KeyReleased)
            {
                std::cout << "Key: " << e.key.code << " was released" << std::endl;
            }

            gameState = e.type == sf::Event::Closed ? WAITING : gameState;
        }
        
        
        window->clear();
        window->display();
    }
    
    window->close();

    return 0;
}*/

int main()
{

    sf::RenderWindow* window{new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, 32), GAME_NAME)};
    window->setKeyRepeatEnabled(false);
    window->setFramerateLimit(FPS);
    b2Vec2* gravity{new b2Vec2(0.f, GRAVITY)};
    b2World* world{new b2World(*gravity)};

    sf::Texture boxTexture;
    sf::Texture groundTexture;
    groundTexture.loadFromFile("assets/sprites/ground.png");
    boxTexture.loadFromFile("assets/sprites/box.png");

    while (window->isOpen())
    {
        /** When the user left-mouse-click, add a box into the world */
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int MouseX = sf::Mouse::getPosition(*window).x;
            int MouseY = sf::Mouse::getPosition(*window).y;
            CreateBox(*world, MouseX, MouseY);
        }

        /** Simulate the world */
        world->Step(1/60.f, 8, 3);

        window->clear(sf::Color::White);
        for (b2Body* BodyIterator = world->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
        {
            if (BodyIterator->GetType() == b2_dynamicBody)
            {
                sf::Sprite Sprite;
                Sprite.setTexture(boxTexture);
                Sprite.setOrigin(16.f, 16.f);
                Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
                Sprite.setRotation(BodyIterator->GetAngle() * 180/b2_pi);
                window->draw(Sprite);
            }
            else
            {
                sf::Sprite GroundSprite;
                GroundSprite.setTexture(groundTexture);
                GroundSprite.setOrigin(400.f, 8.f);
                GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
                GroundSprite.setRotation(180/b2_pi * BodyIterator->GetAngle());
                window->draw(GroundSprite);
            }
        }
        window->display();
    }

    return 0;
}

void CreateGround(b2World& World, float X, float Y)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(X/30.f, Y/30.f);
    BodyDef.type = b2_staticBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((800.f/2)/SCALE, (16.f/2)/SCALE); // Creates a box shape. Divide your desired width and height by 2.
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;  // Sets the density of the body
    FixtureDef.shape = &Shape; // Sets the shape
    Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}

void CreateBox(b2World& World, int MouseX, int MouseY)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(MouseX/SCALE, MouseY/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}
