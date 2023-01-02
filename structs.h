#define KEY_NUMPAD0 0
#define KEY_NUMPAD1 1
#define KEY_NUMPAD2 2
#define KEY_NUMPAD3 3

struct Position {
	float x;
	float y;
	float z;
};

struct Angle {
	float yaw;
	float pitch;
};

struct player_entity {
	Position pos;
	Angle angle;

	unsigned int hp;
	unsigned int armor;

	unsigned int sweap_sec;
	unsigned int pweap_sec;

	unsigned int sWeapon;
	unsigned int pWeapon;
};
