/*
 * weapon.h
 *
 *  Created on: Mar 7, 2018
 *      Author: root
 */

#ifndef MASTER_WEAPON_H_
#define MASTER_WEAPON_H_
#include "structure.h"



Weapon newGun(uint8_t weaponState);
void fireGun(Weapon self, Object source, Environment mainEnv);

Weapon newMulti(uint8_t weaponState);
void fireMulti(Weapon self, Object source, Environment mainEnv);

Weapon newLauncher(uint8_t weaponState);
void fireMissile(Weapon self, Object source, Environment mainEnv);

Weapon newHeavy(uint8_t weaponState);
void fireHeavy(Weapon self, Object source, Environment mainEnv);

Weapon newShot(uint8_t weaponState);
void fireShot(Weapon self, Object source, Environment mainEnv);

Weapon newMachine(uint8_t weaponState);
void fireMachine(Weapon self, Object source, Environment mainEnv);

Weapon newNoppy(uint8_t weaponState);
void fireDisc(Weapon self, Object source, Environment mainEnv);

Weapon newBounce(uint8_t weaponState);
void fireBounce(Weapon self, Object source, Environment mainEnv);



#endif /* MASTER_WEAPON_H_ */
