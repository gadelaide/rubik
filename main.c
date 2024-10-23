#include "manip.h"

int main() {
    srand(time(NULL));

    rubik* cube = init_cube();
    afficher_cube_couleur(cube);
    algo* seq = scramble(3,0);
    afficher_algo(seq);
    ex_algorithme(cube,seq);
    afficher_cube_couleur(cube);
    free_algo(seq);
    free_cube(cube);

    return 0;
}