#include <iostream>
#include <string>
#include <conio.h>
#include <random>

class Dice
{
public:
	int Roll( int nDice )
	{
		int total = 0;
		for( int n = 0; n < nDice; n++ )
		{
			total += d6( rng );
		}
		return total;
	}
private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>( 1,6 );
	std::mt19937 rng = std::mt19937( std::random_device{}() );
};

class MemeFighter
{
public:
	int GetInitiative()
	{
		return speed + die.Roll(2);
	}
	bool IsAlive() const
	{
		return hp > 0;
	}
	void Punch(MemeFighter& target)
	{
		if (IsAlive() && target.IsAlive())
		{
			std::cout << name << " punched " << target.GetName() << "!" << std::endl;
			ApplyDamageTo(target, power + die.Roll(2));
		}
		else
		{
			std::cout << target.GetName() << "'s already dead" << std::endl;
		}
	}
	void Tick()
	{
		if (IsAlive())
		{
			hp += die.Roll(1);
		}
	}
	const std::string& GetName() const
	{
		return name;
	}

protected:
	MemeFighter(const std::string& name, int hp, int speed, int power)
		:
		name(name),
		hp(hp),
		speed(speed),
		power(power)
	{
	}
	void ApplyDamageTo(MemeFighter& target, int damage)
	{
		target.hp -= damage;
	}
protected:
	int hp;
	int speed;
	int power;
	std::string name;
private:
	Dice die;
};

class MemeFrog : public MemeFighter
{
public:
	MemeFrog(const std::string& name)
		:
		MemeFighter(name, 69, 7, 14)
	{
	}
	void SpecialMove(MemeFighter& target)
	{
		if (IsAlive() && target.IsAlive())
		{
			int choice = d3(rng);
			if (choice == 1)
			{
				ApplyDamageTo(target, 20 + die.Roll(3));
				std::cout << GetName() << " made a special move to " << target.GetName() << "!" << std::endl;
			}
			std::cout << GetName() << " has " << hp << "." << std::endl;
		}
	}
	void Tick()
	{
		if (IsAlive())
		{
			ApplyDamageTo(*this, die.Roll(1));
			MemeFighter::Tick();
		}
	}
private:
	std::uniform_int_distribution<int> d3 = std::uniform_int_distribution<int>(1, 3);
	std::mt19937 rng = std::mt19937(std::random_device{}());
	Dice die;
};

class MemeStoner : public MemeFighter
{
public:
	MemeStoner(const std::string& name)
		:
		MemeFighter(name, 80, 4, 10)
	{
	}
	void SpecialMove()
	{
		if (IsAlive())
		{
			int choice = d2(rng);
			if (choice == 1)
			{
				speed += 3;
				power += (69 / 42);
				hp += 10;
				name = "Super " + name;
				std::cout << GetName() << " made a special move!" << std::endl;
			}
			std::cout << GetName() << " has " << hp << "." << std::endl;
		}
	}
private:
	std::uniform_int_distribution<int> d2 = std::uniform_int_distribution<int>(1, 2);
	std::mt19937 rng = std::mt19937(std::random_device{}());
	Dice die;
};

void Engage( MemeFighter& f1,MemeFighter& f2 )
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if( p1->GetInitiative() < p2->GetInitiative() )
	{
		std::swap( p1,p2 );
	}
	// execute attacks
	p1->Punch( *p2 );
	p2->Punch( *p1 );
}

int main()
{
	MemeFrog f1( "Dat Boi" );
	MemeStoner f2( "Good Guy Greg" );

	while( f1.IsAlive() && f2.IsAlive() )
	{
		// trade blows
		Engage( f1,f2 );
		// special moves
		f2.SpecialMove();
		f1.SpecialMove( f2 );
		// end of turn maintainence
		f1.Tick();
		f2.Tick();

		std::cout << "Press any key to continue...";
		while( !_kbhit() );
		_getch();
		std::cout << std::endl << std::endl;
	}

	if( f1.IsAlive() )
	{
		std::cout << f1.GetName() << " is victorious!";
	}
	else
	{
		std::cout << f2.GetName() << " is victorious!";
	}
	while( !_kbhit() );
	return 0;
}