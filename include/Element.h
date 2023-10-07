#pragma once
#include <SFML/Graphics.hpp>

enum Element_Type{
    EMPTY, CURSOR, WIRE, RESISTOR, BATTERY
};

sf::Texture Elements;

class Element{
public:
    Element_Type m_Type = WIRE;
    sf::Sprite m_Sprite;
    Element(){}
    Element(int size, Element_Type type, sf::Texture &texture){
        m_Sprite.setTexture(texture);
        m_Sprite.setTextureRect(sf::IntRect(0,80*(int)(type-2),80,80));
        m_Sprite.setOrigin({0,m_Sprite.getLocalBounds().height/2-3});
        m_Sprite.setScale(size/80.0,size/80.0);
    }
};