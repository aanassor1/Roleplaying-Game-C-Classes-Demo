#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <string>
#include <memory>
#include <vector>
using std::string;

class Player; // forward declaration so that Object can refer to Player

// ------------------ Object and its subclasses -------------------

class Object {
public:
	// Constructor, specifying the name and the value of an object.
	Object(string name, int value);
		
	// Destructor
	virtual ~Object();

	// Return the name of the object.
	string getName() const;

	// Return the value of the object.
	int getValue() const;

	// Return a string giving details of the object, as specified in
	// the assignment webpage.
	virtual string print() const = 0;

private:
	// Use this object.
	// This function should not be public; it only makes sense to be
	// called from within a Player object (the player who owns it)
	virtual void use() = 0;

protected:
	// You probably want to have something like this.
	// For technical reason it may be easier to just use a raw pointer
	Player* owner_;
	string name_;
	int value_;
	// TODO: add any other protected/private member variables

// Overloaded output stream redirection operator, printing the object to the
// given output stream
friend std::ostream& operator<<(std::ostream& os, const Object& o);

// You can "friend" other classes
friend class Player;
};

class Food : public Object {
public:
	// Constructor
	Food(string name, int value);

	// Destructor
	virtual ~Food() override;

	//Virtual print function specifically for food objects.
	virtual string print() const override;

	//Virtual use function specifically for food objects. Increases player's stamina on use.
	virtual void use() override;

private:
	// Add any member variables if needed
};

class Weapon : public Object {
public:
	// Constructor
	Weapon(string name, int value);

	// Destructor
	virtual ~Weapon() override;

	//Virtual print function specifically for weapon objects
	virtual string print() const override;

	//Virtual print function specifically for weapon objects. Equips the weapon to the player.
	virtual void use() override;

private:
	// Add any member variables if needed
};

class Armour : public Object {
public:		
	// Constructor
	Armour(string name, int value);

	// Destructor
	virtual ~Armour() override;

	//Virtual print function specifically for armour objects
	virtual string print() const override;

	//Virtual print function specifically for armour objects. Equips the armour to the player.
	virtual void use() override;

private:
	// Add any member variables if needed
};

// ----------------- Player and its subclasses --------------------

class Player {
public:
	// Default constructor, just to make this release version compilable.
	// If your implementation is correct this should be removed
	
	// Constructor, specifying the name of the player
	// Set the health and stamina both to 100
	Player(string name);
	
	// Destructor
	virtual ~Player();

	// Return the name of the player
	string getName() const;

	// Return the current health of the player
	int getHealth() const;

	// Return the current stamina of the player
	int getStamina() const;

	// Sets the stamina of the player to the arg.
	void setStamina(int);

	// Damages the player's health by subtracting arg to the health.
	void takeDamage(int);

	// Heals the player's health by adding arg to the heath.
	void healAmount(int);

	// Add the object pointed to by the unique pointer to the
	// inventory of objects this player carries.
	// As this is a unique pointer, it has to be "moved" into this
	// function using std::move().
	void pickup(std::unique_ptr<Object> obj);

	// Return a string listing all objects this player owns, in
	// the format specified in the assignment page
	string getInventory() const;

	// Return a string that contains all details of a player, as
	// specified in the assignment page
	virtual string print() const;

	// Use the object with the given name. If the player does not
	// own any object of this name, return false and do nothing,
	// otherwise return true.
	bool use(string name);

	// Sets weapon to be equipped to the player
	void setWeapon(Weapon*);

	// Gets current equipped weapon
	string getWeapon() const;

	// Sets armours to the currently equipped armours of the player
	void setArmour(Armour*);

	// Gets the current armours equipped
	string getArmour() const;

	// Returns a vector of armour pointers
	std::vector<Armour*> getArmourVector() const;

protected:
	// TODO: add any protected or private member variables
	string name_;
	int health_;
	int stamina_;
	std::vector<std::unique_ptr<Object>> inventory_;
	Weapon* weapon_in_use_;
	std::vector<Armour*> armour_in_use_; 
	bool more_than_one_armour_; //Determines how to print out armours
	
// Overloaded stream redirection operator, that prints the contents of
// the player to the given output stream
friend std::ostream& operator<<(std::ostream& os, const Player& p);

// You can "friend" other classes
friend void Food::use();
friend void Weapon::use();
};

class Fighter : public Player {
public:
	// Constructor
	Fighter(string name);

	// Destructor
	virtual ~Fighter() override;

	// Attack the specified player, following the rules in the
	// assignment page
	bool attack(Player& other);

	// Virtual print function that prints specifically for Fighter classes.
	virtual string print() const override;

private:
	// add any member variables if needed
};

class Healer : public Player {
public:
	// Constructor
	Healer(string name);

	// Destructor
	virtual ~Healer() override;

	// Heal the specified player, following the rules in the
	// assignment page
	bool heal(Player& other);

	// Virtual print function that prints specifically for Healer classes.
	virtual string print() const override;

private:
	// add any member variables if needed
};

#endif /* GAME_H_ */