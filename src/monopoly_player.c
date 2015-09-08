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

static TOKEN_T tokens[] =
{
{ TOKEN_TERRIER, "Terrieri", NULL },
{ TOKEN_BATTLESHIP, "Taistelulaiva", NULL },
{ TOKEN_AUTOMOBILE, "Auto", NULL },
{ TOKEN_TOP_HAT, "Silinterihattu", NULL },
{ TOKEN_THIMBLE, "Sormustin", NULL },
{ TOKEN_SHOE, "Kenka", NULL },
{ TOKEN_WHEELBARROW, "Kottikarryt", NULL },
{ TOKEN_IRON, "Silitysrauta", NULL },
{ TOKEN_TRAIN, "Juna", NULL },
{ TOKEN_CANNON, "Tykki", NULL }
};

const uint8_t tokens_total_count = sizeof( tokens ) / sizeof( tokens[0] );

TOKEN_T* player_token_select( PLAYER_T* player )
    {
    uint8_t i;
    uint8_t token_index = 1;
    long int token_select;
    TOKEN_T* token = NULL;

    printf("Vapaat pelimerkit:\n");

    for ( i = 0; i < tokens_total_count; i++ )
        {
        if ( tokens[i].owner == NULL )
            {
            printf("\t%i. %s\n", token_index, tokens[i].name);
            token_index++;
            }
        }

    token_select = player_numeric_query( "Valitse pelimerkkisi: " );

    if ( ( token_select == 0 ) || ( token_select >= token_index ) )
        {
        printf("\to_O wut?\n");
        token_select = 1;
        }

    token_index = 1;

    for ( i = 0; i < tokens_total_count; i++ )
        {
        if ( tokens[i].owner == NULL )
            {
            if ( token_index == token_select )
                {
                tokens[i].owner = player;
                token = &( tokens[i] );
                break;
                }
            token_index++;
            }
        }

    assert( token != NULL );

    printf("Valitsit pelimerkiksesi: %s\n", token->name );
    return token;
    }

void player_banker_init( void )
    {
    banker.token = NULL;
    banker.name = malloc( sizeof( "Pankki" ) );
    strcpy ( banker.name, "Pankki" );
    banker.account_balance = BANK_MONEY_AMOUNT;
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

        player->index = i;
        player->token = NULL;
        player->name = NULL;
        player->account_balance = 0;
        player->current_place = table_square_get( 0 );

        if ( i < player_count )
            {
            printf("********************************************************************************\n");

            while ( player->name == NULL )
                {
                printf("Pelaajan %i nimi: ", i+1 );
                player->name = player_line_query( NULL );
                }

            player->token = player_token_select( player );
            player_money_transfer( bank, player, PLAYER_MONEY_START );
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
    printf("%i eur, %s -> %s\n", amount, from->name, to->name );
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
    long int answer_int;
    char* answer = NULL;

    while( answer == NULL )
        {
        answer = player_line_query( description );
        }

    answer_int = strtol( answer, NULL, 10 );
    free( answer );

    return answer_int;
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

