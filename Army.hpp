#pragma once
#include "Weapons.hpp"

class Army {
	//At the begining of battle units_ constains alive weapons.
	//We dont want to remove destroed weapons from units_.
	//Instead this, we will use additional container with alive weapons numbers - alive_units_order_numbers_ 
	//Lets we will store destroyed weapons at the left side of alive_units_order_numbers_ and alive weapons - at the right side.
	//To do this we will use swap().
	//So, alive_units_order_numbers_ will be look like this
	//	|d|d|d|d| | | | | | | |,
	//			k
	//where k - begin_of_alive_units_.
	//In future this approach will permit to create new functions like repair_destroed_weapons() or call_partners() ...

	const std::string name_;
	std::vector<std::shared_ptr<Weapon>> units_;
	std::vector<uint32_t> alive_units_order_numbers_;
	uint32_t begin_of_alive_units_{ 0 };
	uint32_t army_size_{ 0 };
	mutable uint32_t pos_{ 0 };

public:
	Army(const WeaponsFactory* factory, const std::string& name, const pt::ptree& subtree) : name_{ name } {
		if (factory != nullptr) {
			static uint32_t id{ 0 };
			for (const auto& i : subtree.get_child("")) {
				auto it = weapon_names.find(i.first);
				if (it != weapon_names.end()) {
					for (auto j{ 0 }; j < i.second.get<uint32_t>("count"); ++j) {
						units_.emplace_back(factory->createWeapon((*it).second,
							{ id++,
								(*it).second,
								i.second.get<int32_t>("strength"),
								i.second.get<uint32_t>("damage"),
								i.second.get<double>("accuracy"),
								i.second.get<double>("dodge")
							})
						);
					}
				}
				else {
					std::cout << "weapon " << i.first << " is not supported yet" << std::endl;
				}
			}

			army_size_ = units_.size();
			for (auto j{ 0 }; j < army_size_; ++j) {
				alive_units_order_numbers_.push_back(j);
			}
		}
	};

	bool isArmyLive() const {
		return begin_of_alive_units_ != army_size_;
	};

	void destroyWeapon() {
		std::swap(alive_units_order_numbers_.at(pos_), alive_units_order_numbers_.at(begin_of_alive_units_));
		begin_of_alive_units_++;
	};

	std::shared_ptr<Weapon> chooseWeapon(std::mt19937& gen) const {
		std::uniform_int_distribution<size_t> dis(begin_of_alive_units_, army_size_ - 1);
		pos_ = dis(gen);
		return units_.at(alive_units_order_numbers_.at(pos_));
	}

	std::string getRemainingWeapons() const {
		std::unordered_map < std::string, uint32_t > rem;

		for (const auto& i : output_weapon_names) {
			rem.insert({ i, 0 });
		}

		for (const auto& it : units_) {
			if (it->getStrength() != 0) {
				rem[output_weapon_names[static_cast<int>(it->getType())]]++;
			}
		}

		std::string stream;
		for (const auto& it : rem) {
			stream += it.first + ": " + std::to_string(it.second) + "\n";
		}

		return stream;
	}

	std::string getName() const {
		return name_;
	};
};