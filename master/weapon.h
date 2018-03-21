/*
 * weapon.h
 *
 *  Created on: Mar 7, 2018
 *      Author: root
 */

#ifndef MASTER_WEAPON_H_
#define MASTER_WEAPON_H_
#include "structure.h"


/**newGun: Initializes weapon instance with upgrade state and gun weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newGun(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the gun weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireGun(Weapon self, Object source, Environment env);


/**newMulti: Initializes weapon instance with upgrade state and Multishot weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newMulti(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the multishot weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireMulti(Weapon self, Object source, Environment env);


/**newLauncher: Initializes weapon instance with upgrade state and launcher weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newLauncher(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the launcher weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireMissile(Weapon self, Object source, Environment env);


/**newHeavy: Initializes weapon instance with upgrade state and heavy weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newHeavy(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the heavy weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireHeavy(Weapon self, Object source, Environment env);


/**newShotGun: Initializes weapon instance with upgrade state and shotgun weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newShotGun(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the shotgun weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireShot(Weapon self, Object source, Environment env);


/**newMachineGun: Initializes weapon instance with upgrade state and machinegun weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newMachineGun(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the machinegun weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireMachineGun(Weapon self, Object source, Environment env);


/**newNoppy: Initializes weapon instance with upgrade state and disc launcher weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newNoppy(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the disc launcher weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireDisc(Weapon self, Object source, Environment env);


/**newBounce: Initializes weapon instance with upgrade state and bouncy ball weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newBounce(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the bouncy ball weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireBounce(Weapon self, Object source, Environment env);


/**newLaser: Initializes weapon instance with upgrade state and laser weapon type.
 * Weapon self: Weapon instance.
 * uint8_t weaponState: Upgrade state of the weapon
**/
void newLaser(Weapon self,uint8_t weaponState);

/**fireGun: fire logic for the laser weapon
 * Weapon self: weapon instance.
 * Object source: firing object.
 * Environment env: Game environment.
 */
void fireLaser(Weapon self, Object source, Environment env);

#endif /* MASTER_WEAPON_H_ */
