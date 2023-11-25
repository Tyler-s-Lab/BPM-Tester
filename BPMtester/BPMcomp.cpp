#include "BPMcomp.h"

BPMcomp::BPMcomp() :
	m_pressCount(0),
	m_bpm(0.0f)
{
}

void BPMcomp::start() {
	this->m_pressCount = 0;
	return (void)this->m_timer.restart();
}

void BPMcomp::press() {
	return (void)(++this->m_pressCount);
}

std::string BPMcomp::getString() const {
	char tmp[64];
	this->update();
	sprintf_s(tmp, 64, "%.3f", this->m_bpm);
	return std::string(tmp);
}

inline void BPMcomp::update() const {
	this->m_bpm = this->m_pressCount * 60.0f / this->m_timer.getElapsedTime().asSeconds();
	return;
}
