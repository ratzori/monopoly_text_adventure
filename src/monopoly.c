#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "monopoly_common.h"
#include "monopoly_player.h"
#include "monopoly_table.h"

#include "monopoly.h"

uint8_t game_dice_throw( bool* doubles )
    {
    uint8_t dice1;
    uint8_t dice2;
    int c;

    printf("Heita nopat painamalla enter...");
    c = getchar();
    c = c; // Temp, hide compiling warning
    dice1 = rand() % 6 + 1;
    dice2 = rand() % 6 + 1;

    printf("Silmaluvut %i ja %i = %i\n", dice1, dice2, dice1+dice2 );

    if ( doubles != NULL )
        {
        *doubles = ( dice1 == dice2 );
        }

    return dice1+dice2;
    }

bool game_check_balance( PLAYER_T* player, uint16_t amount )
    {
    if ( player->account_balance >= amount )
        {
        return true;
        }
    else
        {
        printf("Teilla ei valitettavasti ole katetta :(\n");
        return false;
        }
    }

void game_property_buy( PROPERTY_T* property, PLAYER_T* player )
    {
    if ( game_check_balance( player, property->price ) )
        {
        player_money_transfer( player, property->owner, property->price );
        property->owner = player;
        }
    }

void game_station_buy( STATION_T* station, PLAYER_T* player )
    {
    if ( game_check_balance( player, station->price ) )
        {
        player_money_transfer( player, station->owner, station->price );
        station->owner = player;
        }
    }

void game_utility_buy( UTILITY_T* utility, PLAYER_T* player )
    {
    if ( game_check_balance( player, utility->price ) )
        {
        player_money_transfer( player, utility->owner, utility->price );
        utility->owner = player;
        }
    }

void game_square_action( PLAYER_T* player, SQUARE_T* square )
    {
    PAYMENT_T* payment = NULL;
    PROPERTY_T* property = NULL;
    STATION_T* station = NULL;
    UTILITY_T* utility = NULL;

    switch ( square->type )
        {
        case SQUARE_START:
            break;
        case SQUARE_COMMON_LAND:
            break;
        case SQUARE_RANDOM:
            break;
        case SQUARE_INCOME_TAX:
            payment = (PAYMENT_T*)square->data;
            player_money_transfer( player, player_bank_get(), payment->amount );
            break;
        case SQUARE_FREE_PARKING:
            break;
        case SQUARE_GO_TO_JAIL:
            table_square_jail_player_to_cell_set( player );
            table_player_position_set( player, table_square_get(10) );
            break;
        case SQUARE_JAIL:
            break;
        case SQUARE_PROPERTY:
            property = (PROPERTY_T*)square->data;

            if ( property->owner == player_bank_get() )
                {
                printf("Osta kiinteisto [k/E]: ");

                if ( player_query( "k" ) )
                    {
                    game_property_buy( property, player );
                    }
                }
            else
                {
                if ( player != property->owner )
                    {
                    player_money_transfer( player, property->owner, table_property_rent_calculate( property ) );
                    }
                }
            break;
        case SQUARE_STATION:
            station = (STATION_T*)square->data;
            if ( station->owner == player_bank_get() )
                {
                printf("Osta asema [k/E]: ");

                if ( player_query( "k" ) )
                    {
                    game_station_buy( station, player );
                    }
                }
            else
                {
                if ( player != station->owner )
                    {
                    player_money_transfer( player, station->owner, table_station_rent_calculate( station ) );
                    }
                }
            break;
        case SQUARE_UTILITY:
            utility = (UTILITY_T*)square->data;
            if ( utility->owner == player_bank_get() )
                {
                printf("Osta laitos [k/E]: ");

                if ( player_query( "k" ) )
                    {
                    game_utility_buy( utility, player );
                    }
                }
            else
                {
                if ( player != utility->owner )
                    {
                    player_money_transfer( player, utility->owner,
                        table_utility_rent_calculate( utility, game_dice_throw(NULL) ) );
                    }
                }
            break;
        default:
            printf("Lol wut\n");
            assert( 0 );
            break;
        }
    }

void game_round( PLAYER_T* player )
    {
    uint8_t dice_sum;
    uint8_t doubles_count = 0;
    bool doubles = false;
    printf("********************************************************************************\n");
    printf("%s [ %i eur ] - olet ruudussa: ", player->name, player->account_balance );
    table_square_name_print(player->current_place);
    printf("\n");

    if ( table_square_jail_is_player_in_prison( player ) == true )
        {
        printf("Olet vangittuna - ");

        if ( table_square_jail_action( player ) == false )
            {
            return;
            }
        }

    do
        {
        dice_sum = game_dice_throw( &doubles );

        if ( doubles == true )
            {
            doubles_count++;

            if ( doubles_count == 3 )
                {
                printf("Heitit kolmet tuplat, joten joudut vankilaan >:(\n");
                table_square_jail_player_to_cell_set( player );
                break;
                }
            }

        table_player_move( player, dice_sum );
        game_square_action( player, player->current_place );

        if ( table_square_jail_is_player_in_prison( player ) == true )
            {
            break;
            }

        if ( doubles == true )
            {
            printf("Heitit tuplat! - ");
            }
        }while( doubles );
    }

void game_exit_cleanup( void )
    {
    player_exit_cleanup();
    }

int main( void )
    {
    PLAYER_T* player = NULL;
    long int player_count = 0;
    uint8_t current_player = 0;
    uint8_t active_players = 0;

    srand( time( NULL ) );

    table_init();

    player_count = player_numeric_query( "Montako pelaajaa? " );

    if ( player_count < 2 )
        {
        printf("Pelaajia on oltava vahintaan 2.\n" );
        player_count = 2;
        }
    else if ( player_count > PLAYER_COUNT_MAX )
        {
        printf("Pelaajia voi olla maksimissaan %i.\n", PLAYER_COUNT_MAX );
        player_count = PLAYER_COUNT_MAX;
        }

    active_players = (uint8_t)player_count;
    players_init( (uint8_t)player_count );

    while ( 1 )
        {
        player = player_get( current_player );

        if ( player->name != NULL )
            {
            if ( active_players == 1 )
                {
                printf("%s [ %i eur ] voitti pelin!\n", player->name, player->account_balance);
                break;
                }

            game_round( player );

            if ( player->account_balance < 0 )
                {
                printf("%s [ %i eur ] ajautui konkurssiin :D\n", player->name, player->account_balance);
                free( player->name );
                player->name = NULL;
                active_players--;
                }
            }

        if( ++current_player == player_count )
            {
            current_player = 0;
            }
        }

    game_exit_cleanup();

    printf("Pankkiiri nousee, sammuttaa valot huoneesta ja poistuu paikalta.\n");

    return 0;
    }
