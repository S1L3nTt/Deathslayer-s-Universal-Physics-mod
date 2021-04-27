#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

#define CLOT 180

void Element::Element_BLOD() {
	Identifier = "DEFAULT_PT_BLOD";
	Name = "BLOD";
	Colour = PIXPACK(0xEB1515);
	MenuVisible = 0;
	MenuSection = SC_ORGANIC;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 35;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	DefaultProperties.oxygens = 50;
	DefaultProperties.tmp = 2;

	HeatConduct = 69;
	Description = "Blood. Stains particles, clots when still.";

	Properties = TYPE_LIQUID | PROP_NEUTPENETRATE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS) {
	/**
	 * Properties:
	 * carbons - nutrients
	 * oxygens - How much oxygen stored
	 * tmp - How many particles it can still stain
	 * tmp2 - Visocity, slowly increases randomly with contact to air
	 * tmp3 - How alive the blood is?
	 * hydrogens: carbon dioxide & waste
	 */

	// Boundng
	/*if (parts[i].oxygens < 0)
		parts[i].oxygens = 0;
	if (parts[i].oxygens > 100)
		parts[i].oxygens = 100;*/

	// Freezing
	if (parts[i].temp < 273.15f + sim->pv[y / CELL][x / CELL]) {
		sim->part_change_type(i, parts[i].x, parts[i].y, PT_ICEI);
		parts[i].ctype = PT_BLOD;
		parts[i].dcolour = sim->elements[PT_BLOD].Colour + 0x77000000;
		return 0;
	}

	// Boiling
	if (parts[i].temp > 273.15f + 100.0f - sim->pv[y / CELL][x / CELL]) {
		if (RNG::Ref().chance(1, 500))
			sim->part_change_type(i, parts[i].x, parts[i].y, PT_BRMT);
		else {
			sim->part_change_type(i, parts[i].x, parts[i].y, PT_WTRV);
			parts[i].dcolour = sim->elements[PT_BLOD].Colour + 0x44000000;
		}
	}

	// Clotted blood is inert
	if (parts[i].tmp2 >= CLOT) {
		parts[i].oxygens--;
		if (parts[i].oxygens < 30)
			parts[i].oxygens = 30;
		parts[i].vx = parts[i].vy = 0;
	}

	int rx, ry, r, rt;
	unsigned int newcolor;
	for (rx = -1; rx <= 1; ++rx)
		for (ry = -1; ry <= 1; ++ry)
			if (BOUNDS_CHECK && (rx || ry)) {
				r = pmap[y + ry][x + rx];
				rt = TYP(r);
				if (rt == PT_SOAP && RNG::Ref().chance(1, 100)) {
					//sim->kill_part(i);
					continue;
				}

				// Clotted blood has none of these updates
				if (parts[i].tmp2 >= CLOT)
					continue;

				if (!r) {
					// Random visocity increase if not moving fast and minimal pressure
					if (fabs(parts[i].vx) < 0.1f && fabs(parts[i].vy) < 0.1f &&
							fabs(sim->pv[y / CELL][x / CELL]) < 1.0f)
						parts[i].tmp2 += RNG::Ref().between(1, 7);
					continue;
				}
				

				// Oxygenate
				//if (rt == PT_O2 && parts[i].oxygens < 100) {
				//	parts[i].oxygens += 10;
				//	parts[ID(r)].oxygens -= 10;
					//sim->kill_part(ID(r));
			//	}
				// Spread oxygen to surrounding BLOD particles
				if (rt == PT_BLOD && parts[ID(r)].tmp2 < CLOT) {


					if (parts[i].oxygens < 90 && parts[ID(r)].oxygens >= 10 && parts[i].oxygens < parts[ID(r)].oxygens)
					{
						parts[i].oxygens += 10;
						parts[ID(r)].oxygens -= 10;
					}
					if (parts[i].carbons < 90 && parts[ID(r)].carbons >= 10 && parts[i].carbons < parts[ID(r)].carbons)
					{
						parts[i].carbons += 10;
						parts[ID(r)].carbons -= 10;
					}
					if (parts[i].hydrogens < 90 && parts[ID(r)].hydrogens >= 10 && parts[i].hydrogens < parts[ID(r)].hydrogens)
					{
						parts[i].hydrogens += 10;
						parts[ID(r)].hydrogens -= 10;
					}


				//	parts[i].oxygens = (parts[ID(r)].oxygens + parts[i].oxygens + 1) / 2;
					//parts[ID(r)].oxygens = parts[i].oxygens;
				}
				// Stain powders and solids
				else if (rt != PT_ICEI && rt != PT_SNOW && rt != PT_BIZRS &&
						(sim->elements[rt].Properties & TYPE_PART || sim->elements[rt].Properties & TYPE_SOLID)) {
					newcolor = 0xFF000000 + sim->elements[PT_BLOD].Colour;
					if (parts[ID(r)].dcolour != newcolor && parts[i].tmp > 0) {
						--parts[i].tmp;
						parts[ID(r)].dcolour = newcolor;
					}
				}
				// Stain liquids
				// Liquids get stained more, but in a diluted color
				else if (rt != PT_BLOD && rt != PT_SOAP && rt != PT_BIZR && sim->elements[rt].Properties & TYPE_LIQUID) {
					if (parts[i].tmp > 0) {
						newcolor = 0xAA000000 + sim->elements[PT_BLOD].Colour;
						if (parts[ID(r)].dcolour != newcolor && RNG::Ref().chance(1, 2)) {
							--parts[i].tmp;
							parts[ID(r)].dcolour = newcolor;
						}
					}
				}
				// Chance to kill VIRS
				else if (RNG::Ref().chance(1, 10) && (rt == PT_VIRS || rt == PT_VRSG || rt == PT_VRSS))
					sim->kill_part(ID(r));
				// Chance to kill BCTR
				else if (RNG::Ref().chance(1, 10) && rt == PT_BCTR)
					sim->kill_part(ID(r));
			}

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS) {
	if (cpart->tmp2 < CLOT)
		*pixel_mode |= PMODE_BLUR;
	*colr *= 0.2f + 0.8f * cpart->oxygens / 100.0f;
	*colg *= 0.2f + 0.8f * cpart->oxygens / 100.0f;
	*colb *= 0.2f + 0.8f * cpart->oxygens / 100.0f;

	return 0;
}
