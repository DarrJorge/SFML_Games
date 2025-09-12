//
// Created by Oleksandr Pogorelov.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>


using namespace sf;

class Button : public Drawable
{
public:
    using Callback = std::function<void()>;

    Button(const Font& font, const std::string& text, Vector2f pos, Vector2f size);

    void setCallback(Callback cb);
    void setEnabled(bool enabled);
    bool enabled() const { return m_enabled; }

    void setInteractable(bool interactable);
    bool interactable() const { return m_interactable; }

    void setPosition(Vector2f pos);
    Vector2f position() const { return m_shape.getPosition(); }
    Vector2f size() const { return m_shape.getSize(); }

    void updateHover(const Vector2f& mouse);
    bool onClick(const Vector2f& mouse);
    void setFocused(bool focused);

private:
    void draw(RenderTarget& target, RenderStates states) const override;
    void centerText();

private:
    RectangleShape m_shape;
    Text m_text;
    Callback m_callback;

    bool m_enabled{true};
    bool m_interactable{true};
    bool m_focused{false};

    Color m_bgNormal  { Color(40,40,60) };
    Color m_bgHover   { Color(70,70,110) };
    Color m_bgDisabled{ Color(30,30,30) };

};
