
#include "DisplayTextBox.h"

#include <stdio.h>

#define ISOLATION_AWARE_ENABLED 1
#include <Windows.h>
#include <commctrl.h>
#undef ISOLATION_AWARE_ENABLED

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

void Run() {
	sf::RenderWindow window(
		sf::VideoMode(GlobalBase::instance()->getWindowSize().x, GlobalBase::instance()->getWindowSize().y),
		"BPM",
		sf::Style::Default ^ sf::Style::Resize
	);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	BPMcomp BPM;
	DisplayTextBox textbox;
	sf::Event evt;
	unsigned char UI = 1;
	while (UI) {
		UI = 1;
		while (UI == 1) {
			while (window.pollEvent(evt)) {
				switch (evt.type) {
				case sf::Event::Closed:
					UI = 0;
					break;
				case sf::Event::KeyPressed:
					++UI;
					break;
				}
			}
			window.clear(sf::Color::White);
			window.draw(textbox);
			window.display();
		}
		textbox.start();
		BPM.start();
		while (UI == 2) {
			while (window.pollEvent(evt)) {
				switch (evt.type) {
				case sf::Event::Closed:
					UI = 0;
					break;
				case sf::Event::KeyPressed:
					if (evt.key.code == sf::Keyboard::Escape)
						UI = 3;
					BPM.press();
					textbox.update_press(BPM);
					break;
				}
			}
			textbox.update(BPM);
			window.clear(sf::Color::White);
			window.draw(textbox);
			window.display();
		}
		textbox.stop();
	}
	window.close();
}

int main() {
startpoint:
	try {
		Run();
	}
	catch (int err) {
		int chos = MessageBoxW(
			NULL,
			L"Assets missing.",
			L"BPMtester: Initialize Failed",
			MB_RETRYCANCEL | MB_ICONERROR | MB_TOPMOST
		);
		if (chos == IDRETRY)
			goto startpoint;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
