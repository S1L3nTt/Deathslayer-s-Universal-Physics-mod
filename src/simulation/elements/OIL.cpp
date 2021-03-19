#include "simulation/ElementCommon.h"
#include "../ModTools.h"

static int update(UPDATE_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_OIL()
{
	Identifier = "DEFAULT_PT_OIL";
	FullName = "";
	Name = "OIL";
	Colour = PIXPACK(0x404010);
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 20;
	Explosive = 0;
	Meltable = 0;
	Hardness = 5;

	Weight = 20;

	HeatConduct = 42;
	Description = "Flammable, turns into GAS at low pressure or high temperature. Can be formed with NEUT and NITR.";

	Properties = TYPE_LIQUID | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;


	Update = &update;
	Create = &create;
//	Graphics = &graphics;
}
static int update(UPDATE_FUNC_ARGS)
{
	//OIL is a high carbon liquid, it should not have any less than 20 carbons.
	if (parts[i].carbons < 20)sim->part_change_type(i, x, y, PT_DESL);

	int t = parts[i].temp - sim->pv[y / CELL][x / CELL];	//Pressure affects state transitions
	//Freezing into PRFN
	//if (t <= (14.3f * sqrt((parts[i].life - 12))) + 273.15)
	//	sim->part_change_type(i, x, y, PT_PRFN);
	//Boiling into GAS
	if (t > (4 * sqrt(500 * (parts[i].carbons - 4))) + 273.15)
		sim->part_change_type(i, x, y, PT_GAS);
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{


	//Cyens toy
	//Spawns with carbons (20-60), temp must be min 42 ?C
	sim->parts[i].carbons = RNG::Ref().between(20, 61);
	sim->parts[i].hydrogens = makeAlk(sim->parts[i].carbons);
	//sim->parts[i].carbons = rand() % 41 + 20;
	if (sim->parts[i].hydrogens < 2 * sim->parts[i].carbons + 2)sim->parts[i].tmp3 = getBondLoc(sim->parts[i].carbons);
	//Essentially this is creating PRFN then melting it right away
	sim->parts[i].temp = 14.3f * sqrt(sim->parts[i].carbons - 12) + 273.15f;

}




//cyens toy hydrocarbons port
//type, carbons, hydrogens,   tmp3,				ctype
//type,  life,     tmp,       tmp2,            ctype 


//static int graphics(GRAPHICS_FUNC_ARGS)
//{
//	/**firea = 8;
//	*firer = 255;
//	*fireg = 0;
//	*fireb = 0;
//	*pixel_mode |= EFFECT_DBGLINES;
//	*pixel_mode |= EFFECT_GRAVIN;
//	*pixel_mode &= ~PMODE;
//	*pixel_mode |= PMODE_ADD;*/
//	return 1;
//}
