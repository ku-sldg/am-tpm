#include "powerup.h"
#include "startup.h"
#include "createprimary.h"

int main()
{
    char *powerupArgv[] = {"powerup"};
    char *startupArgv[] = {"startup", "-c"};
    char *createprimaryArgv[] = {"createprimary", "-hi", "p"};
    
    powerup(1, powerupArgv);
    startup(2, startupArgv);
    createprimary(3, createprimaryArgv);

    return 0;
}