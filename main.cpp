#include <iostream>
#include <SFML/graphics.hpp>
#include <vector>
#include <cmath>
#include <thread>
#include <atomic>

// atomic variable that both threads can see
std::atomic<int> globalStrokeWidth(20);
// function will run on the second thread
void inputListener() {
    int newWidth;
    while (true) {
        std::cout << "Enter new pixel width(1-50): ";
        if (std::cin >> newWidth) {
            if (newWidth >= 1 && newWidth <= 50) {
                globalStrokeWidth = newWidth;
            } else {
                std::cout << "Invalid pixel width" <<'\n';
            }

        }
    }
}
class Cursor {
private:
    sf::ConvexShape cursorShape;
    /*
    sf::VertexArray customPointer{sf::LineStrip};

    const std::vector<sf::Vector2f> shapeOffSets = {
        {0.0f, 0.0f}, {0.0f, 144.33f}, {35.0f, 120.33f},
        {58.0f,170.66f}, {87.66f, 154.0f}, {68.0f, 106.33f},
        {106.33f, 106.33f}, {0.0f, 0.0f}
    };
    */

public:
    Cursor() {
        cursorShape.setPointCount(7);

        cursorShape.setPoint(0, sf::Vector2f(0.0f, 0.0f));
        cursorShape.setPoint(1,sf::Vector2f(0.0f,72.16f));
        cursorShape.setPoint(2, sf::Vector2f(17.5f, 60.16f));
        cursorShape.setPoint(3, sf::Vector2f(29.0f,85.33f));
        cursorShape.setPoint(4, sf::Vector2f(43.83f, 77.0f));
        cursorShape.setPoint(5, sf::Vector2f(34.0f,53.16f));
        cursorShape.setPoint(6, sf::Vector2f(53.16f, 53.16f));

        //Styling
        cursorShape.setFillColor(sf::Color::Transparent);
        cursorShape.setOutlineColor(sf::Color::Black);
        cursorShape.setOutlineThickness(3.0f);
        /*
        for (const auto &offset : shapeOffSets) {
            customPointer.append(sf::Vertex(sf::Vector2f(mX + offset.x, mY + offset.y), sf::Color::Black));
        }
        */
    }
    void update(float newX, float newY) {
        cursorShape.setPosition(newX, newY);
        /*
        float dx = newX - lastMouseX;
        float dy = newY - lastMouseY;

        for (int i = 0; i < customPointer.getVertexCount(); ++i) {
            customPointer[i].position.x += dx;
            customPointer[i].position.y += dy;
        }

        // update lastMouse x and y
        lastMouseX = newX;
        lastMouseY = newY;
        */
    }

    void draw(sf::RenderWindow& window) {
        window.draw(cursorShape);
    }
};

class PaintStroke {
private:
    int strokeWidth;
    sf::RenderTexture canvas; // drawing surface
    sf::Sprite canvasSprite; // used to display texture on the window
    sf::CircleShape brush;
    //sf::VertexArray paint;
    //std::vector<sf::Vector2f> offsets = getBrushOffsets();
public:
    PaintStroke(int pixelWidth, int windowWidth, int windowHeight) : strokeWidth(pixelWidth) {
        // initalize canvas
        if (!canvas.create(windowWidth, windowHeight)) {
            // Error
        }
        canvas.clear(sf::Color::Transparent);

        brush.setRadius(strokeWidth / 2.0f);
        brush.setFillColor(sf::Color::Red);
        brush.setOrigin(strokeWidth / 2.0f, strokeWidth / 2.0f); // center brush
    }

    /*
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


    */
    void paintAt(float x, float y) {
        brush.setPosition(x,y);
        canvas.draw(brush); // draw the brush onto the texture
        canvas.display();
    }

    void draw(sf::RenderWindow& window) {
        canvasSprite.setTexture(canvas.getTexture());
        window.draw(canvasSprite);
    }
    /*
    void strokeOffsets(float mouseDownX, float mouseDownY) {
        for (auto offset : offsets) {
            paint.append(sf::Vertex(sf::Vector2f({offset.x + mouseDownX, offset.y + mouseDownY}), sf::Color::Red));
        }
    }
    */
    void updateWidth(int newWidth) {
        strokeWidth = newWidth;
        // update brush
        brush.setRadius(strokeWidth / 2.0f);
        brush.setOrigin(strokeWidth / 2.0f, strokeWidth / 2.0f); // center brush

    }
    int getStrokeWidth() const  {
        return strokeWidth;
    }
};

class Color {

};
int main() {
    //sf::ContextSettings settings;
    //settings.antialiasingLevel = 8;
    sf::VideoMode Desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(Desktop, "Paint");

    window.setVerticalSyncEnabled(true);
    sf::Clock clock;
    // This hides the default Windows/macOS arrow
    window.setMouseCursorVisible(false);
    // initalize cursor object
    Cursor cursor;
    // start input thread
    std::thread inputThread(inputListener);
    inputThread.detach(); // let it run independently of the main loop

    PaintStroke paintStroke(globalStrokeWidth, Desktop.width, Desktop.height);
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

            if (paintStroke.getStrokeWidth() != globalStrokeWidth) {
                paintStroke.updateWidth(globalStrokeWidth);
            }
            sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            static sf::Vector2f lastPos = currentPos; // Keep track of the previous frame
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2f diff = currentPos - lastPos;
                float distance = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));

                float spacing = 20 * 0.1f; // 10% spacing
                if (spacing < 1.0f) spacing = 1.0f;
                // if the distance between the last frame and the current frame is greater than 1.0 -> interpolate(fill in empty space)
                if (distance > spacing) {
                    int steps = static_cast<int>(distance / spacing);
                    for (int i = 0; i < steps; ++i) {
                        float t = (float)i / steps;
                        // linear interpolation formula = a + t * (b-a)
                        float x = lastPos.x + t * diff.x;
                        float y = lastPos.y + t * diff.y;

                        paintStroke.paintAt(x,y);
                    }
                } else {
                    paintStroke.paintAt(currentPos.x, currentPos.y);
                }
            }
            lastPos = currentPos;
            window.clear(sf::Color::White);
            paintStroke.draw(window); // draws 1 sprite
            // draw cursor last so it appears on top of canvas
            cursor.draw(window);
            window.display();
        }
    }
    return 0;
}