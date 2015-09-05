#ifndef MONOPOLY_TABLE
#define MONOPOLY_TABLE

typedef struct
{
uint8_t area;
const char* name;
uint16_t price;
PLAYER_T* owner;
} PROPERTY_T;

typedef struct
{
const char* name;
uint16_t price;
PLAYER_T* owner;
} STATION_T;

typedef struct
{
const char* name;
uint16_t price;
PLAYER_T* owner;
} FACILITY_T;

typedef struct
{
const char* desc;
uint16_t amount;
PLAYER_T* receiver;
} PAYMENT_T;

void table_init( void );
SQUARE_T* table_square_get( uint8_t index );
void table_player_position_set( PLAYER_T* player, SQUARE_T* square );
void table_player_move( PLAYER_T* player, uint8_t move_count );

#endif /* #ifndef MONOPOLY_TABLE */
