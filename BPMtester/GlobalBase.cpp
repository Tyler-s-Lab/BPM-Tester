#include "GlobalBase.h"

#include <Windows.h>

GlobalBase* GlobalBase::m_instance = nullptr;

GlobalBase* GlobalBase::instance() {
	if (!GlobalBase::m_instance) {
		GlobalBase::m_instance = new GlobalBase;
	}
    return GlobalBase::m_instance;
}

const sf::Font& GlobalBase::getFont() const {
	return this->m_font;
}

sf::Vector2u GlobalBase::getWindowSize() const {
	return this->m_size;
}

GlobalBase::GlobalBase() :
	m_size(640, 360)
{
	FILE* fp;
	fopen_s(&fp, "assets/ini.ini", "r");
	if (fp == NULL) {
		fprintf_s(stderr, "Open setting file \"assets\\ini.ini\" failed!\n");
		throw 0;
	}
	char fontname[64] = { 0 };
	fscanf_s(fp, "%s", fontname, 63);
	if (!this->m_font.loadFromFile(std::string("assets\\").append(fontname)))
		throw 1;
	return;
}
