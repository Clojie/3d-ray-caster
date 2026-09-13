#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>

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

    const float movementSpeed = 3.5f;
    const float scale = 4.0f;
    float playerRotation = 0.0f;

    bool rotateLeft = false;
    bool rotateRight = false;

    const float rotationSpeed = 120.0f;
    const float pi = 3.14159265358907932384626433832795028841971693993f;

    int pov = 60;

    sf::RenderWindow window(sf::VideoMode(640, 400), "3D Ray Caster");
    window.setFramerateLimit(60);

    sf::Texture wallTexture;
    wallTexture.loadFromFile("resources/face.png");

    bool movingUp = false;
    bool movingDown = false;
    bool movingLeft = false;
    bool movingRight = false;

    sf::RectangleShape ceiling(sf::Vector2f(640, 200));
    ceiling.setFillColor(sf::Color(60, 60, 120));

    sf::RectangleShape floorRect(sf::Vector2f(640, 200));
    floorRect.setFillColor(sf::Color(80, 80, 80));
    floorRect.setPosition(0, 200);

    sf::Clock clock;

    while (window.isOpen())
    {

        float dt = clock.restart().asSeconds();

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) {
                    movingUp = true;
                }
                if (event.key.code == sf::Keyboard::S) {
                    movingDown = true;
                }
                if (event.key.code == sf::Keyboard::A) {
                    movingLeft = true;
                }
                if (event.key.code == sf::Keyboard::D) {
                    movingRight = true;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    rotateRight = true;
                }
                if (event.key.code == sf::Keyboard::Left) {
                    rotateLeft = true;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W) {
                    movingUp = false;
                }
                if (event.key.code == sf::Keyboard::S) {
                    movingDown = false;
                }
                if (event.key.code == sf::Keyboard::A) {
                    movingLeft = false;
                }
                if (event.key.code == sf::Keyboard::D) {
                    movingRight = false;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    rotateRight = false;
                }
                if (event.key.code == sf::Keyboard::Left) {
                    rotateLeft = false;
                }
            }
        }

        // Move player
        float oldPlayerX = playerX;
        float oldPlayerY = playerY;

        float angleRad = playerRotation * pi / 180.0f;

        if (movingUp) {
            playerX += movementSpeed * cos(angleRad) * dt;
            playerY += movementSpeed * sin(angleRad) * dt;
        }
        if (movingDown) {
            playerX -= movementSpeed * cos(angleRad) * dt;
            playerY -= movementSpeed * sin(angleRad) * dt;
        }
        if (movingRight) {
            playerX += movementSpeed * cos(angleRad + pi / 2) * dt;
            playerY += movementSpeed * sin(angleRad + pi / 2) * dt;
        }
        if (movingLeft) {
            playerX -= movementSpeed * cos(angleRad + pi / 2) * dt;
            playerY -= movementSpeed * sin(angleRad + pi / 2) * dt;
        }
        if (rotateLeft) {
            playerRotation -= rotationSpeed * dt;
        }
        if (rotateRight) {
            playerRotation += rotationSpeed * dt;
        }

        // player collision
        if (playerX < 0 || playerX >= boardSize || arr[(int)oldPlayerY][(int)playerX] != 0) {
            playerX = oldPlayerX;
        }
        if (playerY < 0 || playerY >= boardSize || arr[(int)playerY][(int)playerX] != 0) {
            playerY = oldPlayerY;
        }


        window.clear();
        window.draw(ceiling);
        window.draw(floorRect);

        sf::VertexArray walls(sf::Quads);
        sf::Vector2u texSize = wallTexture.getSize();
        float angleDelta = ((float)pov) / window.getSize().x;
        int currentX = 0;
        for (float angle = 0; angle < pov; angle += angleDelta) {
            float deltaX = cos((playerRotation + angle - pov / 2) * pi / 180);
            float deltaY = sin((playerRotation + angle - pov / 2) * pi / 180);

            int wallX = (int)playerX;
            int wallY = (int)playerY;

            float deltaDistX = (deltaX == 0) ? 1e30f : std::abs(1.0f / deltaX);
            float deltaDistY = (deltaY == 0) ? 1e30f : std::abs(1.0f / deltaY);

            int stepX, stepY;
            float sideDistX, sideDistY;

            if (deltaX < 0) {
                stepX = -1; sideDistX = (playerX - wallX) * deltaDistX;
            }
            else {
                stepX =  1; sideDistX = (wallX + 1.0f - playerX) * deltaDistX;
            }
            if (deltaY < 0) {
                stepY = -1; sideDistY = (playerY - wallY) * deltaDistY;
            }
            else {
                stepY =  1; sideDistY = (wallY + 1.0f - playerY) * deltaDistY;
            }

            bool hit = false;
            int side = 0;
            while (!hit) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX; wallX += stepX; side = 0;
                }
                else {
                    sideDistY += deltaDistY; wallY += stepY; side = 1;
                }

                if (wallX < 0 || wallX >= boardSize || wallY < 0 || wallY >= boardSize) {
                    break;
                }
                if (arr[wallY][wallX] == 1) {
                    hit = true;
                }
            }

            if (!hit) {
                currentX++; continue;
            }

            // perpendicular distance fixes fisheye
            float rayLength = (side == 0)
                ? (wallX - playerX + (1 - stepX) / 2.0f) / deltaX
                : (wallY - playerY + (1 - stepY) / 2.0f) / deltaY;

            if (rayLength <= 0) {
                currentX++; continue;
            }

            int wallHeight = (int)(400 / rayLength);
            int yStart = std::max(0, 200 - wallHeight / 2);
            int yEnd   = std::min(399, 200 + wallHeight / 2);

            float hitFrac = (side == 0) ? playerY + rayLength * deltaY : playerX + rayLength * deltaX;
            hitFrac -= std::floor(hitFrac);
            int texX = (int)(hitFrac * texSize.x);
            if ((side == 0 && deltaX > 0) || (side == 1 && deltaY < 0)) {
                texX = texSize.x - texX - 1;
            }
            texX = std::max(0, std::min(texX, (int)texSize.x - 1));

            sf::Color shade = (side == 1) ? sf::Color(160, 160, 160) : sf::Color::White;

            sf::Vertex v0, v1, v2, v3;
            v0.position  = sf::Vector2f(currentX, yStart);
            v1.position  = sf::Vector2f(currentX + 1, yStart);
            v2.position  = sf::Vector2f(currentX + 1, yEnd);
            v3.position  = sf::Vector2f(currentX, yEnd);
            v0.texCoords = sf::Vector2f(texX, 0);
            v1.texCoords = sf::Vector2f(texX + 1, 0);
            v2.texCoords = sf::Vector2f(texX + 1, texSize.y);
            v3.texCoords = sf::Vector2f(texX, texSize.y);
            v0.color = v1.color = v2.color = v3.color = shade;

            walls.append(v0);
            walls.append(v1);
            walls.append(v2);
            walls.append(v3);
            currentX++;
        }
        window.draw(walls, &wallTexture);

        // minimap
        const int cell = 8;
        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                sf::RectangleShape tile(sf::Vector2f(cell - 1, cell - 1));
                tile.setPosition(col * cell, row * cell);
                tile.setFillColor(arr[row][col] == 1 ? sf::Color(200, 200, 200) : sf::Color(40, 40, 40, 180));
                window.draw(tile);
            }
        }

        sf::CircleShape dot(3);
        dot.setFillColor(sf::Color::Red);
        dot.setOrigin(3, 3);
        dot.setPosition(playerX * cell, playerY * cell);
        window.draw(dot);

        sf::VertexArray dir(sf::Lines, 2);
        dir[0].position = sf::Vector2f(playerX * cell, playerY * cell);
        dir[0].color = sf::Color::Red;
        dir[1].position = sf::Vector2f(playerX * cell + cos(angleRad) * 20,
                                        playerY * cell + sin(angleRad) * 20);
        dir[1].color = sf::Color::Red;
        window.draw(dir);

        window.display();
    }
    return 0;
}
