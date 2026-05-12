#include <iostream>
#include <SFML/graphics.hpp>
#include <vector>
class Cursor {
private:
    float lastMouseX;
    float lastMouseY;

    sf::VertexArray customPointer{sf::LineStrip};

    const std::vector<sf::Vector2f> shapeOffSets = {
        {0.0f, 0.0f}, {0.0f, 144.33f}, {35.0f, 120.33f},
        {58.0f,170.66f}, {87.66f, 154.0f}, {68.0f, 108.0f},
        {106.33f, 106.33f}, {0.0f, 0.0f}
    };
    /*
    sf::Vertex points[] = {
        sf::Vertex(sf::Vector2f(100.f,100.f), sf::Color::Red),
        sf::Vertex(sf::Vector2f(100.f,200.f), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(300.f,100.f), sf::Color::Green)
    };
    */

public:
    Cursor(float mX, float mY) : lastMouseX(mX), lastMouseY(mY) {
        for (const auto &offset : shapeOffSets) {
            customPointer.append(sf::Vertex(sf::Vector2f(mX + offset.x, mY + offset.y), sf::Color::White));
        }
    }
    void update(float newX, float newY) {
        float dx = newX - lastMouseX;
        float dy = newY - lastMouseY;

        for (int i = 0; i < customPointer.getVertexCount(); ++i) {
            customPointer[i].position.x += dx;
            customPointer[i].position.y += dy;
        }

        // update lastMouse x and y
        lastMouseX = newX;
        lastMouseY = newY;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(customPointer);
    }
};

//class PaintStroke
int main() {
    //sf::ContextSettings settings;
    //settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Paint");
    window.setVerticalSyncEnabled(true);
    sf::Clock clock;
    // This hides the default Windows/macOS arrow
    window.setMouseCursorVisible(false);
    // initalize cursor object
    Cursor cursor(0.0f, 0.0f);
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseMoved:
                    cursor.update(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            }
            window.clear();
            cursor.draw(window);
            window.display();
        }
    }
    return 0;
}