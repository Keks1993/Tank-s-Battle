#pragma once
#include <assert.h>
#include <random>
#include <time.h>
#include <map>


std::mt19937 gen(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());


enum class Weapon_id { LightTank = 0, MediumTank, HeavyTank, TankDestorer, Locomotive };
enum class Army_id { Allies = 0, Axis };

const std::map<std::string, Weapon_id> weapon_names{
	{"light_tank", Weapon_id::LightTank},
	{"medium_tank", Weapon_id::MediumTank},
	{"heavy_tank", Weapon_id::HeavyTank},
	{"tank_destroyer", Weapon_id::TankDestorer},
	{"locomotive_artillery", Weapon_id::Locomotive}
};

const std::map<std::string, Army_id> player_names{
	{"allies", Army_id::Allies},
	{"axis", Army_id::Axis}
};

std::string output_weapon_names[] = { "Light tank", "Medium tank", "Heavy tank", "Tank destroyer", "Locomotive artillery" };



//abstract product
class Weapon {
public:
	virtual ~Weapon() = default;
	virtual std::string getID() const = 0;
	virtual bool receiveDamage(const int32_t value) = 0;
	virtual bool shoot(std::shared_ptr<Weapon> target) = 0;
	virtual Weapon_id getType() const = 0;
	virtual uint32_t getDamage() const = 0;
	virtual int getStrength() const = 0;
	virtual double getDodge() const = 0;
};


//common weapon properties
struct Props {
	uint32_t id = 0;
	Weapon_id type;

	int32_t strength = 0;
	uint32_t damage = 0;
	double accuracy = 0;
	double dodge = 0;

	Weapon_id get_type() const {
		return type;
	};

	uint32_t get_damage() const {
		return damage;
	};

	int get_strength() const {
		return strength;
	};

	double get_dodge() const {
		return dodge;
	};
};


class Tank : public Weapon, public Props {
public:
	Tank(const Props& p) : Props(p) {};
	virtual ~Tank() = default;

	virtual uint32_t getDamage() const override {
		return get_damage();
	};

	virtual int getStrength() const override {
		return get_strength();
	};

	virtual double getDodge() const override {
		return get_dodge();
	};

	virtual bool receiveDamage(const int32_t value) override {
		strength = (strength - value < 0) ? 0 : (strength - value);
		if (strength == 0) {
			//critical damage
			return true;
		}
		return false;
	}

	virtual bool shoot(std::shared_ptr<Weapon> target) override {
		//P(hit) = P(accuracy) * P(undodge) = P(accuracy) * (1-P(dodge))
		//so, lets we will generate random number in range [0,1] and if number <= hit_probability then hit is occured
		const double hit_probability = accuracy * (1 - target->getDodge());
		if (static_cast<double>(gen()) / std::numeric_limits<uint32_t>::max() <= hit_probability) {
			target->receiveDamage(damage);
			return true;
		}
		return false;
	};
};


class Artillery : public Weapon, public Props {
public:
	Artillery(const Props& p) : Props(p) {};
	virtual ~Artillery() = default;

	virtual uint32_t getDamage() const override {
		return get_damage();
	};

	virtual int getStrength() const override {
		return get_strength();
	};

	virtual double getDodge() const override {
		return get_dodge();
	};

	virtual bool receiveDamage(const int32_t value) override {
		strength = (strength - value < 0) ? 0 : (strength - value);
		if (strength == 0) {
			//critical damage
			return true;
		}
		return false;
	}

	virtual bool shoot(std::shared_ptr<Weapon> target) override {
		//P(hit) = P(accuracy) * P(undodge) = P(accuracy) * (1-P(dodge))
		//so, lets we will generate random number in range [0,1] and if number <= hit_probability then hit is occured
		const double hit_probability = accuracy * (1 - target->getDodge());
		if (static_cast<double>(gen()) / std::numeric_limits<uint32_t>::max() <= hit_probability) {
			target->receiveDamage(damage);
			return true;
		}
		return false;
	};
};

class LightTank : public Tank{
	//specific properties for LightTank
	//...
public:
	LightTank(const Props& p) : Tank(p) {};
	virtual ~LightTank() = default;

	virtual std::string getID() const override {
		return output_weapon_names[static_cast<int>(Weapon_id::LightTank)] + "(" + std::to_string(id) + ")";
	};

	virtual Weapon_id getType() const override {
		return Weapon_id::LightTank;
	};
};

class MediumTank : public Tank {
	//specific properties for MediumTank
	//...
public:
	MediumTank(const Props& p) : Tank(p) {};
	virtual ~MediumTank() = default;

	virtual std::string getID() const override { 
		return output_weapon_names[static_cast<int>(Weapon_id::MediumTank)] + "(" + std::to_string(id) + ")";
	};

	virtual Weapon_id getType() const override {
		return Weapon_id::MediumTank;
	};
};

class HeavyTank : public Tank {
	//specific properties for MediumTank
	//...
public:
	HeavyTank(const Props& p) :Tank(p) {};
	virtual ~HeavyTank() = default;

	virtual std::string getID() const override {
		return output_weapon_names[static_cast<int>(Weapon_id::HeavyTank)] + "(" + std::to_string(id) + ")"; 
	};

	virtual Weapon_id getType() const override {
		return Weapon_id::HeavyTank;
	};
};


class TankDestroyer : public Artillery {
	//specific properties for TankDestroyer
	//...
public:
	TankDestroyer(const Props& p) : Artillery(p) {};
	virtual ~TankDestroyer() = default;
	
	virtual std::string getID() const override {
		return output_weapon_names[static_cast<int>(Weapon_id::TankDestorer)] + "(" + std::to_string(id) + ")"; 
	};

	virtual Weapon_id getType() const override {
		return Weapon_id::TankDestorer;
	};
};

class LocomotiveArtillery : public Artillery {
	//specific properties for LocomotiveArtillery
	//...
public:
	LocomotiveArtillery(const Props& p) : Artillery(p) {};
	virtual ~LocomotiveArtillery() = default;
	
	virtual std::string getID() const override {
		return output_weapon_names[static_cast<int>(Weapon_id::Locomotive)] + "(" + std::to_string(id) + ")"; 
	};
	
	virtual Weapon_id getType() const override {
		return Weapon_id::Locomotive;
	};
};


//specific Allies products
class AlliesLightTank final : public LightTank {
	//specific properties for AlliesLightTank
	//...
public:
	AlliesLightTank(const Props& p) : LightTank(p) {};
};

class AxisLightTank final : public LightTank {
	//specific properties for AlliesLightTank
	//...
public:
	AxisLightTank(const Props& p) : LightTank(p) {};
};

class AlliesMediumTank final : public MediumTank {
	//specific properties for AlliesMediumTank
	//...
public:
	AlliesMediumTank(const Props& p) : MediumTank(p) {};
};

class AxisMediumTank final : public MediumTank {
	//specific properties for AxisMediumTank
	//...
public:
	AxisMediumTank(const Props& p) : MediumTank(p) {};
};

class AlliesHeavyTank final : public HeavyTank {
	//specific properties for AlliesHeavyTank
	//...
public:
	AlliesHeavyTank(const Props& p) : HeavyTank(p) {};
};

class AxisHeavyTank final : public HeavyTank {
	//specific properties for AxisHeavyTank
	//...
public:
	AxisHeavyTank(const Props& p) : HeavyTank(p) {};
};

class AlliesTankDestroyer final : public TankDestroyer {
	//specific properties for AlliesTankDestroyer
	//...
public:
	AlliesTankDestroyer(const Props& p) : TankDestroyer(p) {};
};


//specific Axis products
class AxisTankDestroyer final : public TankDestroyer {
	//specific properties for AxisTankDestroyer
	//...
public:
	AxisTankDestroyer(const Props& p) : TankDestroyer(p) {};
};

class AlliesLocomotiveArtillery final : public LocomotiveArtillery {
	//specific properties for AlliesLocomotiveArtillery
	//...
public:
	AlliesLocomotiveArtillery(const Props& p) : LocomotiveArtillery(p) {};
};

class AxisLocomotiveArtillery final : public LocomotiveArtillery {

public:
	AxisLocomotiveArtillery(const Props& p) : LocomotiveArtillery(p) {};
};


//abstract factory
class WeaponsFactory {
public:
	virtual std::unique_ptr<Weapon> createWeapon(const Weapon_id weapon_id, const Props& prop) const = 0;
	virtual ~WeaponsFactory() = default;
};


//specific Allies factory
class AlliesWeaponsFactory final : public WeaponsFactory {
public:
	std::unique_ptr<Weapon> createWeapon(const Weapon_id id, const Props& prop) const override {
		switch (id) {
		case Weapon_id::LightTank:
			return std::make_unique<AlliesLightTank>(prop);
			break;
		case Weapon_id::MediumTank:
			return std::make_unique<AlliesMediumTank>(prop);
			break;
		case Weapon_id::HeavyTank:
			return std::make_unique<AlliesHeavyTank>(prop);
			break;
		case Weapon_id::TankDestorer:
			return std::make_unique<AlliesTankDestroyer>(prop);
			break;
		case Weapon_id::Locomotive:
			return std::make_unique<AlliesLocomotiveArtillery>(prop);
			break;
		default:
			assert(false);
		}
	}
};

//specific Axis factory
class AxisWeaponsFactory final : public WeaponsFactory {
public:
	std::unique_ptr<Weapon> createWeapon(const Weapon_id id, const Props& prop) const override {
		switch (id) {
		case Weapon_id::LightTank:
			return std::make_unique<AxisLightTank>(prop);
			break;
		case Weapon_id::MediumTank:
			return std::make_unique<AxisMediumTank>(prop);
			break;
		case Weapon_id::HeavyTank:
			return std::make_unique<AxisHeavyTank>(prop);
			break;
		case Weapon_id::TankDestorer:
			return std::make_unique<AxisTankDestroyer>(prop);
			break;
		case Weapon_id::Locomotive:
			return std::make_unique<AxisLocomotiveArtillery>(prop);
			break;
		default:
			assert(false);
		}
	}
};