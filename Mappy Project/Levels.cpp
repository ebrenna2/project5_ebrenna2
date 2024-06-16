#include "Levels.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <cstdio>

Levels::Levels() :currentLevel(1), totalLevels(3), timeLimit(60), counter(0), gameOver(false) {
	font = al_load_ttf_font("AppleGaramond", 36, 0);
}

