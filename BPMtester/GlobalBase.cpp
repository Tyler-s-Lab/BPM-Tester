#include "GlobalBase.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <Windows.h>

namespace fs = std::filesystem;

GlobalBase* GlobalBase::m_instance = nullptr;

GlobalBase* GlobalBase::instance() {
	if (!GlobalBase::m_instance) {
		GlobalBase::m_instance = new GlobalBase;
	}
	return GlobalBase::m_instance;
}

const sf::Font& GlobalBase::getFont() const {
	return m_font;
}

sf::Vector2u GlobalBase::getWindowSize() const {
	return m_size;
}

GlobalBase::GlobalBase() :
	m_size(640, 360) {
	fs::path assets{ "assets" };

	std::ifstream ifs;
	ifs.open(assets / "ini.ini", std::ios_base::in);
	if (!ifs.is_open()) {
		std::cerr << "Open setting file " << assets / "ini.ini" << " failed!\n";
		throw 0;
	}

	std::string fontname;
	ifs >> fontname;
	if (!m_font.openFromFile(assets / fontname))
		throw 1;

	return;
}
