//
// Created by Oleksandr Pogorelov.
//

#include "MenuSystem.h"
#include "../core/Events.h"
#include "ResourceManager.h"

using namespace sf;
using namespace ZombieArena::Core;

MenuSystem::MenuSystem(Vector2u& res, EventBus& events)
        : m_resolution(res)
        , m_events(events)
        , m_view(FloatRect(Vector2f(0, 0), Vector2f(static_cast<float>(res.x), static_cast<float>(res.y))))
        , m_font(ResourceManager::getFont(Paths::FONT_PATH))
        , m_title(m_font, "", 72)
{
    updateTitle("Zombie Arena");
    initButtons();
}

void MenuSystem::initButtons()
{
    m_buttons.reserve(4);

    const Vector2f buttonSize{ 360.f, 56.f };
    const float firstRowY = 360.f;
    const float rowSpacing = 88.f;
    const float halfResolutionX = static_cast<float>(m_resolution.x)*0.5f;
    const float cx = halfResolutionX - buttonSize.x * 0.5f;

    auto addButton = [&](std::string_view label, int rowIndex,
                         bool enabled, bool interactable, Button::Callback cb) -> Button&
    {
        const Vector2f pos{cx, firstRowY + rowIndex * rowSpacing};
        m_buttons.emplace_back(m_font, std::string(label), pos, buttonSize);
        Button& btn = m_buttons.back();
        btn.setEnabled(enabled);
        btn.setInteractable(interactable);
        btn.setCallback(std::move(cb));
        return btn;
    };

    addButton("Start Game", 0, true, true, [this]{ m_events.emit(ActionStartGame{}); m_buttons[0].setEnabled(false); });
    addButton("Continue", 0, false, true, [this]{ m_events.emit(ActionTogglePause{}); });
    addButton("Settings", 1, true, false, [this]{ /* TODO */ });
    addButton("Exit", 2, true, true, [this]{ std::exit(0); });

    setFocus(0);
}

void MenuSystem::setContinueEnabled(bool enabled)
{
    if (m_buttons.size() > 1)
    {
        m_buttons[1].setEnabled(enabled);
    }
}

void MenuSystem::update(const RenderWindow& win)
{
    if (!m_visible) return;
    const auto mouse = win.mapPixelToCoords(Mouse::getPosition(win), m_view);
    for (auto& b : m_buttons) b.updateHover(mouse);
}

void MenuSystem::handleEvent(const std::optional<sf::Event>& event, const RenderWindow& win)
{
    if (!m_visible || event == std::nullopt) return;

    if (auto* mouseButton = event.value().getIf<Event::MouseButtonPressed>())
    {
        if (mouseButton->button == Mouse::Button::Left)
        {
            const auto mouse = win.mapPixelToCoords(Mouse::getPosition(win), m_view);
            for (auto& b : m_buttons)
            {
                if (b.onClick(mouse)) break;
            }
        }
    }
    else if (auto* kp = event.value().getIf<Event::KeyPressed>())
    {
        if (kp->code == Keyboard::Key::Down)
        {
            setFocus((m_focus + 1) % (int)m_buttons.size());
        }
        else if (kp->code == Keyboard::Key::Up)
        {
            setFocus((m_focus - 1 + (int)m_buttons.size()) % (int)m_buttons.size());
        }
        else if (kp->code == Keyboard::Key::Enter || kp->code == Keyboard::Key::Space)
        {
            const auto pos = m_buttons[m_focus].position() + m_buttons[m_focus].size()*0.5f;
            m_buttons[m_focus].onClick(pos);
        }
        else if (kp->code == Keyboard::Key::Escape)
        {
            std::exit(0);
        }
    }
}

void MenuSystem::draw(RenderTarget& target)
{
    if (!m_visible) return;
    const auto prev = target.getView();
    target.setView(m_view);

    RectangleShape dim({ m_view.getSize() });
    dim.setFillColor(Color(0,0,0,180));
    target.draw(dim);

    target.draw(m_title);
    for (auto& b : m_buttons)
    {
        if (b.enabled())
            target.draw(b);
    }

    target.setView(prev);
}

void MenuSystem::setFocus(int idx)
{
    if (m_buttons.empty()) return;

    // search nearest enabled button
    int attempts = 0;
    while (!m_buttons[idx].enabled() && attempts < m_buttons.size())
    {
        int one = m_focus < idx ? 1 : -1;
        idx = (idx + one) % (int)m_buttons.size();
        ++attempts;
    }

    // take off focus from all buttons
    for (auto& b : m_buttons)
    {
        b.setFocused(false);
    }

    if (m_buttons[idx].enabled())
    {
        m_buttons[idx].setFocused(true);
        m_focus = idx;
    }
    else
    {
        m_focus = -1;
    }
}

void MenuSystem::updateTitle(const std::string& title, const Color& color)
{
    m_title.setString(title);
    m_title.setFillColor(color);
    const float halfResolutionX = static_cast<float>(m_resolution.x) * .5f;
    m_title.setPosition({halfResolutionX - m_title.getLocalBounds().size.x * 0.5f, 120.f});
}

void MenuSystem::setWinScreen()
{
    updateTitle("WIN!");
    resetButtons();
}

void MenuSystem::setLoseScreen()
{
    updateTitle("LOSE!");
    resetButtons();
}

void MenuSystem::resetButtons()
{
    if (m_buttons.size() > 1)
    {
        m_buttons[0].setEnabled(true);
        m_buttons[1].setEnabled(false);
    }
}
