#include "Game.h"
#include <iterator>
#include <vector>

// -------------------- Object class ----------------------

Object::Object(string name, int value) {
	try { //If the object value is below 0, throw an error
		if (value < 0) {
			throw std::invalid_argument("Input value must not be negative");
		} else {
			name_ = name;
			value_ = value;
			owner_ = nullptr;
		}
	} 
	catch (const std::invalid_argument &e) {
        throw std::invalid_argument(e.what());
    }
}

Object::~Object() {}

string Object::print() const {
	string output;
	output = output + "??" + ", name: " + name_ + ", value: " + std::to_string(value_);
	return output;
}

string Object::getName() const {
	return name_;
}

int Object::getValue() const {
	return value_;
}

std::ostream& operator<<(std::ostream& os, const Object& o) {
	os << o.print();
	return os;

}
// ------------------- Food class ----------------------

Food::Food(string name, int value) : Object(name, value) {}

Food::~Food() {}

string Food::print() const {
	string output;
	output = output + "Food" + ", name: " + name_ + ", value: " + std::to_string(value_);
	return output;
}

void Food::use() { 
	//Sets stamina and removes it from the inventory
	owner_->setStamina(value_);
	auto it = owner_->inventory_.begin();
	while (it != owner_->inventory_.end()) {
		if (it->get()->getName() == this->getName()) {
			it = owner_->inventory_.erase(it);
			break;
		} else {
			++it;
		}
	}
}

// ------------------- Weapon class ----------------------

Weapon::Weapon(string name, int value) : Object(name, value) {}

Weapon::~Weapon() {}

string Weapon::print() const {
	string output;
	output = output + "Weapon" + ", name: " + name_ + ", value: " + std::to_string(value_);
	return output;
}

void Weapon::use() {
	(*owner_).setWeapon(this);
}

// ------------------- Armour class ----------------------

Armour::Armour(string name, int value) : Object(name, value) {}

Armour::~Armour() {}

string Armour::print() const {
	string output;
	output = output + "Armour" + ", name: " + name_ + ", value: " + std::to_string(value_);
	return output;
}

void Armour::use() {
	(*owner_).setArmour(this);
}

// ------------------- Player class ----------------------

Player::Player(string name) : name_(name), health_(100), stamina_(100), weapon_in_use_(nullptr), more_than_one_armour_(false) {}

Player::~Player() {}

string Player::getName() const {
	return name_;
}

int Player::getHealth() const {
	return health_;
}

int Player::getStamina() const {
	return stamina_; 
}

string Player::print() const {
	string output_;
	output_ += "Name: " + getName();
	output_ += "\n Type: ??";
	output_ += "\n Health: " + std::to_string(getHealth());
	output_ += "\n Stamina: " + std::to_string(getStamina());
	output_ += "\n" + getInventory();
	return output_;
}

void Player::pickup(std::unique_ptr<Object> o) {
	//Puts the object into inventory and sets an owner to it.
	//Throws an error if player is dead
	try {
		if (getHealth() <= 0) {
			throw std::logic_error("Stop! " + getName() + " is already dead.");
		}
	} catch(const std::logic_error& e) {
		throw std::logic_error(e.what());
	}
	o.get()->owner_ = this;
	inventory_.push_back(std::move(o));
}

string Player::getInventory() const {
	//Returns a list of items by looping through the inventory
	string output_ = "List of items:";
	if (inventory_.size() == 0) {
		output_ += " none";
	} else {
		for(int i = 0; i < inventory_.size(); i++) {
			output_ = output_ + "\n " + (*inventory_[i]).print();
		}
	}
	return output_; // dummy
}

bool Player::use(string name) {
	try { //Throws an error if player is dead
		if (getHealth() <= 0) {
			throw std::logic_error("Stop! " + getName() + " is already dead.");
		}
	} catch(const std::logic_error& e) {
		throw std::logic_error(e.what());
	}
	auto it = inventory_.begin(); //Loops through inventory and uses the item if found
	while (it != inventory_.end()) {
		if (it->get()->getName() == name) {
			it->get()->use();
			return true;
		} else {
			++it;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
	os << p.print();
	return os;
}

void Player::takeDamage(int damage) {
	health_ -= damage;
	if (health_ < 0) {
		health_ = 0;
	}
}

void Player::healAmount(int healingAmount) {
	health_ += healingAmount;
	if (health_ > 100) {
		health_ = 100;
	}
}

void Player::setStamina(int staminaAmount) {
	if (stamina_ < 100) {
		stamina_ += staminaAmount;
	}
	if (stamina_ > 100) {
		stamina_ = 100;
	}
}

string Player::getWeapon() const {
	if (weapon_in_use_ == nullptr) {
		return "none";
	} else {
		return weapon_in_use_->getName();
	}
}

void Player::setWeapon(Weapon* weapon) {
	weapon_in_use_ = weapon;
}

string Player::getArmour() const {
	if (armour_in_use_.size() == 0) {
		return "none";
	} else { //Loops through the armour_in_use_ vector and adds all the names to a list
		string output_;
		output_ += armour_in_use_[0]->getName();
		for (int i = 1; i < armour_in_use_.size(); i++) {
			output_ += ", " + armour_in_use_[i]->getName();
		}
		return output_;
	}
}

void Player::setArmour(Armour* armour) {
	armour_in_use_.push_back(armour);
}

// ------------------- Fighter class ----------------------

Fighter::Fighter(string name) : Player(name) {}

Fighter::~Fighter() {}

std::vector<Armour*> Player::getArmourVector() const {
	return armour_in_use_;
}

bool Fighter::attack(Player& other) {
	try { //Throws an error if either player is dead
		if (getHealth() <= 0) {
			throw std::logic_error("Stop! " + getName() + " is already dead.");
		}
		if (other.getHealth() <= 0) {
			throw std::logic_error("Stop! " + other.getName() + " is already dead.");
		}
	} catch(const std::logic_error& e) {
		throw std::logic_error(e.what());
	}
	if (stamina_ < 10) {
		return false;
	}
	stamina_ -= 10;
	int dmg = 10;
	if (weapon_in_use_ != nullptr) {
		dmg += weapon_in_use_->getValue();
	}
	int def = 0;
	for (int i = 0; i < other.getArmourVector().size(); i++) { //Loop throug the other player's armour_in_use_ to add up defense
		def += other.getArmourVector()[i]->getValue();
	}
	if (dmg > def) {
		other.takeDamage(dmg-def);
		return true;
	}
	return false; // dummy
}

string Fighter::print() const {
	string output_;
	output_ += "Name: " + getName();
	output_ += "\nType: Fighter";
	output_ += "\nHealth: " + std::to_string(getHealth());
	output_ += "\nStamina: " + std::to_string(getStamina());
	output_ += "\n" + getInventory();
	output_ += "\nWeapon in use: " + getWeapon();
	output_ += "\nArmour in use: " + getArmour();
	return output_; // dummy
}

// ------------------- Healer class ----------------------

Healer::Healer(string name) : Player(name) {}

Healer::~Healer() {}

bool Healer::heal(Player& other) {
	try { //Throws an error if either player is dead
		if (getHealth() <= 0) {
			throw std::logic_error("Stop! " + getName() + " is already dead.");
		}
		if (other.getHealth() <= 0) {
			throw std::logic_error("Stop! " + other.getName() + " is already dead.");
		}
	} catch(const std::logic_error& e) {
		throw std::logic_error(e.what());
	}
	if (stamina_ < 10) {
		return false;
	}
	stamina_ -= 10;
	if (other.getHealth() < 100) {
		other.healAmount(20);
	} else {
		return false;
	}
	return true;
}

string Healer::print() const {
	string output_;
	output_ += "Name: " + getName();
	output_ += "\nType: Healer";
	output_ += "\nHealth: " + std::to_string(getHealth());
	output_ += "\nStamina: " + std::to_string(getStamina());
	output_ += "\n" + getInventory();
	output_ += "\nArmour in use: " + getArmour();
	return output_;
}