#include "algorithme.h"

int main() {
    algo* seq = init_algo();
    add_algo(seq, U, 1);
    add_algo(seq, F, 2);
    add_algo(seq, R, 3);
    add_algo(seq, L, 4);
    add_algo(seq, B, 5);
    add_algo(seq, D, 6);
    add_algo(seq, U, 7);
    add_algo(seq, F, 8);
    add_algo(seq, R, 9);
    add_algo(seq, L, 10);
    add_algo(seq, B, 11);
    add_algo(seq, D, 12);
    afficher_algo(seq);
    afficher_algo_brut(seq);
    free_algo(seq);

    algo* seq2 = import_algo("U1 F2 R3 L3 B F2 L2 R");
    afficher_algo(seq2);
    afficher_algo_brut(seq2);
    free_algo(seq2);
    
    return 0;
}