#include "ant.h"


void add_angle(Ant* a, float angle)
{
	a->angle = mod_2pi(a->angle + angle);
	a->targetAngle = a->angle;
}

bool AntManager::is_valid_ant_pos(Vector pos)
{
	if (!vec_in_rect(pos, vec_v(0,0), worldSize))
	{
		return false;
	}
	Vector gPos = world_to_grid_pos(pos);
	if (walls[(int)gPos.x][(int)gPos.y] == true)
	{
		return false;
	}

	return true;
}

Vector AntManager::world_to_grid_pos(Vector wpos)
{
	return floor_v(wpos/worldSize * gridSize);
}

AntManager::AntManager(int foodClusterNB_, int foodClusterSize_)
{
	foodNB = foodClusterSize_ * foodClusterNB_;
	for (int i = 0; i < foodClusterNB_; ++i)
	{
		Vector clusterPos;
		clusterPos.x = rand_float(false) * (float)W_SIZE_W;
		clusterPos.y = rand_float(false) * (float)W_SIZE_H;
		for (int i = 0; i < foodClusterSize_; ++i)
		{
			Vector fPos = clusterPos + vec_v(rand_float(true), rand_float(true)) * ((float)W_SIZE_W/30.f);
			food.push_back(fPos);
		}
	}
	for (int x = 0; x < WALL_GRID_W; ++x)
	{
		for (int y = 0; y < WALL_GRID_H; ++y)
		{
			walls[x][y] = 0;
		}
	}
	for (int i = 0; i < NB_PHERO_T; ++i)
	{
		pherosNB[i] = 0;
	}
}

void AntManager::init(int antsNB_)
{
	antsNB = antsNB_;
	for (int i = 0; i < antsNB; ++i)
	{
		float angle = rand_float(false) * 2.f * PI;
		Ant a = {i, nestPos, angle, angle};
		ants.push_back(a);
	}
}

void AntManager::update(float dt)
{
	for (int i = 0; i < antsNB; ++i)
	{
		update_ant(&ants[i], dt);
	}
	evaporate(dt);
	tick++;
}

void AntManager::update_ant(Ant* a, float dt)
{
	leave_phermone(a, dt);
	move_ant(a, dt);
	grab_food(a);
}

void AntManager::move_ant(Ant* a, float dt)
{
	if (dist_v(a->pos, nestPos) < NEST_SIZE)
	{
		if (a->holdFood)
		{
			foodInNestNB++;
			a->holdFood = false;
			add_angle(a, PI);
		}
		a->pheroFuel = 1.f;
	}

	a->lastTurnDur += dt;
	if (a->lastTurnDur > ANT_TUNR_DUR)
	{
		a->lastTurnDur = 0.f;
		a->targetAngle += rand_float(true) * PI * 0.1f;
		a->targetAngle = mod_2pi(a->targetAngle);
		sense(a);
		sense_nest(a);
		sense_food(a);
		Vector dir = vec_from_angle_v(a->angle);
		Vector targetDir = vec_from_angle_v(a->targetAngle);
		Vector dirNormal = vec_v(-dir.y, dir.x);
		float dirDiff = dot_v(targetDir, dirNormal);
		a->angle += ROT_SPEED * dirDiff * dt;
	}

	Vector oldPos = a->pos + vec_from_angle_v(a->angle) * (ANT_SPEED * dt);
	if (is_valid_ant_pos(oldPos))
	{
		a->pos = oldPos;
	}
	else
	{
		add_angle(a, PI * rand_float(true));
	}
}

void AntManager::sense(Ant* a)
{
	if (a->holdFood)
	{
		sense_pheros(a, S_FOOD_N);
	}
	else
	{
		sense_pheros(a, F_FOOD_N);
	}
}

void AntManager::spawn_phero(Vector pos, int pheroType, float initStrenght)
{
	Phero p = {pheroType, pos, initStrenght};
	pheros[pheroType].push_back(p);
	pherosNB[pheroType]++;
}

void AntManager::leave_phermone(Ant* a, float dt)
{
	a->lastPheroDur += dt;
	if (a->lastPheroDur > PHERO_LEAVE_DUR)
	{
		a->lastPheroDur = 0.f;
	}
	else
	{
		return;
	}
	a->pheroFuel -= PHERO_CONS * dt;
	if (a->pheroFuel <= 0)
	{
		return;
	}
	if (a->holdFood)
	{
		spawn_phero(a->pos, F_FOOD_N, 1.f);
	}
	else
	{
		spawn_phero(a->pos, S_FOOD_N, 1.f);
	}
}

void AntManager::grab_food(Ant* a)
{
	if (a->holdFood)
	{
		return;
	}
	for (int i = 0; i < foodNB;)
	{
		if (a->holdFood == false && dist_v(food[i], a->pos) < ANT_SIZE + FOOD_SIZE)
		{
			food.erase(food.begin() + i);
			foodNB--;
			make_held_food(a);
			add_angle(a, PI);
			a->pheroFuel = 1.f;
		}
		else
		{
			i++;
		}
	}
}
void AntManager::make_held_food(Ant* a)
{
	if (a->holdFood == false)
	{
		a->holdFood = true;
	}
}

void AntManager::evaporate(float dt)
{
	for (int i = 0; i < NB_PHERO_T; ++i)
	{
		for (int j = 0; j < pherosNB[i];)
		{
			pheros[i][j].strength -= EVAP_SPEED * dt;
			// printf("%f\n", pheros[i][j].strength);
			if (pheros[i][j].strength <= 0.f)
			{
				pheros[i].erase(pheros[i].begin() + j);
				pherosNB[i]--;
			}
			else
			{
				j++;
			}
		}
	}
}

void AntManager::sense_food(Ant* a)
{
	if (a->holdFood)
	{
		return;
	}
	for (int i = 0; i < foodNB; ++i)
	{
		if (dist_v(food[i], a->pos) < ANT_PERCEPTION_RADIUS)
		{
			a->targetAngle = get_angle_v(food[i] - a->pos);
			return;
		}
	}
}

void AntManager::sense_nest(Ant* a)
{
	if (!a->holdFood)
	{
		return;
	}
	if (dist_v(a->pos, nestPos) < ANT_PERCEPTION_RADIUS + NEST_SIZE)
	{
		a->targetAngle = get_angle_v(nestPos - a->pos);
	}
}


void AntManager::sense_pheros(Ant* a, int pheroType)
{
	float totStrength = 0.f;
	Vector finalPos = vec_v(0.f,0.f);
	Vector dir = vec_from_angle_v(a->angle);
	for (int i = 0; i < pherosNB[pheroType]; ++i)
	{
		Vector aToPhero = pheros[pheroType][i].pos - a->pos;
		float aToPheroNorm = get_norm_v(aToPhero);
		if ((aToPheroNorm < ANT_PERCEPTION_RADIUS) and dot_v(aToPhero, dir) > 0.f)
		{
			totStrength += pheros[pheroType][i].strength;
			finalPos = finalPos + pheros[pheroType][i].pos * pheros[pheroType][i].strength;
		}
	}
	if (totStrength > 0.f)
	{
		a->targetAngle = get_angle_v(finalPos/totStrength - a->pos);
	}
}