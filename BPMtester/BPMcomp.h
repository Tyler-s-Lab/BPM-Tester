#pragma once

#include <SFML\System\Clock.hpp>

#include <string>

class BPMcomp final {
public:
	BPMcomp();
	void start();
	void press();
	std::string getString() const;
protected:
	void update() const;
protected:
	size_t m_pressCount;
	mutable float m_bpm;
	sf::Clock m_timer;
};
