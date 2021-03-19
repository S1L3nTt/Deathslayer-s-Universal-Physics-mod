#include "ModTools.h"






//cyens god
//Generates a random hydrocarbon type (alkane/ene/yne) with given number of carbons, returns number of hydrogens
 int makeAlk(int c) {
	int alkType = rand() % 3;
	return alkType == 0 ? (2 * c + 2) : alkType == 1 ? (2 * c) : (2 * c - 2);
}
 //cyens god
//Gets the bond location randomly for alkenes and alkynes
 int getBondLoc(int c) {
	return (rand() % (c / 2)) + 1;
}
