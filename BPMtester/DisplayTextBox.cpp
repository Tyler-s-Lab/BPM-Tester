#include "DisplayTextBox.h"

namespace {

constexpr sf::Vector2f m_titleOrigin{ 0.5f, 0.5f };
constexpr sf::Vector2f m_pressimeOrigin{ 0.5f, 0.5f };
constexpr sf::Vector2f m_titlePosition{ 0.5f, 0.2f };
constexpr sf::Vector2f m_presstimePosition{ 0.5f, 0.4f };

}

DisplayTextBox::DisplayTextBox() :
	m_started(false),
	m_ini(GlobalBase::instance()->getFont()),
	m_title(GlobalBase::instance()->getFont()),
	m_presstimeBPM(GlobalBase::instance()->getFont()) {

	this->setTextEach(this->m_ini, 30);
	this->m_ini.setString(L"Press any key to start");
	this->refreshOriginEach(this->m_ini, m_titleOrigin);
	this->refreshPositionEach(this->m_ini, { 0.5f, 0.4f });

	this->setTextEach(this->m_title, 45);
	this->m_title.setString("BPM");
	this->refreshPositionEach(this->m_title, m_titlePosition);

	this->setTextEach(this->m_presstimeBPM);
	this->refreshPositionEach(this->m_presstimeBPM, m_presstimePosition);
	return;
}

void DisplayTextBox::start() {
	this->m_presstimeBPM.setString("0.000");
	this->m_started = true;
	return;
}

inline void DisplayTextBox::setTextEach(sf::Text& t_text, unsigned int size) {
	t_text.setCharacterSize(size);
	t_text.setFillColor(sf::Color::Red);
	return;
}

inline void DisplayTextBox::refreshPosition() const {
	this->refreshOriginEach(this->m_title, m_titleOrigin);
	this->refreshOriginEach(this->m_presstimeBPM, m_pressimeOrigin);
	return;
}

inline void DisplayTextBox::refreshOriginEach(sf::Text& t_text, sf::Vector2f t_pos) {
	sf::FloatRect _t_fr = t_text.getGlobalBounds();
	t_text.setOrigin({ round(_t_fr.size.x * t_pos.x), round(_t_fr.size.y * t_pos.y) });
	return;
}

inline void DisplayTextBox::refreshPositionEach(sf::Text& t_text, sf::Vector2f t_pos) {
	sf::Vector2u _t_fr = GlobalBase::instance()->getWindowSize();
	t_text.setPosition({ round(_t_fr.x * t_pos.x), round(_t_fr.y * t_pos.y) });
	return;
}

void DisplayTextBox::update_press(std::string_view bpm_num_text) {
	return this->m_presstimeBPM.setString(std::string{ bpm_num_text });
}

void DisplayTextBox::stop() {
	this->m_started = false;
	return;
}

void DisplayTextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (this->m_started) {
		this->refreshPosition();
		target.draw(this->m_title, states);
		target.draw(this->m_presstimeBPM, states);
	}
	else {
		target.draw(this->m_ini, states);
	}
	return;
}
