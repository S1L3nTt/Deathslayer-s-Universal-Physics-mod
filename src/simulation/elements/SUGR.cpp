#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_SUGR() {
	Identifier = "DEFAULT_PT_SUGR";
	Name = "SUGR";
	Colour = PIXPACK(0xFFF9F2);
	MenuVisible = 1;
	MenuSection = SC_ORGANIC;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 1;

	Flammable = 20;
	Explosive = 0;
	Meltable = 2;
	Hardness = 2;
	Description = "Sugar. Great food for bacteria.";

	Properties = TYPE_PART | PROP_NEUTPASS | PROP_EDIBLE;


	DefaultProperties.tmp4 = 100;
	DefaultProperties.hydrogens = 25;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 273.15f + 186.0f;
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS) {

	/**
 * Properties
 * tmp4:  amount of matter
 */


	if (parts[i].tmp4 <= 0)
		sim->kill_part(i);
	


	int r, rx, ry;

	r = sim->photons[y][x];
	
	// React with NEUT to make carbon, hydrogen, oxygen
	if (TYP(r) == PT_NEUT && RNG::Ref().chance(1, 20)) {
		sim->kill_part(ID(r));
		if (RNG::Ref().chance(1, 3))
			sim->part_change_type(i, x, y, PT_CRBN);
		else
			sim->part_change_type(i, x, y, RNG::Ref().chance(1, 2) ? PT_O2 : PT_H2);
		return 0;
	}



	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry)) {
				r = pmap[y+ry][x+rx];
				if (!r) continue;

				// Dissolve
				if (TYP(r) == PT_WATR || TYP(r) == PT_DSTW) {
					sim->part_change_type(i, x, y, PT_SWTR);
						sim->kill_part(ID(r));
					return 0;
				}

				// Grow YEST
				else if (TYP(r) == PT_YEST && RNG::Ref().chance(1, 300)) {
					sim->part_change_type(i, parts[i].x, parts[i].y, PT_YEST);
					return 0;
				}

				// React with acid to form carbon
				else if (TYP(r) == PT_ACID || TYP(r) == PT_CAUS) {
					parts[i].temp += 120.0f;
					sim->part_change_type(ID(r), parts[ID(r)].x, parts[ID(r)].y,
						RNG::Ref().chance(1, 2) ? PT_CRBN : PT_WTRV);
					if (RNG::Ref().chance(1, 2))
						sim->kill_part(i);
					return 0;
				}
			} 
	return 0;
}
