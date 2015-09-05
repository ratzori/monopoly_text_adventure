#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "monopoly_common.h"
#include "monopoly_table.h"

#include "monopoly_player.h"

static PLAYER_T players[PLAYER_COUNT_MAX] =
{
{ true, "Pankki", 1000000, NULL },
{ false, "Pelaaja 1", 0, NULL},
{ false, "Pelaaja 2", 0, NULL },
{ false, "Pelaaja 3", 0, NULL },
{ false, "Pelaaja 4", 0, NULL }
};

void players_init( uint8_t player_count )
    {
    uint8_t i;

    PLAYER_T* bank = &( players[0] );
    PLAYER_T* player = &( players[1] );

    for ( i = 0; i < player_count && i < PLAYER_COUNT_MAX-1; i++ )
        {
        if ( !player_money_transfer( bank, player, 30000 ) )
            {
            printf("Pankilta loppui rahat :(\n");
            return;
            }

        player->active = true;
        player->current_place = table_square_get( 0 );

        player++;
        }
    }

PLAYER_T* player_get( uint8_t index )
    {
    if ( index < PLAYER_COUNT_MAX )
        {
        return &players[index];
        }
    else
        {
        printf("player_get(), index %i\n", index );
        return NULL;
        }
    }

PLAYER_T* player_bank_get( void )
    {
    return &players[0];
    }

bool player_money_transfer( PLAYER_T* from, PLAYER_T* to, int32_t amount )
    {
    if ( from->account_balance < amount )
        {
        return false;
        }

    printf("%i mk, %s -> %s\n", amount, from->name, to->name );
    from->account_balance -= amount;
    to->account_balance += amount;
    return true;
    }

bool player_query( int accept )
    {
    int c;
    c = getchar();

    if ( c == accept )
        {
        return true;
        }
    else
        {
        return false;
        }
    }

