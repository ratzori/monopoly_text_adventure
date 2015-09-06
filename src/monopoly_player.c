#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <ctype.h>

#include "monopoly_common.h"
#include "monopoly_table.h"

#include "monopoly_player.h"

static PLAYER_T banker;
static PLAYER_T players[PLAYER_COUNT_MAX];

void player_banker_init( void )
    {
    banker.token = TOKEN_INVALID;
    banker.name = malloc( sizeof( "Pankki" ) );
    strcpy ( banker.name, "Pankki" );
    banker.account_balance = 1000000;
    banker.current_place = table_square_get( 0 );
    }

void players_init( uint8_t player_count )
    {
    uint8_t i;

    PLAYER_T* bank = &( banker );
    PLAYER_T* player = NULL;

    player_banker_init();

    for ( i = 0; i < PLAYER_COUNT_MAX; i++ )
        {
        player = &( players[i] );

        player->token = TOKEN_INVALID;
        player->name = NULL;
        player->account_balance = 0;
        player->current_place = table_square_get( 0 );

        if ( i < player_count )
            {
            while ( player->name == NULL )
                {
                printf("Pelaajan %i nimi: ", i+1 );
                player->name = player_line_query( NULL );
                }

            player_money_transfer( bank, player, 30000 );
            }
        }
    }

PLAYER_T* player_get( uint8_t index )
    {
    assert( index < PLAYER_COUNT_MAX );

    return &players[index];
    }

PLAYER_T* player_bank_get( void )
    {
    return &banker;
    }

void player_money_transfer( PLAYER_T* from, PLAYER_T* to, int32_t amount )
    {
    printf("%i mk, %s -> %s\n", amount, from->name, to->name );
    from->account_balance -= amount;
    to->account_balance += amount;
    }

bool player_query( const char* accept )
    {
    char* answer;
    size_t answer_length;
    size_t character;

    answer = player_line_query( NULL );

    if ( answer == NULL )
        {
        return false;
        }

    answer_length = strlen( answer );

    for ( character = 0; character < answer_length; character++ )
        {
        /* Convert player's answer to lowercase */
        answer[character] = tolower( answer[character] );
        }

    if ( strstr( answer, accept ) != NULL )
        {
        return true;
        }
    else
        {
        return false;
        }

    free( answer );
    }

char* player_line_query( char* description )
    {
    ssize_t line_length = -1;
    size_t byte_count;
    char* line = NULL;

    if ( description != NULL )
        {
        printf("%s", description );
        }

    line_length = getline( &line, &byte_count, stdin );

    assert( line_length != -1 );

    /* Clean line-endings ( LF, CR, CRLF, LFCR ) */
    line[strcspn(line, "\r\n")] = 0;

    line_length = strlen( line );

    if ( strlen( line ) == 0 )
        {
        /* Player just pressed enter */
        free( line );
        return NULL;
        }

    return line;
    }

long int player_numeric_query( char* description )
    {
    char* answer = NULL;

    while( answer == NULL )
        {
        answer = player_line_query( description );
        }

    return strtol( answer, NULL, 10 );
    }

void player_exit_cleanup( void )
    {
    uint8_t i;

    free( banker.name );

    for ( i = 0; i < PLAYER_COUNT_MAX; i++ )
        {
        if ( players[i].name != NULL )
            {
            free( players[i].name );
            }
        }
    }

