#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "monopoly_common.h"
#include "monopoly_player.h"
#include "monopoly_table.h"

#include "monopoly.h"

void property_buy( PROPERTY_T* property, PLAYER_T* player )
    {
    if ( player->account_balance >= property->price )
        {
        player_money_transfer( player, property->owner, property->price );

        property->owner = player;
        }
    else
        {
        printf("Teilla ei valitettavasti ole katetta.\n");
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

                if ( player_query( 0x6B ) )
                    {
                    property_buy( property, player );
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
            station = station;
            break;
        case SQUARE_FACILITY:
            facility = (FACILITY_T*)square->data;
            facility = facility; //TODO
            break;
        default:
            printf("Lol wut\n");
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

bool game_round( PLAYER_T* player )
    {
    uint8_t dice_sum;

    printf("%s [ %i mk ] - ", player->name, player->account_balance);
    dice_sum = game_dice_throw();

    table_player_move( player, dice_sum );
    game_square_action( player, player->current_place );

    return true;
    }

int main( void )
    {
    bool game_running = true;
    PLAYER_T* player;
    uint8_t current_player = 1;

    srand (time(NULL));

    table_init();
    players_init( 3 );

    while ( game_running )
        {
        player = player_get( current_player );
        game_running = game_round( player );

        if( ++current_player > 3 )
            {
            current_player = 1;
            }
        }

    return 0;
    }
