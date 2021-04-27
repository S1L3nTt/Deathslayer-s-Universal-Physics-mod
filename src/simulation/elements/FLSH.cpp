#include "simulation/ElementCommon.h"

int Element_FLSH_update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_FLSH() {
	Identifier = "DEFAULT_PT_FLSH";
	Name = "FLSH";
	Colour = PIXPACK(0xF05B5B);
	MenuVisible = 1;
	MenuSection = SC_ORGANIC;
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

	Flammable = 1;
	Explosive = 0;
	Meltable = 0;
	Hardness = 15;

	Weight = 100;

	DefaultProperties.oxygens = 50;
	DefaultProperties.carbons = 50;
	DefaultProperties.water = 100;
	DefaultProperties.tmp3 = 100;
	DefaultProperties.tmp4 = 100;
	DefaultProperties.tmpcity[7] = 600;

	HeatConduct = 104;
	Description = "Flesh. Can be cooked.";

	Properties = TYPE_SOLID | PROP_NEUTPENETRATE | PROP_EDIBLE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_FLSH_update;
	Graphics = &graphics;
	Create = &create;
}

static void create(ELEMENT_CREATE_FUNC_ARGS) {
	int gx = x / 50 * 50;
	int gy = y / 50 * 50;
	float theta = x + y;

	int newx = cos(theta) * (x - gx) - sin(theta) * (y - gy) + x;
	int newy = sin(theta) * (x - gx) + cos(theta) * (y - gy) + y;
	x = newx, y = newy;

	int thickness = (x / 4 + y / 4) % 5;
	if ((x + y) % 25 < thickness || abs(x - y) % 25 < thickness) {
		sim->parts[i].life = 1; // White part
	}
}

int Element_FLSH_update(UPDATE_FUNC_ARGS) {
	/**
	 * Properties
	 * life:  Graphics
	 * oxygens:   Oxygen stored
	 * carbons:  Nutrients stored
	 * tmp2: Highest temperature
	 * hydrogens: waste
	 * pavg[0]: Type 0 = inside, 1 = skin, 2 = dead
	 * tmp3: health of part 0-100
	 * tmp4: energy stored for use
	 * tmpcity[8]:  != 0 breaks meat
	 * water: water needed for life
	 * tmpcity[7]: capacity for stuff
	 * 
	 */
	int r = 0;
	int capacity = 0;
	if (parts[i].tmp3 <= 0 && parts[i].pavg[0] != 2)
	{
		parts[i].pavg[0] = 2;
		parts[i].tmp3 = 0;
	}

	if (parts[i].pavg[0] != 2 && sim->timer % 20 == 0)
	{

			if (parts[i].oxygens > 0 && parts[i].carbons > 0 && parts[i].water > 0 && parts[i].tmpcity[3] < 100)
			{
				parts[i].oxygens--;

				parts[i].carbons--;

				parts[i].hydrogens++;
				parts[i].tmpcity[3]++;
				if (RNG::Ref().chance(1, 10))
				parts[i].temp++;
			}
			else if(parts[i].tmpcity[3] > 0)
			{
				parts[i].tmpcity[3]--;
				if (RNG::Ref().chance(1, 10))
				parts[i].temp++;
			}
			else
			{
				parts[i].tmp3--;
			}
			
		
	}
	

	// The randomization is to avoid easily burning meat and creating
	// a more realistic crystallization effect
	if (parts[i].temp > 110.0f + 273.15f && RNG::Ref().chance(1, 100))
		parts[i].tmp2 = parts[i].temp;
	if (parts[i].temp > parts[i].tmp2 && (parts[i].temp < 110.0f + 273.15f ||
			parts[i].temp > 150.0f + 273.15f))
		parts[i].tmp2 = parts[i].temp;
	
	// Rot if dead and not cooked or frozen
	if (parts[i].temp > 3.0f + 273.15f && parts[i].tmp2 < 120.0f + 273.15f && parts[i].pavg[0] == 2
			&& RNG::Ref().chance(1, 10000)) {
		if (RNG::Ref().chance(1, 20))
		{
			sim->part_change_type(i, x, y, PT_BCTR);
			parts[i].ctype = 512; // Meat eating gene
			parts[i].tmp = 0;
			parts[i].tmp2 = 0;
			parts[i].tmp3 = 110;
			//parts[i].life = 110;
			parts[i].oxygens = 0;
			parts[i].hydrogens = 0;
			parts[i].carbons = 0;
			return 1;
		}
		else
			parts[i].tmpcity[8] = 1;
		
	}



	if ((fabs(sim->pv[y / CELL][x / CELL]) > 5.0f && RNG::Ref().chance(1, 300)) || parts[i].tmpcity[8]) {
		sim->part_change_type(i, x, y, PT_BRKN);
		parts[i].ctype = PT_FLSH;
		parts[i].tmpcity[8] = 1;
	}
	if (parts[i].pavg[0] != 2)
	{	
		
		
		// Death
	// Radiation burns
		int r = sim->photons[y][x];
	if (r) {

		if (parts[ID(r)].temp > 273.15f + 110.0f)
		{
			parts[i].tmp2 += 10;
			parts[i].tmp3 --;
	
		}
	}

		// Pressure
		if (fabs(sim->pv[y / CELL][x / CELL]) > 10.0f)
		{
			parts[i].tmp3 --;
			
		}
		// Temperature
		if ((parts[i].temp < 273.15f - 5.0f || parts[i].temp > 50.0f + 273.15f) && RNG::Ref().chance(1, 10))
			parts[i].tmp3--;
		// Lack of oxygen or nutrients
		if ((parts[i].oxygens <= 0 || parts[i].carbons <= 0) && parts[i].tmpcity[3] <= 0)
			parts[i].tmp3--;
	}
	int rx, ry, rt;

	
	for (ry = -1; ry < 2; ry++)
	for (rx = -1; rx < 2; rx++)
		if (BOUNDS_CHECK && (rx || ry)) {
			r = pmap[y + ry][x + rx];
			int partnum = 10;
			if (!r) {
				// Alive flesh
				if (parts[i].pavg[0] != 2 && RNG::Ref().chance(1, 1000)) {
					// Create skin
					parts[i].pavg[0] = 1;
				}
				continue;
			}
			rt = TYP(r);

			// Alive flesh
			if (parts[i].pavg[0] != 2 && parts[ID(r)].pavg[0] != 2) {
				// Distribute nutrients
				if (rt == PT_FLSH || rt == PT_UDDR || rt == PT_LUNG || rt == PT_STMH || rt == PT_BVSL || rt == PT_BLOD)
				{
					if (rt == parts[i].type)
						partnum += 10;
					//take
					capacity = parts[i].oxygens + parts[i].carbons + parts[i].hydrogens;
					if (RNG::Ref().chance(1, 8) && capacity + partnum < parts[i].tmpcity[7])
					{
						

						if (parts[i].oxygens < parts[i].tmpcity[7] / 3 && parts[ID(r)].oxygens >= partnum + 10 && parts[i].oxygens < parts[ID(r)].oxygens)
						{
							parts[i].oxygens += partnum;
							parts[ID(r)].oxygens -= partnum;
						}
						if (parts[i].carbons < parts[i].tmpcity[7] / 3 && parts[ID(r)].carbons >= partnum + 10 && parts[i].carbons < parts[ID(r)].carbons)
						{
							parts[i].carbons += partnum;
							parts[ID(r)].carbons -= partnum;
						}
						if (parts[i].hydrogens < parts[i].tmpcity[7] / 3 && parts[ID(r)].hydrogens >= partnum + 10 && parts[i].hydrogens < parts[ID(r)].hydrogens)
						{
							parts[i].hydrogens += partnum;
							parts[ID(r)].hydrogens -= partnum;
						}
						if (parts[i].water < parts[i].tmpcity[7] / 3 && parts[ID(r)].water >= partnum + 10 && parts[i].water < parts[ID(r)].water)
						{
							parts[i].water += partnum;
							parts[ID(r)].water -= partnum;

						}

					}
					//give
					capacity = parts[ID(r)].oxygens + parts[ID(r)].carbons + parts[ID(r)].hydrogens;
					if (RNG::Ref().chance(1, 8) && capacity + partnum < parts[ID(r)].tmpcity[7])
					{
						
						if (parts[ID(r)].oxygens < parts[ID(r)].tmpcity[7] / 3 && parts[i].oxygens >= partnum + 10 && parts[ID(r)].oxygens < parts[i].oxygens)
						{
							parts[ID(r)].oxygens += partnum;
							parts[i].oxygens -= partnum;
						}
						if (parts[ID(r)].carbons < parts[ID(r)].tmpcity[7] / 3 && parts[i].carbons >= partnum + 10 && parts[ID(r)].carbons < parts[i].carbons)
						{
							parts[ID(r)].carbons += partnum;
							parts[i].carbons -= partnum;
						}
						if (parts[ID(r)].hydrogens < parts[ID(r)].tmpcity[7] / 3 && parts[i].hydrogens >= partnum + 10 && parts[ID(r)].hydrogens < parts[i].hydrogens)
						{
							parts[ID(r)].hydrogens += partnum;
							parts[i].hydrogens -= partnum;
						}
						if (parts[ID(r)].water < parts[ID(r)].tmpcity[7] / 3 && parts[i].water >= partnum + 10 && parts[ID(r)].water < parts[i].water)
						{
							parts[ID(r)].water += partnum;
							parts[i].water -= partnum;

						}
					}

					/*int diff = parts[i].oxygens - parts[ID(r)].oxygens;
					parts[i].oxygens -= diff / 2;
					parts[ID(r)].oxygens += (diff + 1) / 2;
					diff = parts[i].carbons - parts[ID(r)].carbons;
					parts[i].carbons -= diff / 2;
					parts[ID(r)].carbons += (diff + 1) / 2;
					diff = parts[i].hydrogens - parts[ID(r)].hydrogens;
					parts[i].hydrogens -= diff / 2;
					parts[ID(r)].hydrogens += (diff + 1) / 2;*/
				}

				// Die if touching toxic chemicals
				if ((rt == PT_H2O2 || rt == PT_ACID || rt == PT_CAUS || rt == PT_PLUT || rt == PT_URAN ||
					rt == PT_ISOZ ||  rt == PT_ISZS || rt == PT_POLO || rt == PT_MERC || (rt == PT_HCL && parts[i].type != PT_STMH)) && RNG::Ref().chance(1, 20)) {
					parts[i].tmp3--;
				}

				// Absorb oxygen from blood
				//// Pretend there are some nutrients in the blood
				//if (rt == PT_BVSL && parts[ID(r)].tmp > 10 && parts[i].oxygens < 100) {
				////	int amount = std::min(50, parts[ID(r)].life);
				//	parts[i].oxygens += 10;
				//	//parts[i].carbons += 10;
				//	parts[ID(r)].oxygens -= 10;
				//}
			}
		}
	if (fabs(sim->pv[y / CELL][x / CELL]) > 5.0f)
	{
		parts[i].vx += 0.10f * sim->vx[y / CELL][x / CELL];
		parts[i].vy += 0.10f * sim->vy[y / CELL][x / CELL];
	}
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS) {
	if (cpart->pavg[0] == 1) { // Skin
		*colr = 255;
		*colg = 226;
		*colb = 204;
	}
	else if (cpart->life == 1) { // White part
		*colr = 255;
		*colg = 238;
		*colb = 230;
	}
	else { // Redden if oxygenated, green for waste, blue for nutrients
		int red = std::min(40, cpart->oxygens / 10);
		int green = std::min(40, cpart->hydrogens / 10);
		int blue = std::min(40, cpart->carbons / 10);
		*colr += red;
		*colg += green;
		*colb += blue;
		*colg -= red + blue;
		*colb -= red + green;
		*colr -= green + blue;
	}
	

	if (cpart->pavg[0] == 2) { // death
		float percent_fade =  1;
		percent_fade += ((abs(nx - ny) * (nx + ny) + nx) % 5) / 10.0f; // Noise

		*colr -= (*colr - 160) * percent_fade;
		*colg -= (*colg - 96) * percent_fade;
		*colb -= (*colb - 69) * percent_fade;
	}

	// Cooking
	// Well done (Around 70 - 80 C)
	if (cpart->carbons > 273.15f + 40.0f) {
		float percent_fade = std::min(cpart->tmp2 - 273.15f, 80.0f) / 80.0f;
		percent_fade += ((abs(nx - ny) * (nx + ny) + nx) % 5) / 10.0f; // Noise

		*colr -= (*colr - 160) * percent_fade;
		*colg -= (*colg - 96) * percent_fade;
		*colb -= (*colb - 69) * percent_fade;

		// Grill lines
		if ((nx + ny) % 30 < 3) {
			*colr *= 0.9f, *colg *= 0.9f, *colb *= 0.9f;
		}
	}
	// Burnt (Above 110 C)
	if (cpart->tmp2 > 273.15f + 110.0f) {
		float m = 1.0f - std::min(cpart->tmp2 - 273.15f + 90.0f, 200.0f) / 200.0f;
		m = 0.2 + 0.8 * m; // Prevent 100% black
		*colr *= m, *colg *= m, *colb *= m;
	}
	// Blue when cold
	if (cpart->temp < 273 && cpart->tmp2 < 273.15f + 110.0f) {
		*colr -= (int)restrict_flt((273-cpart->temp)/5,0,80);
		*colg += (int)restrict_flt((273-cpart->temp)/4,0,40);
		*colb += (int)restrict_flt((273-cpart->temp)/1.5,0,100);
	}

	return 0;
}
