#include "vector.h"

#define WALL_GRID_H 150
#define WALL_GRID_W 225
#define W_SIZE_H 480
#define W_SIZE_W 720
#define CELL_SIZE W_SIZE_W/WALL_GRID_W
#define DRAW_PHERO_SIZE 1.f
#define ANT_SPEED 1.f
#define FOOD_SIZE 5.f
#define ROT_SPEED 0.1f
#define NB_PHERO_T 2
#define ANT_SIZE 3.f
#define PHERO_LEAVE_DUR 10.f
#define ANT_TUNR_DUR 0.1f
#define EVAP_SPEED 0.001f
#define ANT_PERCEPTION_RADIUS 20.f
#define NEST_SIZE 30.f
#define PHERO_CONS 0.004f

enum PheroType
{
	S_FOOD_N = 0,
	F_FOOD_N = 1
};

struct Phero
{
	int pheroType = S_FOOD_N;
	Vector pos = vec_v(0.f,0.f);
	float strength = 1.f;
} typedef Phero;

struct Ant
{
	int id = 0;
	Vector pos = vec_v(0.f,0.f);
	float angle = 0.f;
	float targetAngle = 0.f;
	bool holdFood = false;
	float lastPheroDur = 0.f;
	float lastTurnDur = 0.f;
	float pheroFuel = 1.f;
} typedef Ant;

void add_angle(Ant* a, float angle);

class AntManager
{
public:
	AntManager(int foodClusterNB_, int foodClusterSize_);
	void init(int antsNB_);
	void update(float dt);
	void update_ant(Ant* a, float dt);
	void move_ant(Ant* a, float dt);
	void spawn_phero(Vector pos, int pheroType, float initStrenght);
	void leave_phermone(Ant* a, float dt);
	void grab_food(Ant* a);
	void evaporate(float dt);
	void sense_pheros(Ant* a, int pheroType);
	void sense(Ant* a);
	void sense_food(Ant* a);
	void sense_nest(Ant* a);
	bool is_valid_ant_pos(Vector pos);
	Vector world_to_grid_pos(Vector wpos);
	void make_held_food(Ant* a);

	std::vector<Phero> pheros[NB_PHERO_T];
	bool walls[WALL_GRID_W][WALL_GRID_H];
	int pherosNB[NB_PHERO_T];
	std::vector<Ant> ants;
	int antsNB;
	std::vector<Vector> food;
	int foodNB;
	int tick = 0;
	int foodInNestNB = 0;
	float lastFoodGorDur = 0.f;
	// std::vector<*Phero> pherosMap;
	
	Vector worldSize = vec_v(W_SIZE_W, W_SIZE_H);
	Vector gridSize = vec_v(WALL_GRID_W, WALL_GRID_H);
	Vector nestPos = worldSize - worldSize/5.f;
};