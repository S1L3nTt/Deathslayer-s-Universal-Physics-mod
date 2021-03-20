#include "simulation/ElementCommon.h"
#include "../ModTools.h"

static int update(UPDATE_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);


void Element::Element_GAS()
{
	Identifier = "DEFAULT_PT_GAS";
	Name = "GAS";
	Colour = PIXPACK(0xE0FF20);
	MenuVisible = 1;
	MenuSection = SC_GAS;
	Enabled = 1;

	Advection = 1.0f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.75f;
	HotAir = 0.001f	* CFDS;
	Falldown = 0;

	Flammable = 600;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 1;

	DefaultProperties.temp = R_TEMP + 2.0f + 273.15f;
	HeatConduct = 42;
	Description = "Diffuses quickly and is flammable. Liquefies into OIL under pressure.";

	Properties = TYPE_GAS | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
}
// RNG::Ref().between(20, 61)

static int update(UPDATE_FUNC_ARGS) {
	//carbons=num carbons
	//tmp=num tmp3
	//tmp3=location of double/triple bond if alkene/alkyne (tmp<2*carbons+2)

	//Condensation
	int t = parts[i].temp - sim->pv[y / CELL][x / CELL];	//Pressure affects state transitions
	if ((parts[i].carbons == 1 && t <= (-180 + 273.15)) || (parts[i].carbons == 2 && t <= (-100 + 273.15)) || (parts[i].carbons == 3 && t <= -50 + 273.15) || (parts[i].carbons >= 4 && t <= (4 * sqrt(500 * (parts[i].carbons - 4))) + 273.15))
	{
		if (parts[i].carbons < 8)//Low carbon condensation
			sim->part_change_type(i, x, y, PT_MWAX);
		else if (parts[i].carbons >= 8 && parts[i].carbons < 20) //Medium carbon condensation
			sim->part_change_type(i, x, y, PT_DESL);
		else //High carbon condensation
			sim->part_change_type(i, x, y, PT_OIL);
	}

	//Update
	int r, rx, ry;
	for (rx = -1; rx < 2; rx++)
	{
		for (ry = -1; ry < 2; ry++)
		{
			if (BOUNDS_CHECK)
			{
				r = pmap[y + ry][x + rx];
				if (!r || TYP(r) == PT_GAS)
					r = sim->photons[y + ry][x + rx];
				if (!r)
					continue;
				if (parts[i].temp >= -100 * log(parts[i].carbons) + 673.15f || TYP(r) == PT_FIRE || TYP(r) == PT_PLSM || TYP(r) == PT_LAVA) {
					for (int yy = -3; yy <= 3; yy++)
						for (int xx = -3; xx <= 3; xx++)
							if (xx * xx + yy * yy <= 9) {
								sim->create_part(-1, x + xx, y + yy, PT_FIRE);
								sim->pv[y / CELL][x / CELL] = parts[i].carbons > 45 ? 5 : 50.0f - parts[i].carbons;
								sim->part_change_type(i, x, y, PT_CO2);
							}
					if (RNG::Ref().between(1,  parts[i].carbons))sim->part_change_type(i, x, y, RNG::Ref().between(10, 3) ? PT_CO2 : PT_SMKE);
				}


				}
			}
		}

	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS) {
	//Spawns with carbons (1-4)
	sim->parts[i].carbons = RNG::Ref().between(1, 4);
	if (sim->parts[i].carbons == 1) { //Creation of methane, can only be CH4 as a pure hydrocarbon
		sim->parts[i].hydrogens= 4;
		sim->parts[i].tmp3 = 0;
	}
	else { //Creating any other type of hydrocarbon
		sim->parts[i].hydrogens= makeAlk(sim->parts[i].carbons);
		if (sim->parts[i].hydrogens< 2 * sim->parts[i].carbons + 2)sim->parts[i].tmp3 = getBondLoc(sim->parts[i].carbons);
	}
}