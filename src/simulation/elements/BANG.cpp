#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_BANG()
{
	Identifier = "DEFAULT_PT_BANG";
	Name = "TNT";
	Colour = PIXPACK(0xC05050);
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 88;
	Description = "TNT, explodes all at once.";

	Properties = TYPE_SOLID | PROP_NEUTPENETRATE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	//if(parts[i].tmp==0)
	//{
	//	if(parts[i].temp>=673.0f)
	//		parts[i].tmp = 1;
	//	else
			for (rx=-1; rx<2; rx++)
				for (ry=-1; ry<2; ry++)
					if (BOUNDS_CHECK && (rx || ry))
					{
						r = pmap[y+ry][x+rx];
						if (!r)
							continue;
						// Spark detection
						if (TYP(r) == PT_SPRK) {
						 
							
								sim->pv[y / CELL][x / CELL] += 100;
								
								parts[i].life += 500;
								parts[i].temp += 500;
								sim->part_change_type(i, x, y, PT_EMBR);
								return 1;
							
						}
						else if (TYP(r) == PT_EMBR|| TYP(r) == PT_FIRE && parts[i].temp > 800 - sim->pv[y / CELL][x / CELL] && RNG::Ref().chance(1, 20)) {
							
							sim->pv[y / CELL][x / CELL] += 20;
							parts[i].temp += 500;
							parts[i].life += 100;
							sim->part_change_type(i, x, y, RNG::Ref().chance(1, 5) ? PT_EMBR : PT_FIRE);
						//	sim->part_change_type(i, x, y, PT_EMBR);
							return 1;
						}
					}
			if (parts[i].temp > 1000.0f - sim->pv[y / CELL][x / CELL]) {
			
				sim->pv[y / CELL][x / CELL] += 10;
				parts[i].temp += 400;
				parts[i].life += 100;
				
				sim->part_change_type(i, x, y, RNG::Ref().chance(1, 5) ? PT_EMBR : PT_FIRE);
				return 1;
			}
			if (parts[i].temp > 320 - sim->pv[y / CELL][x / CELL])
			{
				parts[i].vx += 0.99f * sim->vx[y / CELL][x / CELL];
				parts[i].vy += 0.99f * sim->vy[y / CELL][x / CELL];
			}
			if (sim->pv[y / CELL][x / CELL] > 150)
			{
				sim->pv[y / CELL][x / CELL] += 20;
				parts[i].temp += 500;
				parts[i].life += 100;
				sim->part_change_type(i, x, y, RNG::Ref().chance(1, 5) ? PT_EMBR : PT_FIRE);
				//	sim->part_change_type(i, x, y, PT_EMBR);
				return 1;
			}



	//}
	//else if(parts[i].tmp==1)
	//{
	//	if ((ID(pmap[y][x]) == i))
	//	{
	//		PropertyValue value;
	//		value.Integer = 2;
	//		sim->flood_prop(x, y, offsetof(Particle, tmp), value, StructProperty::Integer);
	//	}
	//	parts[i].tmp = 2;
	//}
	//else if(parts[i].tmp==2)
	//{
	//	parts[i].tmp = 3;
	//}
	//else
	//{
	//	float otemp = parts[i].temp-273.15f;
	//	//Explode!!
	//	sim->pv[y/CELL][x/CELL] += 0.5f;
	//	parts[i].tmp = 0;
	//	if (RNG::Ref().chance(1, 3))
	//	{
	//		if (RNG::Ref().chance(1, 2))
	//		{
	//			sim->create_part(i, x, y, PT_FIRE);
	//		}
	//		else
	//		{
	//			sim->create_part(i, x, y, PT_SMKE);
	//			parts[i].life = RNG::Ref().between(500, 549);
	//		}
	//		parts[i].temp = restrict_flt((NORMAL_MAX_TEMP /4)+otemp, MIN_TEMP, MAX_TEMP);
	//	}
	//	else
	//	{
	//		if (RNG::Ref().chance(1, 15))
	//		{
	//			sim->create_part(i, x, y, PT_EMBR);
	//			parts[i].tmp = 0;
	//			parts[i].life = 50;
	//			parts[i].temp = restrict_flt((NORMAL_MAX_TEMP/3)+otemp, MIN_TEMP, MAX_TEMP);
	//			parts[i].vx = float(RNG::Ref().between(-10, 10));
	//			parts[i].vy = float(RNG::Ref().between(-10, 10));
	//		}
	//		else
	//		{
	//			sim->kill_part(i);
	//		}
	//	}
	//	return 1;
	//}
	return 0;
}
