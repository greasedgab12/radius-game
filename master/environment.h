#ifndef MASTER_ENVIRONMENT_H_
#define MASTER_ENVIRONMENT_H_
#include "object.h"


typedef struct Environment_struct* Environment;

struct Environment_struct{
	uint8_t buttons;
	uint32_t time;
	uint8_t gameState;
	Object* objectList;
	uint8_t oPos;
};


#endif /* MASTER_ENVIRONMENT_H_ */
