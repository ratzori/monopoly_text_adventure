#ifndef MONOPOLY_TABLE
#define MONOPOLY_TABLE

typedef struct
{
uint8_t group;
const char* name;
uint16_t price;
uint16_t rent[6];
uint8_t house_count;
PLAYER_T* owner;
} PROPERTY_T;

typedef struct
{
const char* name;
uint16_t price;
uint16_t rent;
PLAYER_T* owner;
} STATION_T;

typedef struct
{
const char* name;
uint16_t price;
PLAYER_T* owner;
} UTILITY_T;

typedef struct
{
const char* desc;
uint16_t amount;
PLAYER_T* receiver;
} PAYMENT_T;

typedef struct
{
uint8_t dice_roll_times;
PLAYER_T* inmate;
} PRISONER_T;

typedef struct
{
PRISONER_T prisoners[PLAYER_COUNT_MAX];
} JAIL_T;

void table_init( void );
void table_square_name_print( SQUARE_T* square );
SQUARE_T* table_square_get( uint8_t index );
void table_player_position_set( PLAYER_T* player, SQUARE_T* square );
void table_player_move( PLAYER_T* player, uint8_t move_count );
uint16_t table_property_rent_calculate( PROPERTY_T* input_property );
uint16_t table_station_rent_calculate( STATION_T* input_station );
uint16_t table_utility_rent_calculate( UTILITY_T* input_utility, uint8_t dice_result );
bool table_square_jail_action( PLAYER_T* player );
bool table_square_jail_is_player_in_prison( PLAYER_T* player );
void table_square_jail_player_to_cell_set( PLAYER_T* player );
void table_square_jail_player_to_free_set( PLAYER_T* player );

#endif /* #ifndef MONOPOLY_TABLE */
