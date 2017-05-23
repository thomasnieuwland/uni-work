#include <iostream>
#include "chordController.hpp"

int main(int argc, char *argv[]) {
    chordController c;
    c.read(argv[1]);

    return 0;
}