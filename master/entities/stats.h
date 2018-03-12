#ifndef ENTITIES_STATS
#define ENTITIES_STATS


struct player_status
{
	uint16_t points;
	uint8_t level ;

	uint8_t current_weapon ;
	uint8_t current_ship ;

	uint16_t bought_equipment ;
};

struct shop_prizes
{
	uint8_t weapons;
	uint8_t ships ;
	uint8_t select ;
};





#endif
