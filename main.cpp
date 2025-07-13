#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main()
{
    const int boardSize = 10;
    const int arr[boardSize][boardSize] = {
        {1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 1, 1},
        {1, 0, 1, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    // Initializing Variables
    float playerX = 5;
    float playerY = 5;

    const float movementSpeed = 0.01f;
    const float scale = 4.0f;
    float playerRotation = 0.0f;

    bool rotateLeft = false;
    bool rotateRight = false;

    const float rotationSpeed = 0.5f;
    const float length = 1000.0f;
    const float pi = 3.14159265358907932384626433832795028841971693993f;

    int pov = 50;

    sf::RenderWindow window(sf::VideoMode(500, 400), "iD Tech SFML Project");

    sf::Texture cursorTexture;
    cursorTexture.loadFromFile("resources/cursor.png");

    sf::Sprite cursor;
    cursor.setTexture(cursorTexture);
    cursor.setScale(3, 3);
    cursor.setOrigin(3.5, 3.5);

    sf::Texture wallTexture;
    wallTexture.loadFromFile("resources/face.png");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("resources/LadyBug.png");

    sf::Sprite player;
    player.setTexture(playerTexture);
    player.setOrigin(4, 4);
    player.setScale(2, 2);
    player.setPosition(playerX, playerY);

    bool movingUp = false;
    bool movingDown = false;
    bool movingLeft = false;
    bool movingRight = false;

    while (window.isOpen())
    {

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::W)
                    movingUp = true;
                if (event.key.code == sf::Keyboard::S)
                    movingDown = true;
                if (event.key.code == sf::Keyboard::A)
                    movingLeft = true;
                if (event.key.code == sf::Keyboard::D)
                    movingRight = true;
                if (event.key.code == sf::Keyboard::Right) 
                    rotateRight = true;
                if (event.key.code == sf::Keyboard::Left) 
                    rotateLeft = true;
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::W)
                    movingUp = false;
                if (event.key.code == sf::Keyboard::S)
                    movingDown = false;
                if (event.key.code == sf::Keyboard::A)
                    movingLeft = false;
                if (event.key.code == sf::Keyboard::D)
                    movingRight = false;
                if (event.key.code == sf::Keyboard::Right)
                    rotateRight = false;
                if (event.key.code == sf::Keyboard::Left)
                    rotateLeft = false;
            }
            if (event.type == event.MouseMoved) {

                cursor.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            }
            //std::cout << playerRotation << std::endl;
        }

        // Move player
        float oldPlayerX = playerX;
        float oldPlayerY = playerY;

        if (movingUp) {
            playerY -= movementSpeed * sin((playerRotation + 90) * pi / 180);
            playerX -= movementSpeed * cos((playerRotation + 90)* pi / 180);
            std::cout << sin(playerRotation * pi / 180) << std::endl;
        }
        if (movingDown) {
            playerY += movementSpeed * sin((playerRotation + 90) * pi / 180);
            playerX += movementSpeed * cos((playerRotation + 90) * pi / 180);
        }
        if (movingRight) {
            playerY += movementSpeed * sin(playerRotation * pi / 180);
            playerX += movementSpeed * cos(playerRotation * pi / 180);
        }
        if (movingLeft) {
            playerY -= movementSpeed * sin(playerRotation * pi / 180);
            playerX -= movementSpeed * cos(playerRotation * pi / 180);
        }
        if (rotateLeft) {
            playerRotation -= rotationSpeed;
        }
        if (rotateRight) {
            playerRotation += rotationSpeed;
        }

        player.setRotation(playerRotation);

        // player collision
        if (playerX < 10 && playerX > 0 && playerY < 10 && playerY > 0 && arr[int(playerY)][int(playerX)] == 0) {
            player.setPosition((playerX * 8) * scale, (playerY * 8) * scale);
        }
        else {
            playerX = oldPlayerX;
            playerY = oldPlayerY;
            std::cout << "you hit a wall" << std::endl;
        }
        
        
        window.clear();
        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                if (arr[col][row] == 1) {
                    sf::Sprite wall;
                    wall.setTexture(wallTexture);
                    wall.setPosition((row * 8 + 4)* scale, (col * 8 + 4)* scale);
                    wall.setScale(scale, scale);
                    wall.setOrigin(4, 4);
                }
            }
        }
        sf::Vector2f prevPosX = player.getPosition();
        sf::Vector2f prevPosY = player.getPosition();
        float base = window.getSize().x / boardSize;
        float angleDelta = ((float)pov)/window.getSize().x;
        int currentX = 0;
        for (float angle = 0; angle < pov; angle += angleDelta) {
            float rayLength = 0;
            sf::VertexArray rayX(sf::LineStrip, 2);
            rayX[0].position = player.getPosition();
            while (rayLength < length) {
                float deltaX = sin((playerRotation + angle - pov / 2) * pi / 180);
                float deltaY = cos((playerRotation + angle - pov / 2) * pi / 180);
                float x = player.getPosition().x + rayLength * deltaX;
                float y = player.getPosition().y - rayLength * deltaY;
                rayX[1].position = sf::Vector2f(x, y);
                int wallX = x / scale / 8;
                int wallY = y / scale / 8;
                if (rayX[1].position.y > 0 && rayX[1].position.y < window.getSize().y && rayX[1].position.x > 0 && rayX[1].position.x < window.getPosition().x) {
                    if (arr[wallY][wallX] == 1) {
                        rayX[0].color = sf::Color::Blue;
                        rayX[1].color = sf::Color::Blue;
                        break;
                    }
                }
                rayLength+= 2;
            }
            rayLength = rayLength / 5;
            sf::VertexArray white(sf::LineStrip, 2);
            float ceiling = window.getSize().y / 2 - window.getSize().y / rayLength;
            float floor = window.getSize().y - ceiling;
            white[0].position = sf::Vector2f(currentX, ceiling);
            white[1].position = sf::Vector2f(currentX, floor);

            white[0].color = sf::Color::Blue;
            window.draw(white);
            currentX++;
        }
        window.display();
    }
    return 0;
}
