#include <iostream>
#include <cctype>
#include <SFML/Graphics.hpp>

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    bool ingame = false;
    bool leaderBoard = false;
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

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                ingame = false;
                welcomeWindow.close();
            }

            if(event.type == sf::Event::TextEntered) {
                char typedChar = static_cast<char>(event.text.unicode);

                if (((typedChar >= 'a' && typedChar <= 'z') || (typedChar >= 'A' && typedChar <= 'Z')) && name.size() < 10) {
                    name += typedChar;
                    name[0] = std::toupper(name[0]);
                    if (name.size() > 1) {
                        for (int i = 1; i < name.size(); i++) {
                            name[i] = std::tolower(name[i]);
                        }
                    }
                }
            }
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::BackSpace && !name.empty()) {
                    name.pop_back();
                } else if (event.key.code == sf::Keyboard::Enter && name.size() > 0) {
                    std::cout << "Change screen to in game" << std::endl;
                    ingame = true;
                    welcomeWindow.close();
                } else if (event.key.code == sf::Keyboard::Escape) {
                    ingame = false;
                    welcomeWindow.close();
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
    while (ingame) {
        sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

        while (gameWindow.isOpen()) {
            sf::Event event;
            while(gameWindow.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    ingame = false;
                    gameWindow.close();
                }
                if(event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        std::cout << "Open leader board screen" << std::endl;
                        leaderBoard = true;
                    }
                }
                gameWindow.clear(sf::Color::White);
                gameWindow.display();
            }

            if (leaderBoard) {
                sf::RenderWindow leaderBoardWindow(sf::VideoMode(colCount*16, rowCount*16 + 50), "Leader Board", sf::Style::Close);

                while (leaderBoardWindow.isOpen()) {
                    while(leaderBoardWindow.pollEvent(event)) {
                        if(event.type == sf::Event::Closed) {
                            leaderBoard = false;
                            leaderBoardWindow.close();
                        }
                        if(event.type == sf::Event::KeyPressed) {
                            if (event.key.code == sf::Keyboard::Escape) {
                                std::cout << "Closing leader board screen" << std::endl;
                                leaderBoard = false;
                                leaderBoardWindow.close();
                            }
                        }
                        leaderBoardWindow.clear(sf::Color::Blue);
                        leaderBoardWindow.display();
                    }
                }
            }
        }
    }
    return 0;
}