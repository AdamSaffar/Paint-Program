#include <iostream>
#include <SFML/graphics.hpp>
#include <vector>
#include <cmath>
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

class PaintStroke {
private:
    int strokeWidth;
    sf::VertexArray paint;
    std::vector<sf::Vector2f> offsets = getBrushOffsets();
public:
    PaintStroke(int pixelWidth) : strokeWidth(pixelWidth){}


    std::vector<sf::Vector2f> getBrushOffsets() {
        std::vector<sf::Vector2f> offsets;
        // find center point of brush grid
        float center = (strokeWidth - 1) / 2.0f;
        float radius = strokeWidth / 2.0f;

        for (int y = 0; y < strokeWidth; y++) {
            for (int x = 0; x < strokeWidth ; x++) {
                // distance formula = sqrt(x^2 + y^2)
                float dx = x - center;
                float dy = y - center;

                if ((dx * dx) + (dy * dy) <= (radius * radius)) {
                    offsets.push_back({static_cast<float>(x), static_cast<float>(y)});
                }
            }
        }
        return offsets;
    }

    void strokeOffsets(float mouseDownX, float mouseDownY) {
        for (auto offset : offsets) {
            paint.append(sf::Vertex(sf::Vector2f({offset.x + mouseDownX, offset.y + mouseDownY}), sf::Color::Red));
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(paint);
    }
};

class Color {

};
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
    //int pixelWidth;
    //std::cin >>  pixelWidth;
    PaintStroke paintStroke(20);
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
                    break;
            }
            sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            static sf::Vector2f lastPos = currentPos; // Keep track of the previous frame
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2f diff = currentPos - lastPos;
                float distance = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));

                // if the distance between the last frame and the current frame is greater than 1.5 -> interpolate(fill in empty space)
                if (distance > 1.0f) {
                    int steps = static_cast<int>(distance);
                    for (int i = 0; i < steps; ++i) {
                        float t = (float)i / steps;
                        // linear interpolation formula = a + t * (b-a)
                        float x = lastPos.x + t * diff.x;
                        float y = lastPos.y + t * diff.y;

                        paintStroke.strokeOffsets(x,y);
                    }
                } else {
                    paintStroke.strokeOffsets(currentPos.x, currentPos.y);
                }
            }
            lastPos = currentPos;
            window.clear();
            paintStroke.draw(window);
            // draw cursor last so it appears on top of canvas
            cursor.draw(window);
            window.display();
        }
    }
    return 0;
}