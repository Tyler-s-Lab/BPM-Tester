#pragma once

#include<SFML\Graphics\Font.hpp>

class GlobalBase final {
public:
	const sf::Font& getFont() const;
	sf::Vector2u getWindowSize() const;
public:
	static GlobalBase* instance();
protected:
	GlobalBase();
protected:
	sf::Font m_font;
	sf::Vector2u m_size;
protected:
	static GlobalBase* m_instance;
};

