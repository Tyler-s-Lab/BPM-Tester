#include "BPMcomp.h"

BPMcomp::BPMcomp() :
	//m_pressCount(0),
	m_bpm(0.0f) {}

void BPMcomp::start() {
	//this->m_pressCount = 0;
	m_times.clear();
	m_times.push_back(0);
	return (void)this->m_timer.restart();
}

void BPMcomp::press() {
	m_times.push_back(m_timer.getElapsedTime().asMicroseconds());
	//return (void)(++this->m_pressCount);
}

std::string BPMcomp::getString() const {
	char tmp[64];
	this->update();
	sprintf_s(tmp, 64, "%.3lf", this->m_bpm);
	return std::string(tmp);
}

inline void BPMcomp::update() const {
	//this->m_bpm = this->m_pressCount * 60.0f / this->m_timer.getElapsedTime().asSeconds();
	int totalN = m_times.size();
	int interval = totalN / 2;
	double res = 0.0;
	int n = totalN - interval;
	for (int i = 0; i < n; ++i) {
		res += interval * 60.0 / (m_times[i + interval] - m_times[i]) * 1000000.0;
	}
	res /= n;
	m_bpm = res;
	return;
}
