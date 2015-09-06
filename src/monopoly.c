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

void game_property_buy( PROPERTY_T* property, PLAYER_T* player )
    {
    if ( player->account_balance >= property->price )
        {
        player_money_transfer( player, property->owner, property->price );

        property->owner = player;
        }
    else
        {
        printf("Teilla ei valitettavasti ole katetta :(.\n");
        }
    }

void game_square_action( PLAYER_T* player, SQUARE_T* square )
    {
    PAYMENT_T* payment = NULL;
    PROPERTY_T* property = NULL;
    STATION_T* station = NULL;
    FACILITY_T* facility = NULL;

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
            table_player_position_set( player, table_square_get(10) );
            break;
        case SQUARE_JAIL:
            break;
        case SQUARE_PROPERTY:
            property = (PROPERTY_T*)square->data;

            if ( property->owner == player_bank_get() )
                {
                printf("Osta kiinteisto k/E: ");

                if ( player_query( "k" ) )
                    {
                    game_property_buy( property, player );
                    }
                }
            else
                {
                if ( player != property->owner )
                    {
                    player_money_transfer( player, property->owner, property->price / 3 ); /* Check the price */
                    }
                }
            break;
        case SQUARE_STATION:
            station = (STATION_T*)square->data;
            station = station; //TODO
            break;
        case SQUARE_FACILITY:
            facility = (FACILITY_T*)square->data;
            facility = facility; //TODO
            break;
        default:
            printf("Lol wut\n");
            assert( 0 );
            break;
        }
    }

uint8_t game_dice_throw( void )
    {
    uint8_t dice1;
    uint8_t dice2;
    int c;

    printf("heita nopat painamalla enter...");
    c = getchar();
    c = c; // Temp, hide compiling warning
    dice1 = rand() % 6 + 1;
    dice2 = rand() % 6 + 1;

    printf("Silmaeluvut %i ja %i = %i\n", dice1, dice2, dice1+dice2 );

    return dice1+dice2;
    }

void game_round( PLAYER_T* player )
    {
    uint8_t dice_sum;

    printf("%s [ %i mk ] - ", player->name, player->account_balance);
    dice_sum = game_dice_throw();

    table_player_move( player, dice_sum );
    game_square_action( player, player->current_place );
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
                printf("%s [ %i mk ] voitti pelin!\n", player->name, player->account_balance);
                break;
                }

            game_round( player );

            if ( player->account_balance < 0 )
                {
                printf("%s [ %i mk ] ajautui konkurssiin :D\n", player->name, player->account_balance);
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
