#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include "ant.h"

Vector world_to_screen_pos(Vector v)
{
    Vector ratio = v/vec_v(W_SIZE_W, W_SIZE_H);
    return ratio * vec_v(SCREEN_W, SCREEN_H);
}

Vector grid_to_screen_pos(Vector pos)
{
    return pos/vec_v(WALL_GRID_W,WALL_GRID_H)*vec_v(SCREEN_W,SCREEN_H);
}

Vector screen_to_grid_pos(Vector pos)
{
    pos = vec_v(clamp(pos.x, 0, SCREEN_W), clamp(pos.y, 0, SCREEN_H));
    return pos/vec_v(SCREEN_W,SCREEN_H)*vec_v(WALL_GRID_W-2,WALL_GRID_H-2);
}

Vector screen_to_world_pos(Vector pos)
{
    return pos/vec_v(SCREEN_W,SCREEN_H)*vec_v(W_SIZE_W, W_SIZE_H);
}

inline sf::Sprite create_sprite(sf::Texture* tex,std::string path)
{
    tex->loadFromFile(path);
    sf::Sprite spr;
    spr.setTexture(*tex);
    sf::Vector2f sprSize(spr.getTexture()->getSize().x * spr.getScale().x,spr.getTexture()->getSize().y * spr.getScale().y);
    spr.setOrigin(sprSize/2.f);
    spr.setScale(sf::Vector2f(0.05, 0.05));
    return spr;
}

void centerText(sf::Text* t_)
{
    sf::FloatRect textRect = t_->getLocalBounds();
    t_->setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top  + textRect.height/2.0f);
}

int main()
{
	srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, 32), "Main", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    Vector screenSize = vec_v(SCREEN_W, SCREEN_H);
    Vector worldSize = vec_v(W_SIZE_W, W_SIZE_H);
    Vector gridSize = vec_v(WALL_GRID_W, WALL_GRID_H);
    AntManager antMan(1, 300);

    sf::Texture antTex;
    sf::Sprite antSpr = create_sprite(&antTex, "res/ant.png");
    antSpr.scale(sf::Vector2f(ANT_SIZE/worldSize.x * screenSize.x * 0.1f, ANT_SIZE/worldSize.x * screenSize.x * 0.1f));
    
    sf::Texture nestTex;
    sf::Sprite nestSpr = create_sprite(&nestTex, "res/nest.png");
    nestSpr.scale(sf::Vector2f(NEST_SIZE/worldSize.x * screenSize.x * 0.1f, NEST_SIZE/worldSize.x * screenSize.x * 0.1f));

    sf::Texture foodTex;
    sf::Sprite foodSpr = create_sprite(&foodTex, "res/food.png");
    foodSpr.scale(sf::Vector2f(FOOD_SIZE/worldSize.x * screenSize.x * 0.1f, FOOD_SIZE/worldSize.x * screenSize.x * 0.1f));

    sf::Texture antFoodTex;
    sf::Sprite antFoodSpr = create_sprite(&antFoodTex, "res/ant_with_food.png");
    antFoodSpr.scale(sf::Vector2f(ANT_SIZE/worldSize.x * screenSize.x * 0.1f, ANT_SIZE/worldSize.x * screenSize.x * 0.1f));

    sf::Texture bgTex;
    bgTex.loadFromFile("res/bg.png");
    sf::Sprite bgSpr;
    bgSpr.setTexture(bgTex);
    bgSpr.setScale((float)SCREEN_W/bgSpr.getTexture()->getSize().x, (float)SCREEN_H/bgSpr.getTexture()->getSize().y);

    // bgSpr.setScale
    // bgSpr.setPosition(vec_to_sfvec2_v(screenSize/2));
    // bgSpr.scale(sf::Vector2f(ANT_SIZE/worldSize.x * screenSize.x * 0.1f, ANT_SIZE/worldSize.x * screenSize.x * 0.1f));

    int tick = 0;
    bool isPaused = true;
    bool hasStarted = false;
    sf::Font font;
    font.loadFromFile("res/GoogleSans-Bold.ttf");
    sf::Text text;
    text.setFont(font);

    while(window.isOpen())
    {
    	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                Vector mouseGPos = screen_to_grid_pos(sfvec2_to_vec_v(mousePos));
                antMan.walls[(int)mouseGPos.x][(int)mouseGPos.y] = 1;
                antMan.walls[(int)mouseGPos.x+1][(int)mouseGPos.y] = 1;
                antMan.walls[(int)mouseGPos.x][(int)mouseGPos.y+1] = 1;
                antMan.walls[(int)mouseGPos.x+1][(int)mouseGPos.y+1] = 1;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            {
                antMan.nestPos = screen_to_world_pos(sfvec2_to_vec_v(mousePos));
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                Vector mouseWPos = screen_to_world_pos(sfvec2_to_vec_v(mousePos));
                antMan.food.push_back(mouseWPos);
                antMan.foodNB++;
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    isPaused = !isPaused;
                    if (!hasStarted)
                    {
                        antMan.init(200);
                        hasStarted = true;
                    }
                }
            }
        }

        tick++;
        window.clear();
        if (!isPaused)
        {
            antMan.update(5);
        }

        window.draw(bgSpr);
        sf::RectangleShape square(sf::Vector2f(1.f/WALL_GRID_W * SCREEN_W, 1.f/WALL_GRID_H * SCREEN_H));
        square.setFillColor(sf::Color::White);
        for (int x = 0; x < WALL_GRID_W; ++x)
        {
            for (int y = 0; y < WALL_GRID_H; ++y)
            {
                if (!antMan.walls[x][y])
                {
                    continue;
                }
                square.setPosition(vec_to_sfvec2_v(grid_to_screen_pos(vec_v(x,y))));
                window.draw(square);
            }
        }

        sf::CircleShape circle;
        circle.setRadius(DRAW_PHERO_SIZE/worldSize.x * screenSize.x);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        sf::Color col = sf::Color(0, 75, 255);
        for (int i = 0; i < antMan.pherosNB[0]; ++i)
        {
            sf::Color tempCol = col;
            tempCol.a = antMan.pheros[0][i].strength * 255;
            circle.setPosition(vec_to_sfvec2_v(world_to_screen_pos(antMan.pheros[0][i].pos)));
            circle.setFillColor(tempCol);
            window.draw(circle);
        }
        col = sf::Color(255, 75, 0);
        for (int i = 0; i < antMan.pherosNB[1]; ++i)
        {
            sf::Color tempCol = col;
            tempCol.a = antMan.pheros[1][i].strength * 255;
            circle.setFillColor(tempCol);
            circle.setPosition(vec_to_sfvec2_v(world_to_screen_pos(antMan.pheros[1][i].pos)));
            window.draw(circle);
        }
        for (int i = 0; i < antMan.foodNB; ++i)
        {
            foodSpr.setPosition(vec_to_sfvec2_v(world_to_screen_pos(antMan.food[i])));
            window.draw(foodSpr);
        }
        if (hasStarted)
        {
            for (int i = 0; i < antMan.antsNB; ++i)
            {
                if (antMan.ants[i].holdFood)
                {
                    antFoodSpr.setRotation(rad_to_deg(antMan.ants[i].angle));
                    antFoodSpr.setPosition(vec_to_sfvec2_v(world_to_screen_pos(antMan.ants[i].pos)));
                    window.draw(antFoodSpr);
                }
                else
                {
                    antSpr.setRotation(rad_to_deg(antMan.ants[i].angle));
                    antSpr.setPosition(vec_to_sfvec2_v(world_to_screen_pos(antMan.ants[i].pos)));
                    window.draw(antSpr);
                }
            }
        }
        nestSpr.setPosition(vec_to_sfvec2_v(world_to_screen_pos(antMan.nestPos)));
        window.draw(nestSpr);
        text.setString(std::to_string(antMan.foodInNestNB));
        text.setPosition(vec_to_sfvec2_v(world_to_screen_pos(antMan.nestPos)));
        text.setCharacterSize(NEST_SIZE/worldSize.x * screenSize.x * 0.7f);
        centerText(&text);
        window.draw(text);

        window.display();

    }

	return 0;
}