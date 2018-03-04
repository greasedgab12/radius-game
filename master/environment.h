/*
 * environment.h
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#ifndef MASTER_ENVIRONMENT_H_
#define MASTER_ENVIRONMENT_H_


Environment newEnvironment(Object* objectList);

void addObject(Environment env,Object instance);

void removeObject(Environment env, Object instance);


#endif /* MASTER_ENVIRONMENT_H_ */
