#include "Console.h"
#include "Configuration.h"

int main(int argc, const char * argv[]) {
    Console console;
    console.start();
    console.waitForFinish();
    return EC_OK;
}
