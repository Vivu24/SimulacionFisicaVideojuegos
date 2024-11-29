
#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const PxVec3& anchor_pos) :
	SpringForceGenerator(k, resting, nullptr)
{
	_other = new Particle(anchor_pos, PxVec3(0, 0, 0), PxVec3(0,0,0), 5, 50);
}

AnchoredSpringFG::~AnchoredSpringFG()
{
	delete _other;
}
