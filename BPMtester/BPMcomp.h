#pragma once

#include <SFML\System\Clock.hpp>

#include <string>
#include <vector>

class BPMcomp final {
public:
	BPMcomp();
	void start();
	void press();
	std::string getString() const;
protected:
	void update() const;
protected:
	//size_t m_pressCount;
	std::vector<long long> m_times;
	mutable double m_bpm;
	sf::Clock m_timer;
};
