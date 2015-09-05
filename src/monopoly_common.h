#ifndef MONOPOLY_COMMON
#define MONOPOLY_COMMON

typedef enum
{
SQUARE_INVALID = 0,
SQUARE_START,
SQUARE_COMMON_LAND,
SQUARE_RANDOM,
SQUARE_INCOME_TAX,
SQUARE_FREE_PARKING,
SQUARE_GO_TO_JAIL,
SQUARE_JAIL,
SQUARE_PROPERTY,
SQUARE_STATION,
SQUARE_FACILITY
} SQUARE_E;

typedef struct
{
uint8_t index;
SQUARE_E type;
void* data;
} SQUARE_T;

typedef struct
{
bool active;
char name[100];
int32_t account_balance; /* Check this */
SQUARE_T* current_place;
} PLAYER_T;

#endif /* #ifndef MONOPOLY_COMMON */
