#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "monopoly.h"
#include "monopoly_common.h"
#include "monopoly_player.h"

#include "monopoly_table.h"

static SQUARE_T table[] =
{
{ 0,  SQUARE_START, NULL },
{ 1,  SQUARE_PROPERTY, NULL },
{ 2,  SQUARE_COMMON_LAND, NULL },
{ 3,  SQUARE_PROPERTY, NULL },
{ 4,  SQUARE_INCOME_TAX, NULL },
{ 5,  SQUARE_STATION, NULL },
{ 6,  SQUARE_PROPERTY, NULL },
{ 7,  SQUARE_RANDOM, NULL },
{ 8,  SQUARE_PROPERTY, NULL },
{ 9,  SQUARE_PROPERTY, NULL },
{ 10, SQUARE_JAIL, NULL },
{ 11, SQUARE_PROPERTY, NULL },
{ 12, SQUARE_UTILITY, NULL },
{ 13, SQUARE_PROPERTY, NULL },
{ 14, SQUARE_PROPERTY, NULL },
{ 15, SQUARE_STATION, NULL },
{ 16, SQUARE_PROPERTY, NULL },
{ 17, SQUARE_COMMON_LAND, NULL },
{ 18, SQUARE_PROPERTY, NULL },
{ 19, SQUARE_PROPERTY, NULL },
{ 20, SQUARE_FREE_PARKING, NULL },
{ 21, SQUARE_PROPERTY, NULL },
{ 22, SQUARE_RANDOM, NULL },
{ 23, SQUARE_PROPERTY, NULL },
{ 24, SQUARE_PROPERTY, NULL },
{ 25, SQUARE_STATION, NULL },
{ 26, SQUARE_PROPERTY, NULL },
{ 27, SQUARE_PROPERTY, NULL },
{ 28, SQUARE_UTILITY, NULL },
{ 29, SQUARE_PROPERTY, NULL },
{ 30, SQUARE_GO_TO_JAIL, NULL },
{ 31, SQUARE_PROPERTY, NULL },
{ 32, SQUARE_PROPERTY, NULL },
{ 33, SQUARE_COMMON_LAND, NULL },
{ 34, SQUARE_PROPERTY, NULL },
{ 35, SQUARE_STATION, NULL },
{ 36, SQUARE_RANDOM, NULL },
{ 37, SQUARE_PROPERTY, NULL },
{ 38, SQUARE_INCOME_TAX, NULL },
{ 39, SQUARE_PROPERTY, NULL }
};

const uint8_t square_total_count = sizeof( table ) / sizeof( table[0] );

static PROPERTY_T real_estate[] =
{
{ 1, "Korkeavuorenkatu", 60, { 2, 10, 30, 90, 160, 250 }, 0, NULL },
{ 1, "Kasarminkatu", 60, { 4, 20, 60, 180, 320, 450 }, 0, NULL },
{ 2, "Rantatie", 100, { 6, 30, 90, 270, 400, 550 }, 0, NULL },
{ 2, "Kauppatori", 100, { 6, 30, 90, 270, 400, 550 }, 0, NULL },
{ 2, "Esplanadi", 120, { 8, 40, 100, 300, 450, 600 }, 0, NULL },
{ 3, "Hameentie", 140, { 10, 50, 150, 450, 625, 750 }, 0, NULL },
{ 3, "Siltasaari", 140, { 10, 50, 150, 450, 625, 750 }, 0, NULL },
{ 3, "Kaisanniemenkatu", 160, { 12, 60, 180, 500, 700, 900 }, 0, NULL },
{ 4, "Liisankatu", 180, { 14, 70, 200, 550, 750, 950 }, 0, NULL },
{ 4, "Snellmaninkatu", 180, { 14, 70, 200, 550, 750, 950 }, 0, NULL },
{ 4, "Unioninkatu", 200, { 16, 80, 220, 600, 800, 1000 }, 0, NULL },
{ 5, "Lonnrotinkatu", 220, { 18, 90, 250, 700, 875, 1050 }, 0, NULL },
{ 5, "Annankatu", 220, { 18, 90, 250, 700, 875, 1050 }, 0, NULL },
{ 5, "Simonkatu", 240, { 20, 100, 300, 750, 925, 1100 }, 0, NULL },
{ 6, "Mikonkatu", 260, { 22, 110, 330, 800, 975, 1150 }, 0, NULL },
{ 6, "Aleksanterinkatu", 260, { 22, 110, 330, 800, 975, 1150 }, 0, NULL },
{ 6, "Keskuskatu", 280, { 24, 120, 360, 850, 1025, 1200 }, 0, NULL },
{ 7, "Tehtaankatu", 300, { 26, 130, 390, 900, 1100, 1275 }, 0,  NULL },
{ 7, "Eira", 300, { 26, 130, 390, 900, 1100, 1275 }, 0,  NULL },
{ 7, "Bulevardi", 320, { 28, 150, 450, 1000, 1200, 1400 }, 0,  NULL },
{ 8, "Mannerheimintie", 350, { 35, 175, 500, 1100, 1300, 1500 }, 0,  NULL },
{ 8, "Erottaja", 400, { 50, 200, 600, 1400, 1700, 2000 }, 0, NULL }
};

const uint8_t property_total_count = sizeof( real_estate ) / sizeof( real_estate[0] );

static STATION_T stations[] =
{
{ "Pasilan asema", 200, 25, NULL },
{ "Sornaisten asema", 200, 25, NULL },
{ "Rautatieasema", 200, 25, NULL },
{ "Tavara-asema", 200, 25, NULL }
};

const uint8_t station_total_count = sizeof( stations ) / sizeof( stations[0] );

static UTILITY_T utilities[] =
{
{ "Sahkolaitos", 150, NULL },
{ "Vesilaitos", 150, NULL }
};

const uint8_t utility_total_count = sizeof( utilities ) / sizeof( utilities[0] );

static PAYMENT_T income_taxes[] =
{
{ "Maksa tulovero", 200, NULL },
{ "Maksa lisavero", 100, NULL }
};

static JAIL_T jail;

void table_init( void )
    {
    PROPERTY_T* property;
    PAYMENT_T* income_tax;
    STATION_T* station;
    UTILITY_T* utility;
    uint8_t curr_prop_count = 0;
    uint8_t curr_income_tax_count = 0;
    uint8_t curr_station_count = 0;
    uint8_t curr_utility_count = 0;
    uint8_t i;

    for ( i = 0; i < PLAYER_COUNT_MAX; i++ )
        {
        jail.prisoners[i].dice_roll_times = 0;
        jail.prisoners[i].inmate = NULL;
        }

    for ( i = 0; i < square_total_count; i++ )
        {
        switch ( table[i].type )
            {
            case SQUARE_START:
                break;
            case SQUARE_COMMON_LAND:
                break;
            case SQUARE_RANDOM:
                break;
            case SQUARE_INCOME_TAX:
                income_tax = (void*)&( income_taxes[curr_income_tax_count] );
                income_tax->receiver = player_bank_get();
                table[i].data = (void*)income_tax;
                curr_income_tax_count++;
                break;
            case SQUARE_FREE_PARKING:
                break;
            case SQUARE_GO_TO_JAIL:
                break;
            case SQUARE_JAIL:
                break;
            case SQUARE_PROPERTY:
                property = &( real_estate[curr_prop_count] );
                property->owner = player_bank_get();
                table[i].data = (void*)property;
                curr_prop_count++;
                break;
            case SQUARE_STATION:
                station = &( stations[curr_station_count] );
                station->owner = player_bank_get();
                table[i].data = (void*)station;
                curr_station_count++;
                break;
            case SQUARE_UTILITY:
                utility = &( utilities[curr_utility_count] );
                utility->owner = player_bank_get();
                table[i].data = (void*)utility;
                curr_utility_count++;
                break;
            default:
                printf("Lol wut\n");
                break;
            }
        }
    }

void table_square_name_print( SQUARE_T* square, PLAYER_T* player )
    {
    PAYMENT_T* payment = NULL;
    PROPERTY_T* property = NULL;
    STATION_T* station = NULL;
    UTILITY_T* utility = NULL;

    switch ( square->type )
        {
        case SQUARE_START:
            printf("lahto");
            break;
        case SQUARE_COMMON_LAND:
            printf("yhteismaa");
            break;
        case SQUARE_RANDOM:
            printf("sattuma");
            break;
        case SQUARE_INCOME_TAX:
            payment = (PAYMENT_T*)square->data;
            printf("%s ( %i eur )", payment->desc, payment->amount );
            break;
        case SQUARE_FREE_PARKING:
            printf("vapaa pysakointi");
            break;
        case SQUARE_GO_TO_JAIL:
            printf("mene suoraan vankilaan, kulkematta lahtoruudun kautta!");
            break;
        case SQUARE_JAIL:
            printf("vankila");
            break;
        case SQUARE_PROPERTY:
            property = (PROPERTY_T*)square->data;
            printf("%s ( %i eur )", property->name, property->price );

            if ( property->owner == player )
                {
                printf(" - oma.");
                }
            else if ( property->owner != player_bank_get() )
                {
                printf(" - omistaja: %s", property->owner->name );
                }
            break;
        case SQUARE_STATION:
            station = (STATION_T*)square->data;
            printf("%s ( %i eur )", station->name, station->price );

            if ( station->owner == player )
                {
                printf(" - oma.");
                }
            else if ( station->owner != player_bank_get() )
                {
                printf(" - omistaja: %s", station->owner->name );
                }
            break;
        case SQUARE_UTILITY:
            utility = (UTILITY_T*)square->data;
            printf("%s ( %i eur )", utility->name, utility->price );

            if ( utility->owner == player )
                {
                printf(" - oma.");
                }
            else if ( utility->owner != player_bank_get() )
                {
                printf(" - omistaja: %s", utility->owner->name );
                }
            break;
        default:
            assert( 0 );
            break;
        }
    }

SQUARE_T* table_square_get( uint8_t index )
    {
    assert ( index < square_total_count );

    return &table[index];
    }

void table_player_position_set( PLAYER_T* player, SQUARE_T* square )
    {
    player->current_place = square;

    printf("%s siirtyy ruutuun: ", ((TOKEN_T*)player->token)->name);
    table_square_name_print( player->current_place, player );
    printf("\n");
    }

void table_player_move( PLAYER_T* player, uint8_t move_count )
    {
    assert( table_square_jail_is_player_in_prison( player ) == false );

    if ( ( player->current_place->index + move_count ) < square_total_count )
        {
        table_player_position_set( player, table_square_get( player->current_place->index + move_count ) );
        }
    else
        {
        /* New round */
        printf("Kuljit lahtoruudun kautta.\n");
        player_money_transfer( player_bank_get(), player, 200 );
        table_player_position_set( player, table_square_get( ( player->current_place->index + move_count ) - square_total_count ) );
        }
    }

uint16_t table_property_rent_calculate( PROPERTY_T* input_property )
    {
    uint8_t i;
    uint8_t group_total_count = 0;
    uint8_t same_owner_in_group_count = 0;
    PROPERTY_T* property;

    for ( i = 0; i < property_total_count; i++ )
        {
        property = &( real_estate[i] );

        if ( property->group == input_property->group )
            {
            group_total_count++;

            if ( property->owner == input_property->owner )
                {
                same_owner_in_group_count++;
                }
            }
        }

    if ( ( group_total_count == same_owner_in_group_count ) && ( input_property->house_count == 0 ) )
        {
        return input_property->rent[0] * 2;
        }
    else
        {
        return input_property->rent[input_property->house_count];
        }
    }

uint16_t table_station_rent_calculate( STATION_T* input_station )
    {
    uint8_t i;
    uint8_t same_owner_count = 0;
    STATION_T* station;
    uint16_t rent = 0;

    for ( i = 0; i < station_total_count; i++ )
        {
        station = &( stations[i] );

        if ( station->owner == input_station->owner )
            {
            same_owner_count++;
            }
        }

    rent = input_station->rent;

    for( i = 1; i < same_owner_count; i++ )
        {
        rent *= 2;
        }

    return rent;
    }

uint16_t table_utility_rent_calculate( UTILITY_T* input_utility, uint8_t dice_result )
    {
    uint8_t i;
    uint8_t same_owner_count = 0;
    UTILITY_T* utility;

    for ( i = 0; i < utility_total_count; i++ )
        {
        utility = &( utilities[i] );

        if ( utility->owner == input_utility->owner )
            {
            same_owner_count++;
            }
        }

    if ( same_owner_count == utility_total_count )
        {
        return 10 * dice_result;
        }
    else
        {
        return 4 * dice_result;
        }
    }

bool table_square_jail_action( PLAYER_T* player )
    {
    PRISONER_T* prisoner;
    prisoner = &( jail.prisoners[player->index] );
    bool allowed_to_continue_round = false;
    bool doubles = false;

    // TODO if: use "Get out of Jail Free card"

    printf("maksa takuut [k/E]: ");
    if ( player_query( "k" ) )
        {
        printf("Maksoit takuut ja vapauduit vankilasta.\n");
        table_square_jail_player_to_free_set( player );
        player_money_transfer( player, player_bank_get(), 50 );
        allowed_to_continue_round = true;
        }
    else
        {
        (void)game_dice_throw( &doubles );

        if ( doubles )
            {
            printf("Heitit tuplat ja vapauduit vankilasta.\n");
            table_square_jail_player_to_free_set( player );
            prisoner->dice_roll_times = 0;
            }
        else
            {
            prisoner->dice_roll_times++;
            if ( prisoner->dice_roll_times == 3 )
                {
                printf("Et saanut tuplia kolmella kierroksella.\n");
                printf("Maksoit takuut ja vapauduit vankilasta.\n");
                player_money_transfer( player, player_bank_get(), 50 );
                table_square_jail_player_to_free_set( player );
                prisoner->dice_roll_times = 0;
                }
            else
                {
                printf("Et saanut tuplia, joten vankeutesi jatkuu.\n");
                }
            }
        }

    return allowed_to_continue_round;
    }

bool table_square_jail_is_player_in_prison( PLAYER_T* player )
    {
    return ( jail.prisoners[player->index].inmate == player );
    }

void table_square_jail_player_to_cell_set( PLAYER_T* player )
    {
    jail.prisoners[player->index].dice_roll_times = 0;
    jail.prisoners[player->index].inmate = player;
    }

void table_square_jail_player_to_free_set( PLAYER_T* player )
    {
    jail.prisoners[player->index].dice_roll_times = 0;
    jail.prisoners[player->index].inmate = NULL;
    }
