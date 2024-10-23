#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

enum mvnt_e {
    U, F, R, L, B, D, I
};
typedef enum mvnt_e mvnt;

struct algo_s {
    mvnt mouvement;
    int nb;
    struct algo_s* next;
};
typedef struct algo_s algo;

algo* init_algo() {
    algo* algo = malloc(sizeof(algo));
    assert(algo != NULL);
    algo->mouvement = I;
    algo->nb = 1;
    algo->next = NULL;
    return algo;
}

void add_algo(algo* seq, mvnt mvt, int rotation) {
    if (rotation%4 == 0) return;
    algo* tmp = seq;
    while (tmp->next != NULL) tmp = tmp->next;
    if (tmp->mouvement == I) {
        tmp->mouvement = mvt;
        tmp->nb = rotation % 4;
        return;
    }
    if (tmp->mouvement == mvt) {
        tmp->nb = (tmp->nb + rotation) % 4;
    } else {
        tmp->next = init_algo();
        tmp->next->mouvement = mvt;
        tmp->next->nb = rotation % 4;
    }
}

void free_algo(algo* seq) {
    if (seq->next != NULL) {
        free_algo(seq->next);
    }
    free(seq);
}

algo* copy_algo(algo* seq) {
    algo* new_seq = init_algo();
    algo* tmp = seq;
    while (tmp != NULL) {
        add_algo(new_seq, tmp->mouvement, tmp->nb);
        tmp = tmp->next;
    }
    return new_seq;
}

int length_algo(algo* seq) {
    int len = 0;
    algo* tmp = seq;
    while (tmp != NULL) {
        if (tmp->mouvement != I && tmp->nb%4 != 0) {
            len++;
        }
        tmp = tmp->next;
    }
    return len;
}

void fusion_algo(algo* seq1, algo* seq2) {
    algo* tmp = seq1;
    while (tmp->next != NULL) tmp = tmp->next;
    tmp->next = seq2;
}

void algo_equivalentX_aux(algo** seq) {
    algo* tmp = *seq;
    while (tmp != NULL) {
        switch (tmp->mouvement) {
            case U: tmp->mouvement = F; break;
            case D: tmp->mouvement = B; break;
            case R: tmp->mouvement = R; break;
            case L: tmp->mouvement = L; break;
            case F: tmp->mouvement = D; break;
            case B: tmp->mouvement = U; break;
            default: tmp->mouvement = I; break;
        }
        tmp = tmp->next;
    }
}

void algo_equivalentY_aux(algo** seq) {
    algo* tmp = *seq;
    while (tmp != NULL) {
        switch (tmp->mouvement) {
            case U: tmp->mouvement = U; break;
            case D: tmp->mouvement = D; break;
            case R: tmp->mouvement = B; break;
            case L: tmp->mouvement = F; break;
            case F: tmp->mouvement = R; break;
            case B: tmp->mouvement = L; break;
            default: tmp->mouvement = I; break;
        }
        tmp = tmp->next;
    }
}

void algo_equivalentZ_aux(algo** seq) {
    algo* tmp = *seq;
    while (tmp != NULL) {
        switch (tmp->mouvement) {
            case U: tmp->mouvement = L; break;
            case D: tmp->mouvement = R; break;
            case R: tmp->mouvement = U; break;
            case L: tmp->mouvement = D; break;
            case F: tmp->mouvement = F; break;
            case B: tmp->mouvement = B; break;
            default: tmp->mouvement = I; break;
        }
        tmp = tmp->next;
    }
}

void algo_equivalent(algo** seq, int n, char orientation) {
    int rotations = n % 4;
    if (rotations == 0) {
        return;
    }
    for (int i = 0; i < rotations; i++) {
        switch (orientation) {
            case 'X': algo_equivalentX_aux(seq); break;
            case 'Y': algo_equivalentY_aux(seq); break;
            case 'Z': algo_equivalentZ_aux(seq); break;
            default: break;
        }
    }
}

algo* scramble(int moyenne, int ecart) {
    algo* seq = init_algo();
    int nb;
    if (ecart == 0) {nb = moyenne;}
    else {nb = rand() % (2*ecart) + moyenne - ecart;}
    for (int i = 0; i < nb; i++) {
        int mouvement = rand() % 6;
        char str;
        switch (mouvement) {
            case 0: str = U; break;
            case 1: str = D; break;
            case 2: str = R; break;
            case 3: str = L; break;
            case 4: str = F; break;
            case 5: str = B; break;
        }
        int rotation = rand() % 3 + 1;
        add_algo(seq, str, rotation);
    }
    return seq;
}

void afficher_algo(algo* seq) {
    algo* tmp = seq;
    while (tmp != NULL) {
        if (tmp->mouvement != I) {
            switch (tmp->mouvement) {
                case U: printf("U"); break;
                case D: printf("D"); break;
                case R: printf("R"); break;
                case L: printf("L"); break;
                case F: printf("F"); break;
                case B: printf("B"); break;
                default: break;
            }
            if (tmp->nb%4 > 1) {
                printf("%i", tmp->nb%4);
            }
            printf(" ");
        }
        tmp = tmp->next;
    }
    printf("\n");
}

algo* import_algo(char* chaine) {
    algo* seq = init_algo();
    int i = 0;
    while (chaine[i] != '\0') {
        if (chaine[i] == ' ') {
            i++;
        } else {
            mvnt mvt;
            switch (chaine[i]) {
                case 'U': mvt = U; break;
                case 'D': mvt = D; break;
                case 'R': mvt = R; break;
                case 'L': mvt = L; break;
                case 'F': mvt = F; break;
                case 'B': mvt = B; break;
                default: mvt = I; break;
            }
            i++;
            int rotation = 1;
            if (chaine[i] >= '0' && chaine[i] <= '9') {
                rotation = (chaine[i] - '0')%4;
                i++;
            }
            add_algo(seq, mvt, rotation);
        }
    }
    return seq;
}

void afficher_algo_brut(algo* seq) {
    algo* tmp = seq;
    while (tmp != NULL) {
        printf("(%i, %i) -> ", tmp->mouvement, tmp->nb);
        tmp = tmp->next;
    }
    printf("\n");
}