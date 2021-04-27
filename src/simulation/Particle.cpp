#include <cstddef>
#include "Particle.h"

std::vector<StructProperty> const &Particle::GetProperties()
{
	static std::vector<StructProperty> properties = {
		{ "type"   , StructProperty::ParticleType, (intptr_t)(offsetof(Particle, type   )) },
		{ "life"   , StructProperty::ParticleType, (intptr_t)(offsetof(Particle, life   )) },
		{ "ctype"  , StructProperty::ParticleType, (intptr_t)(offsetof(Particle, ctype  )) },
		{ "x"      , StructProperty::Float       , (intptr_t)(offsetof(Particle, x      )) },
		{ "y"      , StructProperty::Float       , (intptr_t)(offsetof(Particle, y      )) },
		{ "vx"     , StructProperty::Float       , (intptr_t)(offsetof(Particle, vx     )) },
		{ "vy"     , StructProperty::Float       , (intptr_t)(offsetof(Particle, vy     )) },
		{ "temp"   , StructProperty::Float       , (intptr_t)(offsetof(Particle, temp   )) },
		{ "flags"  , StructProperty::UInteger    , (intptr_t)(offsetof(Particle, flags  )) },
		{ "tmp"    , StructProperty::Integer     , (intptr_t)(offsetof(Particle, tmp    )) },
		{ "tmp2"   , StructProperty::Integer     , (intptr_t)(offsetof(Particle, tmp2   )) },
		{ "tmp3"   , StructProperty::Integer     , (intptr_t)(offsetof(Particle, tmp3   )) },
		{ "tmp4"   , StructProperty::Integer	 , (intptr_t)(offsetof(Particle, tmp4	)) },
		{ "tmpcity0"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[0])) },
		{ "tmpcity1"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[1])) },
		{ "tmpcity2"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[2])) },
		{ "tmpcity3"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[3])) },
		{ "tmpcity4"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[4])) },
		{ "tmpcity5"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[5])) },
		{ "tmpcity6"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[6])) },
		{ "tmpcity7"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[7])) },
		{ "tmpcity8"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[8])) },
		{ "tmpcity9"  , StructProperty::Float    , (intptr_t)(offsetof(Particle, tmpcity[9])) },
		{ "carbons"  , StructProperty::Integer	 , (intptr_t)(offsetof(Particle, carbons)) },
		{ "hydrogens"  , StructProperty::Integer, (intptr_t)(offsetof(Particle, hydrogens))},
		{ "oxygens"  , StructProperty::Integer	 , (intptr_t)(offsetof(Particle, oxygens)) },
		{ "water"   , StructProperty::Integer	 , (intptr_t)(offsetof(Particle, water)) },
		{ "dcolour", StructProperty::UInteger    , (intptr_t)(offsetof(Particle, dcolour)) },
		{ "pavg0"  , StructProperty::Float       , (intptr_t)(offsetof(Particle, pavg[0])) },
		{ "pavg1"  , StructProperty::Float       , (intptr_t)(offsetof(Particle, pavg[1])) },
	};
	return properties;
}
