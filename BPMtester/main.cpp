
#include "DisplayTextBox.h"
#include "TapTempoPLL.h"
#include <format>

#define ISOLATION_AWARE_ENABLED 1
#include <Windows.h>
#include <commctrl.h>
#undef ISOLATION_AWARE_ENABLED

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

void Run() {
	sf::RenderWindow window{
		sf::VideoMode{ { GlobalBase::instance()->getWindowSize().x, GlobalBase::instance()->getWindowSize().y } },
		"BPM",
		sf::Style::Default ^ sf::Style::Resize
	};
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(500);
	window.setKeyRepeatEnabled(false);

	sf::Clock clock;
	TapTempoPLL pll;
	DisplayTextBox textbox;
	unsigned char UI = 1;
	while (UI) {
		UI = 1;
		pll.reset();
		while (UI == 1) {
			while (const std::optional event = window.pollEvent()) {
				if (event->is<sf::Event::Closed>()) {
					UI = 0;
					continue;
				}
				if (event->is<sf::Event::KeyPressed>()) {
					++UI;
					continue;
				}
			}
			window.clear(sf::Color::White);
			window.draw(textbox);
			window.display();
		}
		textbox.start();
		clock.restart();
		while (UI == 2) {
			while (const std::optional event = window.pollEvent()) {
				const auto timepoint = clock.getElapsedTime();
				if (const auto pkey = event->getIf<sf::Event::KeyPressed>()) {
					if (pkey->code == sf::Keyboard::Key::Escape)
						UI = 3;
					pll.addTap(timepoint.asSeconds());
					textbox.update_press(std::format("{:.3f}\n", pll.getBPMOr(0.0)));
					continue;
				}
				if (event->is<sf::Event::Closed>()) {
					UI = 0;
					continue;
				}
			}

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
	catch (int) {
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
