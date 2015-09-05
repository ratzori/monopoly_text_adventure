#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

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
{ 12, SQUARE_FACILITY, NULL },
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
{ 28, SQUARE_FACILITY, NULL },
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

const uint8_t square_count = sizeof( table ) / sizeof( table[0] );

static PROPERTY_T real_estate[] =
{
{ 1, "Korkeavuorenkatu", 1000, NULL },
{ 1, "Kasarminkatu", 1000, NULL },
{ 2, "Rantatie", 2000, NULL },
{ 2, "Kauppatori", 2000, NULL },
{ 2, "Esplanadi", 2200, NULL },
{ 3, "Haemeentie", 2500, NULL },
{ 3, "Siltasaari", 2500, NULL },
{ 3, "Kaisanniemenkatu", 3000, NULL },
{ 4, "Liisankatu", 3500, NULL },
{ 4, "Snellmaninkatu", 3500, NULL },
{ 4, "Unioninkatu", 3500, NULL },
{ 5, "Loennrotinkatu", 4200, NULL },
{ 5, "Annankatu", 4200, NULL },
{ 5, "Simonkatu", 4500, NULL },
{ 6, "Mikonkatu", 5000, NULL },
{ 6, "Aleksanterinkatu", 5000, NULL },
{ 6, "Keskuskatu", 5300, NULL },
{ 7, "Tehtaankatu", 6000, NULL },
{ 7, "Eira", 6000, NULL },
{ 7, "Bulevardi", 6000, NULL },
{ 8, "Mannerheimintie", 6500, NULL },
{ 8, "Erottaja", 8000, NULL }
};

const uint8_t property_count = sizeof( real_estate ) / sizeof( real_estate[0] );

static STATION_T stations[] =
{
{ "Pasilan asema", 4000, NULL },
{ "Soernaeisten asema", 4000, NULL },
{ "Rautatieasema", 4000, NULL },
{ "Tavara-asema", 4000, NULL }
};

static FACILITY_T facilities[] =
{
{ "Saehkoelaitos", 3000, NULL },
{ "Vesilaitos", 3000, NULL }
};

static PAYMENT_T income_taxes[] =
{
{ "Maksa tulovero", 4000, NULL },
{ "Maksa lisaevero", 2000, NULL }
};

void table_init( void )
    {
    PROPERTY_T* property;
    PAYMENT_T* income_tax;
    STATION_T* station;
    FACILITY_T* facility;
    uint8_t curr_prop_count = 0;
    uint8_t curr_income_tax_count = 0;
    uint8_t curr_station_count = 0;
    uint8_t curr_facility_count = 0;
    uint8_t i;

    for ( i = 0; i < square_count; i++ )
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
            case SQUARE_FACILITY:
                facility = &( facilities[curr_facility_count] );
                facility->owner = player_bank_get();
                table[i].data = (void*)facility;
                curr_facility_count++;
                break;
            default:
                printf("Lol wut\n");
                break;
            }
        }
    }

void table_square_name_print( SQUARE_T* square )
    {
    PAYMENT_T* payment = NULL;
    PROPERTY_T* property = NULL;
    STATION_T* station = NULL;
    FACILITY_T* facility = NULL;

    switch ( square->type )
        {
        case SQUARE_START:
            printf("Laehtoe\n");
            break;
        case SQUARE_COMMON_LAND:
            printf("Yhteismaa\n");
            break;
        case SQUARE_RANDOM:
            printf("Sattuma\n");
            break;
        case SQUARE_INCOME_TAX:
            payment = (PAYMENT_T*)square->data;
            printf("%s ( %i mk )\n", payment->desc, payment->amount );
            break;
        case SQUARE_FREE_PARKING:
            printf("Vapaa pysaekoeinti\n");
            break;
        case SQUARE_GO_TO_JAIL:
            printf("Mene vankilaan\n");
            break;
        case SQUARE_JAIL:
            printf("Vankila\n");
            break;
        case SQUARE_PROPERTY:
            property = (PROPERTY_T*)square->data;
            printf("%s ( %i mk )", property->name, property->price );
            if ( property->owner != player_bank_get() )
                {
                printf(" - omistaja: %s\n", property->owner->name );
                }
            else
                {
                printf("\n");
                }
            break;
        case SQUARE_STATION:
            station = (STATION_T*)square->data;
            printf("%s ( %i mk )\n", station->name, station->price );
            break;
        case SQUARE_FACILITY:
            facility = (FACILITY_T*)square->data;
            printf("%s ( %i mk )\n", facility->name, facility->price );
            break;
        default:
            assert( 0 );
            break;
        }
    }

SQUARE_T* table_square_get( uint8_t index )
    {
    assert ( index < square_count );

    return &table[index];
    }

void table_player_position_set( PLAYER_T* player, SQUARE_T* square )
    {
    player->current_place = square;

    printf("%s - on nyt ruudussa: ", player->name );
    table_square_name_print( player->current_place );
    }

void table_player_move( PLAYER_T* player, uint8_t move_count )
    {
    if ( ( player->current_place->index + move_count ) < square_count )
        {
        table_player_position_set( player, table_square_get( player->current_place->index + move_count ) );
        }
    else
        {
        /* New round */
        table_player_position_set( player, table_square_get( square_count - player->current_place->index + move_count ) );
        printf("%s kulki lahtoruudun kautta: ", player->name);
        player_money_transfer( player_bank_get(), player, 1000 );
        }
    }

