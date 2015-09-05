#ifndef MONOPOLY_PLAYER
#define MONOPOLY_PLAYER

#define PLAYER_COUNT_MAX 8 + 1
#define START_MONEY 30000

typedef enum
{
TOKEN_INVALID = 0,
TOKEN_TERRIER,
TOKEN_BATTLESHIP,
TOKEN_AUTOMOBILE,
TOKEN_TOP_HAT,
TOKEN_THIMBLE,
TOKEN_SHOE,
TOKEN_WHEELBARROW,
TOKEN_IRON
} TOKEN_T;


void players_init( uint8_t player_count );

PLAYER_T* player_get( uint8_t index );

PLAYER_T* player_bank_get( void );

bool player_money_transfer( PLAYER_T* from, PLAYER_T* to, int32_t amount );

bool player_query( int accept );

#endif /* #ifndef MONOPOLY_PLAYER */
