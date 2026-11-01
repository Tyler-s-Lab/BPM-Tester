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
	// Microseconds; m_times[0] == 0 is the first real tap (not the start key).
	std::vector<long long> m_times;
	mutable double m_bpm;
	sf::Clock m_timer;
};
