/*
 * weapon.h
 *
 *  Created on: Mar 7, 2018
 *      Author: root
 */

#ifndef MASTER_WEAPON_H_
#define MASTER_WEAPON_H_
#include "structure.h"



void newGun(Weapon self,uint8_t weaponState);
void fireGun(Weapon self, Object source, Environment mainEnv);

void newMulti(Weapon self,uint8_t weaponState);
void fireMulti(Weapon self, Object source, Environment mainEnv);

void newLauncher(Weapon self,uint8_t weaponState);
void fireMissile(Weapon self, Object source, Environment mainEnv);

void newHeavy(Weapon self,uint8_t weaponState);
void fireHeavy(Weapon self, Object source, Environment mainEnv);

void newShotGun(Weapon self,uint8_t weaponState);
void fireShot(Weapon self, Object source, Environment mainEnv);

void newMachineGun(Weapon self,uint8_t weaponState);
void fireMachineGun(Weapon self, Object source, Environment mainEnv);

void newNoppy(Weapon self,uint8_t weaponState);
void fireDisc(Weapon self, Object source, Environment mainEnv);

void newBounce(Weapon self,uint8_t weaponState);
void fireBounce(Weapon self, Object source, Environment mainEnv);



#endif /* MASTER_WEAPON_H_ */
