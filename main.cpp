#include <iostream>
#include <SFML/Graphics.hpp>

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    std::string name = "";
    int colCount = 25;
    int rowCount = 16;
    int mineCount = 50;
    int width = colCount*32;
    int height = rowCount*32 + 100;
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cout << "Error loading font" << std::endl;
        return -1;
    }
    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Welcome!", sf::Style::Close);

    sf::Text welcome("WELCOME TO MINESWEEPER!", font, 24);
    welcome.setFillColor(sf::Color::White);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcome, width/2, height/2 - 150);

    sf::Text enterName("Enter your name:", font, 20);
    enterName.setFillColor(sf::Color::White);
    enterName.setStyle(sf::Text::Bold);
    setText(enterName, width/2, height/2 - 75);

    sf::Text nameText(name + "|", font, 18);
    nameText.setFillColor(sf::Color::White);
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, width/2, height/2 - 45);

    while(welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
            }

            if(event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128 && event.text.unicode >= 32 && name.size() < 10) {
                    name += event.text.unicode;
                }
            }
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::BackSpace && !name.empty()) {
                    name.pop_back();
                }
            }
        }

        welcomeWindow.clear(sf::Color::Blue);

        welcomeWindow.draw(welcome);
        welcomeWindow.draw(enterName);
        nameText.setString(name + "|");
        setText(nameText, width/2, height/2 - 45);
        welcomeWindow.draw(nameText);

        welcomeWindow.display();
    }
    return 0;
}