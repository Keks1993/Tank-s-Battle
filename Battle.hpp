#pragma once
#include "Common.h"
#include "Army.hpp"
#include "Weapons.hpp"

class Battle
{
	std::string log_name_;

	struct Event {
		std::string name_;
		std::string from_;
		std::string to_;
		std::string res_;
	};

	std::vector<Event> events_;
	std::vector<std::shared_ptr<Army>> participants_;
	std::mt19937 g_;

	bool isGameContinued() const
	{
		if (participants_.at((uint32_t)Army_id::Allies)->isArmyLive() && participants_.at((uint32_t)Army_id::Axis)->isArmyLive())
		{
			return true;
		}
		return false;
	};


public:
	Battle(const std::string& settings, const std::string& log) : log_name_{ log }
	{
		pt::ptree tree;
		pt::read_json(settings, tree);
		std::unique_ptr<WeaponsFactory> WeaponsFactory;
		g_.seed(ch::duration_cast<ch::nanoseconds>(ch::steady_clock::now().time_since_epoch()).count());

		for (const auto& i : tree.get_child(""))
		{
			auto it = player_names.find(i.first);
			if (it != player_names.end())
			{
				switch ((*it).second)
				{
				case Army_id::Axis:
					WeaponsFactory = std::make_unique<AxisWeaponsFactory>();
					break;
				case Army_id::Allies:
					WeaponsFactory = std::make_unique<AlliesWeaponsFactory>();
					break;
				}
				participants_.push_back(std::make_shared<Army>(WeaponsFactory.get(), i.first, i.second));
			}
			else
			{
				std::cout << "army " << i.first << " is not supported yet" << std::endl;
			}
		}
	};

	void simulate()
	{
		std::uniform_int_distribution<size_t> dis(0, participants_.size() - 1);
		size_t round{dis(g_)};
		while (isGameContinued())
		{
			std::string army_name = participants_.at(round % participants_.size())->getName();

			auto attacker = participants_.at(round % participants_.size())->chooseWeapon(g_);
			auto target = participants_.at(++round % participants_.size())->chooseWeapon(g_);

			std::string res;
			if (attacker->shoot(target))
			{
				// hit
				res = std::to_string(attacker->getDamage()) + " damage, " + target->getID();
				if (target->getStrength() == 0)
				{
					//destroyed
					participants_.at(round % participants_.size())->destroyWeapon();
					res += " destroyed.";
				}
				else
				{
					//damage
					res += " " + std::to_string(target->getStrength()) + " left.";
				}
			}
			else
			{
				//miss
				res = "miss, " + target->getID() + " " + std::to_string(target->getStrength()) + " left.";
			}
			events_.push_back({army_name, attacker->getID(), target->getID(), res});
		}

		printHistory();
	};

	void printHistory() const {
		std::ofstream log(log_name_);
		for (const auto& [army, from, to, res] : events_) {
			log << army<<": " << from << " shoots at " << to << ". Result: " << res << std::endl;
		}

		log << std::endl << "----------------------------------------------------------------------------------------------" << std::endl;
		for (const auto& i : participants_) {
			if (i->isArmyLive()) {
				if (i->getName().compare("allies") == 0) {
					log << "Allies";
				}
				if (i->getName().compare("axis") == 0) {
					log << "Axis";
				}
				log << " won." << std::endl << "Remaining tanks:" << std::endl;
				log << i->getRemainingWeapons();
			}
		}
		log << std::endl;
	};
};

