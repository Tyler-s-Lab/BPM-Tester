#include "DisplayTextBox.h"

DisplayTextBox::DisplayTextBox() :
	m_started           (false),
	m_titleOrigin       (0.5f, 0.5f),
	m_realtimeOrigin    (0.5f, 0.5f),
	m_pressimeOrigin    (0.5f, 0.5f),
	m_titlePosition     (0.5f, 0.2f),
	m_realtimePosition  (0.5f, 0.6f),
	m_presstimePosition (0.5f, 0.4f)
{
	this->setTextEach(this->m_ini, 30);
	this->setTextEach(this->m_title, 45);
	this->setTextEach(this->m_presstimeBPM);
	this->setTextEach(this->m_realtimeBPM, 30);

	this->m_ini.setString(L"Press any key to start");
	this->m_title.setString("BPM");

	this->refreshPositionEach(this->m_ini, { 0.5f, 0.4f });
	this->refreshPositionEach(this->m_title, this->m_titlePosition);
	this->refreshPositionEach(this->m_realtimeBPM, this->m_realtimePosition);
	this->refreshPositionEach(this->m_presstimeBPM, this->m_presstimePosition);
	this->refreshOriginEach(this->m_ini, this->m_titleOrigin);
	return;
}

void DisplayTextBox::start() {
	this->m_presstimeBPM.setString("0.000");
	return (void)(this->m_started = true);
}

inline void DisplayTextBox::setTextEach(sf::Text& t_text, unsigned int size) {
	t_text.setCharacterSize(size);
	t_text.setFillColor(sf::Color::Red);
	t_text.setFont(GlobalBase::instance()->getFont());
	return;
}

inline void DisplayTextBox::refreshPosition() const {
	this->refreshOriginEach(this->m_title, this->m_titleOrigin);
	this->refreshOriginEach(this->m_realtimeBPM, this->m_realtimeOrigin);
	this->refreshOriginEach(this->m_presstimeBPM, this->m_pressimeOrigin);
	return;
}

inline void DisplayTextBox::refreshOriginEach(sf::Text& t_text, sf::Vector2f t_pos) {
	sf::FloatRect _t_fr = t_text.getGlobalBounds();
	t_text.setOrigin(_t_fr.width * t_pos.x, _t_fr.height * t_pos.y);
	return;
}

inline void DisplayTextBox::refreshPositionEach(sf::Text& t_text, sf::Vector2f t_pos) {
	sf::Vector2u _t_fr = GlobalBase::instance()->getWindowSize();
	t_text.setPosition(_t_fr.x * t_pos.x, _t_fr.y * t_pos.y);
	return;
}

void DisplayTextBox::update_press(const BPMcomp& BPM) {
	return this->m_presstimeBPM.setString(BPM.getString());
}

void DisplayTextBox::update(const BPMcomp& BPM) {
	return this->m_realtimeBPM.setString(BPM.getString());
}

void DisplayTextBox::stop() {
	return (void)(this->m_started = false);
}

void DisplayTextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (this->m_started) {
		this->refreshPosition();
		target.draw(this->m_title, states);
		target.draw(this->m_realtimeBPM, states);
		target.draw(this->m_presstimeBPM, states);
	}
	else {
		target.draw(this->m_ini, states);
	}
	return;
}
