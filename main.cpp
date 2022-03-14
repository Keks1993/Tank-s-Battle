#pragma once
#include "Common.h"
#include "Weapons.hpp"
#include "Army.hpp"
#include "Battle.hpp"


std::string settings_file = "settings.json";
std::string log_file = "output.txt";

int main(int argc, char* argv[])
{
	if (!fs::exists(settings_file))
	{
		std::cout << "file " << settings_file << "does not exist;";
		return 1;
	}

	Battle battle(settings_file, log_file);
	battle.simulate();

	return 0;
}