#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_WATR()
{
	Identifier = "DEFAULT_PT_WATR";
	Name = "WATR";
	Colour = PIXPACK(0x2030D0);
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

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 30;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "Water. Conducts electricity, freezes, essencial for life.";

	Properties = TYPE_LIQUID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_NEUTPASS;

	DefaultProperties.water = 100;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 273.15f;
	LowTemperatureTransition = PT_ICEI;
	HighTemperature = 373.0f;
	HighTemperatureTransition = PT_WTRV;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	for (ry=-1; ry<2; ry++)
	for (rx=-1; rx<2; rx++)
		
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				// water diffusion

				if (TYP(r) == PT_HCL || TYP(r) == PT_WATR) {




					// give

					if (RNG::Ref().chance(1, 8))
					{
						if (parts[i].tmp4 > parts[ID(r)].tmp4)
						{
							parts[ID(r)].tmp4 += std::min(10, parts[i].tmp4);
							parts[i].tmp4 -= std::min(10, parts[i].tmp4);
							parts[ID(r)].ctype = parts[i].ctype;
						}
						if (parts[i].carbons > parts[ID(r)].carbons)
						{

							parts[ID(r)].carbons += std::min(10, parts[i].carbons);
							parts[i].carbons -= std::min(10, parts[i].carbons);


						}
						if (parts[i].oxygens > parts[ID(r)].oxygens)
						{


							parts[ID(r)].oxygens += std::min(10, parts[i].oxygens);
							parts[i].oxygens -= std::min(10, parts[i].oxygens);


						}
						if (parts[i].hydrogens > parts[ID(r)].hydrogens)
						{


							parts[ID(r)].hydrogens += std::min(10, parts[i].hydrogens);
							parts[i].hydrogens -= std::min(10, parts[i].hydrogens);

						}
						if (parts[i].water > parts[ID(r)].water)
						{


							parts[ID(r)].water += std::min(10, parts[i].water);
							parts[i].water -= std::min(10, parts[i].water);

						}
					}
					// take
					if (RNG::Ref().chance(1, 8))
					{

						if (parts[ID(r)].tmp4 > parts[i].tmp4)
						{
							parts[i].tmp4 += std::min(10, parts[ID(r)].tmp4);
							parts[i].tmp4 -= std::min(10, parts[ID(r)].tmp4);
							parts[i].ctype = parts[ID(r)].ctype;

						}

						if (parts[ID(r)].carbons > parts[i].carbons)
						{

							parts[i].carbons += std::min(10, parts[ID(r)].carbons);
							parts[ID(r)].carbons -= std::min(10, parts[ID(r)].carbons);

						}
						if (parts[ID(r)].oxygens > parts[i].oxygens)
						{


							parts[i].oxygens += std::min(10, parts[ID(r)].oxygens);
							parts[ID(r)].oxygens -= std::min(10, parts[ID(r)].oxygens);

						}
						if (parts[ID(r)].hydrogens > parts[i].hydrogens)
						{


							parts[i].hydrogens += std::min(10, parts[ID(r)].hydrogens);
							parts[ID(r)].hydrogens -= std::min(10, parts[ID(r)].hydrogens);
						}
						if (parts[ID(r)].water > parts[i].water)
						{

							parts[i].water += std::min(10, parts[ID(r)].water);
							parts[ID(r)].water -= std::min(10, parts[ID(r)].water);

						}
					}
				}
				else if (TYP(r)==PT_SALT && RNG::Ref().chance(1, 50))
				{
					sim->part_change_type(i,x,y,PT_SLTW);
					// on average, convert 3 WATR to SLTW before SALT turns into SLTW
					if (RNG::Ref().chance(1, 3))
						sim->part_change_type(ID(r),x+rx,y+ry,PT_SLTW);
				}
				else if ((TYP(r)==PT_RBDM||TYP(r)==PT_LRBD) && (sim->legacy_enable||parts[i].temp>(273.15f+12.0f)) && RNG::Ref().chance(1, 100))
				{
					sim->part_change_type(i,x,y,PT_FIRE);
					parts[i].life = 4;
					parts[i].ctype = PT_WATR;
				}
				else if (TYP(r)==PT_FIRE && parts[ID(r)].ctype!=PT_WATR)
				{
					sim->kill_part(ID(r));
					if (RNG::Ref().chance(1, 30))
					{
						sim->kill_part(i);
						return 1;
					}
				}
			//	else if (TYP(r)==PT_SLTW && RNG::Ref().chance(1, 2000))
				//{
			//		sim->part_change_type(i,x,y,PT_SLTW);
			//	}
				else if (TYP(r)==PT_ROCK && fabs(parts[i].vx)+fabs(parts[i].vy) >= 0.5 && RNG::Ref().chance(1, 1000)) // ROCK erosion
				{
					if (RNG::Ref().chance(1,3))
						sim->part_change_type(ID(r),x+rx,y+ry,PT_SAND);
					else
						sim->part_change_type(ID(r),x+rx,y+ry,PT_STNE);
				}
				if ((sim->elements[TYP(r)].Properties & TYPE_PART ||
					sim->elements[TYP(r)].Properties & TYPE_SOLID) && !RNG::Ref().chance(1, 30))
					parts[i].vx = parts[i].vy = 0;
				if (sim->NoWeightSwitching && TYP(r) != parts[i].type && (y > parts[ID(r)].y && RNG::Ref().chance(1, restrict_flt(20 - sim->elements[TYP(r)].Weight / 10, 1, MAX_TEMP)) || y < parts[ID(r)].y && RNG::Ref().chance(1, 200)) && (sim->elements[TYP(r)].Properties & TYPE_PART || sim->elements[TYP(r)].Properties & TYPE_LIQUID))
				{
					sim->better_do_swap(i, x, y, ID(r), parts[ID(r)].x, parts[ID(r)].y);
					return 1;
				//	sim->better_do_move(i, x, y, parts[ID(r)].x, parts[ID(r)].y);
				//	sim->better_do_move(ID(r), parts[ID(r)].x, parts[ID(r)].y, x, y);
				}
			}
	return 0;
}
