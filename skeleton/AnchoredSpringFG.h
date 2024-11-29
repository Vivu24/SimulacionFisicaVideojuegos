#pragma once
#include "SpringForceGenerator.h"
class AnchoredSpringFG : public SpringForceGenerator
{
public:
	AnchoredSpringFG(float k, float resting, const PxVec3& anchor_pos)
		: SpringForceGenerator(k, resting, nullptr) {
		_other = new Particle(anchor_pos, { 0,0,0 }, { 0,0,0 }, { 1,0,0,1 }, BOX);
		_other->setMass(1e6);
	}

	~AnchoredSpringFG() {
		delete _other;
	}
	inline void setAnchorLife(float t) {
		_other->setLifeTime(t);
	}

};

