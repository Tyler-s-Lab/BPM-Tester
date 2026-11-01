#pragma once

#include <SFML/Graphics.hpp>

#include "GlobalBase.h"

class DisplayTextBox final:
	public sf::Drawable
{
public:
	DisplayTextBox();
	void start();
	void update_press(std::string_view bpm_num_text);
	void stop();
protected:
	static void setTextEach(sf::Text& t_text, unsigned int size = 50);
	void refreshPosition() const;
	static void refreshOriginEach(sf::Text& t_text, sf::Vector2f t_pos);
	static void refreshPositionEach(sf::Text& t_text, sf::Vector2f t_pos);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
	bool m_started;
	sf::Text m_ini;
	mutable sf::Text m_title;
	mutable sf::Text m_presstimeBPM;
};
