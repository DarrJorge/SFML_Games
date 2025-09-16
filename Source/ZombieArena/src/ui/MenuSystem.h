//
// Created by Oleksandr Pogorelov.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "../core/GameTypes.h"
#include "Button.h"

class EventBus;

class MenuSystem
{
public:
    MenuSystem(sf::Vector2u& resolution, EventBus& events);

    void update(const sf::RenderWindow& win);
    void handleEvent(const std::optional<sf::Event>& event, const sf::RenderWindow& win);
    void draw(sf::RenderTarget& rt);
    void setContinueEnabled(bool enabled);
    void setVisible(bool visible) { m_visible = visible; }
    bool visible() const { return m_visible; }

    void setWinScreen();
    void setLoseScreen();

private:
    void initButtons();
    void setFocus(int idx);
    void updateTitle(const std::string& title, const sf::Color& color = sf::Color::White);
    void resetButtons();

private:
    Vector2u& m_resolution;
    EventBus& m_events;
    sf::View m_view;
    sf::Sprite m_background;

    sf::Font m_font;
    sf::Text m_title;

    std::vector<Button> m_buttons;
    int m_focus{0};
    bool m_visible{true};
};
