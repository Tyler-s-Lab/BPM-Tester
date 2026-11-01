#include "BPMcomp.h"

BPMcomp::BPMcomp() :
	m_bpm(0.0) {}

void BPMcomp::start() {
	m_times.clear();
	m_bpm = 0.0;
	// The first real tap defines t = 0 and starts the clock; the start key is
	// not a beat. The fit below keeps a free intercept, so any delay before
	// that first tap is absorbed instead of being treated as one beat.
}

void BPMcomp::press() {
	if (m_times.empty()) {
		m_timer.restart();
		m_times.push_back(0);
		return;
	}
	const long long now = m_timer.getElapsedTime().asMicroseconds();
	if (now <= m_times.back())
		return;
	m_times.push_back(now);
}

std::string BPMcomp::getString() const {
	char tmp[64];
	this->update();
	sprintf_s(tmp, 64, "%.3lf", this->m_bpm);
	return std::string(tmp);
}

void BPMcomp::update() const {
	const int count = static_cast<int>(m_times.size());
	if (count < 3) {
		m_bpm = 0.0;
		return;
	}

	// Ordinary least squares of t_j = tau + j * period:
	// period = cov(j, t) / var(j) = (K*Sjt - Sj*St) / (K*Sjj - Sj*Sj).
	double sumJ = 0.0;
	double sumJJ = 0.0;
	double sumT = 0.0;
	double sumJT = 0.0;
	for (int j = 0; j < count; ++j) {
		const double t = static_cast<double>(m_times[j]) * 1e-6;
		sumJ += static_cast<double>(j);
		sumJJ += static_cast<double>(j) * j;
		sumT += t;
		sumJT += static_cast<double>(j) * t;
	}

	const double denom = count * sumJJ - sumJ * sumJ;
	if (denom <= 0.0) {
		m_bpm = 0.0;
		return;
	}

	const double period = (count * sumJT - sumJ * sumT) / denom;
	m_bpm = (period > 0.0) ? 60.0 / period : 0.0;
	return;
}
