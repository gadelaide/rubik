#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
        if (chaine[i] >= '2' && chaine[i] <= '9') {
            rotation = (chaine[i] - '0')%4;
            i++;
        }
        add_algo(seq, mvt, rotation);
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