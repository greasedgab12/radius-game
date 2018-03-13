/*
 * enemy.h
 *
 *  Created on: Mar 7, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_ENEMY_H_
#define MASTER_ENTITIES_ENEMY_H_
#include "structure.h"

/**newEnemy:Create blank enemy type object.
 * uint8_t* sprite: pointer to sprite in program memory
 * uint8_t x,y: coordinates of the new enemy type object.
 * return Object: enemy type object.
 */
Object newEnemy(uint8_t * sprite,uint8_t x, uint8_t y);

/**newEnemyFloater: Create an enemy which flies from right to left in a straight line.
 * uint8_t* sprite: pointer to sprite in program memory
 * uint8_t y: y-coordinates of the new floater enemy.
 * return Object: floater enemy.
 */
Object newEnemyFloater(uint8_t * sprite, uint8_t y);

/**Create an oscillating enemy.
 * uint8_t *sprite: pointer to sprite in programm memory.
 * uint8_t y: y-coordinates of the new floater enemy.
 * uint8_t f: phase length of the oscillation.
 * return Object: glider enemy.
 */
Object newEnemyGlider(uint8_t * sprite, uint8_t y, uint8_t phaseLength);

/**Create an enemy which directly flies to the players position.
 * uint8_t *sprite: pointer to sprite in programm memory.
 * uint8_t y: y-coordinates of the new floater enemy.
 * return Object: tracker enemy.
 */
Object newEnemyTracker(uint8_t * sprite, uint8_t y);

/**Create essentially an enemy floater with a gun weapon type.
 * uint8_t *sprite: pointer to sprite in programm memory.
 * uint8_t y: y-coordinates of the new floater enemy.
 * uint8_t invProbability: Parameter inversely proportional to the shot probability.
 * return Object: shooter enemy.
 */
Object newEnemyShooter(uint8_t * sprite, uint8_t y, uint8_t invProbability);

//Base enemy think method. Enemy will only fly from right to left.
void enemyThink(Object self, Environment mainEnv);
//Think method which causes the enemy object to oscillate
void enemyGliderThink(Object self, Environment mainEnv);
//Think method which causes the enemy to track the player.
void enemyTrackerThink(Object self, Environment mainEnv);
//Base enemy think method with usage of the entities weapon.
void enemyShooterThink(Object self, Environment mainEnv);
//Enemy collision routine.
uint8_t enemyCollide(Object self, Object other, uint8_t cType, uint8_t iter);

/**getEnemyByType: Creates enemy object from given parameters.
 * uint8_t type: Type of the desired enemy. See defines.h.
 *  uint8_t* sprite: Pointer to sprite data in program memory.
 *  uint8_t health: Maximum health of the created object.
 *  uint8_t armor: Armor of the created object.
 *  uint8_t speed: Initial velocity and maximum velocity of the created object.
 *  uint8_t y: y Position of the object.
 *  uint8_t param: Either phaseLength or invProbability of the created object,
 *  				if type is either FLOATER or SHOOTER.
 */

Object getEnemyByType( uint8_t type, uint8_t* sprite,uint8_t health,uint8_t armor,int8_t speed, uint8_t y, uint8_t param);
#endif /* MASTER_ENTITIES_ENEMY_H_ */
