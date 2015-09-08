#ifndef MONOPOLY_PLAYER
#define MONOPOLY_PLAYER

void players_init( uint8_t player_count );

PLAYER_T* player_get( uint8_t index );

PLAYER_T* player_bank_get( void );

void player_money_transfer( PLAYER_T* from, PLAYER_T* to, int32_t amount );

bool player_query( const char* accept );

char* player_line_query( char* description );

long int player_numeric_query( char* description );

void player_exit_cleanup( void );

#endif /* #ifndef MONOPOLY_PLAYER */
