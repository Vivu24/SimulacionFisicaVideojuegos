#pragma once
#include "ForceGenerator.h"


class GravityGenerator : public ForceGenerator
{
private:
	double gravity_;
public:
	GravityGenerator(double g, double t);

	Vector3 applyForce(Particle* p);
	void update(double t) {
		cout << "UPdate" << endl;
	};
};

