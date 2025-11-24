// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>

using namespace sf;
using namespace std;

int main()
{
    VideoMode vm(1920, 1080);

    RenderWindow window(vm, "Chaos Game!!", Style::Default);

    vector<Vector2f> vertices;
    vector<Vector2f> points;

    // Font for text display
    Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        // If arial.ttf is not available, try to use the default font
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        {
            cout << "Error loading font!" << endl;
        }
    }

    // Text objects for user instructions
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(Color::White);
    instructionText.setPosition(10.f, 10.f);

    srand(time(0));

    // Game state
    enum GameState { SELECT_VERTICES, SELECT_START, RUNNING };
    GameState currentState = SELECT_VERTICES;

    while (window.isOpen())
    {
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "the left button was pressed" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;

                    if (currentState == SELECT_VERTICES && vertices.size() < 3)
                    {
                        vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));

                        if (vertices.size() == 3)
                        {
                            currentState = SELECT_START;
                        }
                    }
                    else if (currentState == SELECT_START && points.size() == 0)
                    {
                        // Fourth click - starting point
                        points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                        currentState = RUNNING;
                    }
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        /*
        ****************************************
        Update
        ****************************************
        */

        // Update instruction text based on current state
        if (currentState == SELECT_VERTICES)
        {
            stringstream ss;
            ss << "Click " << (3 - vertices.size()) << " more point(s) to create triangle vertices";
            instructionText.setString(ss.str());
        }
        else if (currentState == SELECT_START)
        {
            instructionText.setString("Click one more point to start the algorithm");
        }
        else if (currentState == RUNNING)
        {
            instructionText.setString("Algorithm running... Press ESC to exit");

            // Generate multiple points each frame for faster fractal generation
            for (int i = 0; i < 100; i++)  // Generate 100 points per frame
            {
                if (points.size() > 0)
                {
                    // Select random vertex
                    int randomIndex = rand() % 3;

                    // Calculate midpoint between random vertex and the last point
                    Vector2f lastPoint = points.back();
                    Vector2f vertex = vertices[randomIndex];

                    Vector2f newPoint;
                    newPoint.x = (lastPoint.x + vertex.x) / 2.0f;
                    newPoint.y = (lastPoint.y + vertex.y) / 2.0f;

                    // Add the new point
                    points.push_back(newPoint);
                }
            }
        }

        /*
        ****************************************
        Draw
        ****************************************
        */
        window.clear();

        window.draw(instructionText);

        // Draw vertices as blue squares
        for (int i = 0; i < vertices.size(); i++)
        {
            RectangleShape rect(Vector2f(10, 10));
            rect.setPosition(Vector2f(vertices[i].x - 5, vertices[i].y - 5)); // Center the square
            rect.setFillColor(Color::Blue);
            window.draw(rect);
        }

        // Draw points as small red dots
        for (int i = 0; i < points.size(); i++)
        {
            // Using small rectangles for points
            RectangleShape point(Vector2f(2, 2));
            point.setPosition(points[i]);
            point.setFillColor(Color::Red);
            window.draw(point);
        }

        window.display();
    }

    return 0;
}