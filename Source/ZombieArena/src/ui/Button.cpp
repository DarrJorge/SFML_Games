//
// Created by Oleksandr Pogorelov.
//

#include "Button.h"
#include <iostream>

using namespace sf;


Button::Button(const Font& font, const std::string& text, Vector2f pos, Vector2f size)
    : m_text(font, text)
{
    m_shape.setSize(size);
    m_shape.setPosition(pos);
    m_shape.setFillColor(m_bgNormal);
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(Color(60,60,60));

    m_text.setCharacterSize(28);
    centerText();
}

void Button::setCallback(Callback cb)
{
    m_callback = std::move(cb);
}

void Button::setEnabled(bool enabled)
{
    m_enabled = enabled;
    m_shape.setFillColor(enabled ? m_bgNormal : m_bgDisabled);
}

void Button::setInteractable(bool interactable)
{
    m_interactable = interactable;
    m_shape.setFillColor(m_interactable ? m_bgNormal : m_bgDisabled);
}

void Button::setPosition(Vector2f pos)
{
    m_shape.setPosition(pos);
    centerText();
}

void Button::updateHover(const Vector2f& mouse)
{
    if (!m_interactable)
    {
        m_shape.setFillColor(m_bgDisabled);
        return;
    }

    if (m_shape.getGlobalBounds().contains(mouse))
        m_shape.setFillColor(m_bgHover);
    else
        m_shape.setFillColor(m_bgNormal);
}

bool Button::onClick(const Vector2f& mouse)
{
    if (!m_interactable) return false;
    if (m_shape.getGlobalBounds().contains(mouse) && m_callback)
    {
        m_callback();
        return true;
    }
    return false;
}

void Button::setFocused(bool focused)
{
    m_focused = focused;
    m_shape.setOutlineColor(focused ? Color::White : Color(60,60,60));
}

void Button::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_text, states);
}

void Button::centerText()
{
    const auto bgRect = m_shape.getGlobalBounds();
    const auto textRect = m_text.getLocalBounds();
    const Vector2f textPos = {bgRect.position.x + (bgRect.size.x - textRect.size.x)/2.f - textRect.position.x,
                              bgRect.position.y  + (bgRect.size.y - textRect.size.y)/2.f - textRect.position.y - 4.f};
    m_text.setPosition(textPos);
}