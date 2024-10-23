#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int corners[8];
    int edges[12];
    int twisted[8];
    int flipped[12];
} rubik;


/* REPRESENTATIONS

couche 1 :
            coins 7     | arete 10  | coins 4
          arete 7     | centre U  | arete 4
        coins 3     | arete 3   | coins 0

couche 2 :
            arete 8     | centre B  | arete 5
          centre L    | x         | centre R
        arete 2     | centre F  | arete 0

couche 3 :
            coins 6     | arete 11  | coins 5
          arete 9     | centre D  | arete 6
        coins 2     | arete 1   | coins 1


                0  1  2
                3  U  5
                6  7  8
     36 37 38 | 9  10 11 | 18 19 20 | 27 28 29
     39 L  41 | 12 F  14 | 21 R  23 | 30 B  32
     42 43 43 | 15 16 17 | 24 25 26 | 33 34 35
                45 46 47
                48 D  50
                51 52 53

*/

rubik* init_cube() {
    rubik* cube = malloc(sizeof(rubik));
    assert(cube != NULL);
    for (int i = 0; i < 8; i++) {
        cube->corners[i] = i;
        cube->twisted[i] = 0;
    }
    for (int i = 0; i < 12; i++) {
        cube->edges[i] = i;
        cube->flipped[i] = 0;
    }
    return cube;
}

void free_cube(rubik* cube) {
    if (cube != NULL) {
        free(cube);
    }
}

void reset_cube(rubik* cube) {
    for (int i = 0; i < 8; i++) {
        cube->corners[i] = i;
        cube->twisted[i] = 0;
    }
    for (int i = 0; i < 12; i++) {
        cube->edges[i] = i;
        cube->flipped[i] = 0;
    }
}

int sign(int* permutation, int n) {
    int signature = 1;
    int i, j;
    
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (permutation[i] > permutation[j]) {
                signature = -signature;
            }
        }
    }
    return signature;
}

bool is_solvable(rubik* cube) {
    int nb_twisted = 0;
    int nb_flipped = 0;
    for (int i = 0; i < 12; i++) {
        nb_flipped += cube->flipped[i];
    }
    for(int i = 0; i < 8; i++) {
        nb_twisted += cube->twisted[i];
    }
    for(int i = 0; i < 8; i++) {
        if (cube->corners[i] < 0 || cube->corners[i] > 7) return false;
    }
    for(int i = 0; i < 12; i++) {
        if (cube->edges[i] < 0 || cube->edges[i] > 11) return false;
    }
    for(int i = 0; i < 8; i++) {
        if (cube->twisted[i] != 0 && cube->twisted[i] != 1 && cube->twisted[i] != 2) return false;
    }
    for(int i = 0; i < 12; i++) {
        if (cube->flipped[i] != 0 && cube->flipped[i] != 1) return false;
    }
    if ((nb_twisted % 3 != 0 || nb_flipped % 2 != 0 )|| (sign(cube->corners,8) != sign(cube->edges,12))) return false;
    return true;
}

char corner_to_face(int i, int j) {
    switch (i) {
        case 7: if (j == 0) { return 'U'; } else if (j == 1) { return 'L'; } else if (j == 2) { return 'B'; } break;
        case 4: if (j == 0) { return 'U'; } else if (j == 1) { return 'B'; } else if (j == 2) { return 'R'; } break;
        case 0: if (j == 0) { return 'U'; } else if (j == 1) { return 'R'; } else if (j == 2) { return 'F'; } break;
        case 3: if (j == 0) { return 'U'; } else if (j == 1) { return 'F'; } else if (j == 2) { return 'L'; } break;
        case 6: if (j == 0) { return 'D'; } else if (j == 1) { return 'B'; } else if (j == 2) { return 'L'; } break;
        case 5: if (j == 0) { return 'D'; } else if (j == 1) { return 'R'; } else if (j == 2) { return 'B'; } break;
        case 1: if (j == 0) { return 'D'; } else if (j == 1) { return 'F'; } else if (j == 2) { return 'R'; } break;
        case 2: if (j == 0) { return 'D'; } else if (j == 1) { return 'L'; } else if (j == 2) { return 'F'; } break;
    }
    return 'I';
}

char edge_to_face(int i, int j) {
    switch (i) {
        case 10: if (j == 0) { return 'U'; } else if (j == 1) { return 'B'; } break;
        case 7: if (j == 0) { return 'U'; } else if (j == 1) { return 'L'; } break;
        case 4: if (j == 0) { return 'U'; } else if (j == 1) { return 'R'; } break;
        case 3: if (j == 0) { return 'U'; } else if (j == 1) { return 'F'; } break;
        case 8: if (j == 0) { return 'L'; } else if (j == 1) { return 'B'; } break;
        case 5: if (j == 0) { return 'R'; } else if (j == 1) { return 'B'; } break;
        case 0: if (j == 0) { return 'R'; } else if (j == 1) { return 'F'; } break;
        case 2: if (j == 0) { return 'L'; } else if (j == 1) { return 'F'; } break;
        case 11: if (j == 0) { return 'D'; } else if (j == 1) { return 'B'; } break;
        case 9: if (j == 0) { return 'D'; } else if (j == 1) { return 'L'; } break;
        case 6: if (j == 0) { return 'D'; } else if (j == 1) { return 'R'; } break;
        case 1: if (j == 0) { return 'D'; } else if (j == 1) { return 'F'; } break;
    }
    return 'I';
}

char* cube_to_chaine(rubik* cube) {
    char* res = malloc(sizeof(char)*54);
    assert(res != NULL);
    // face U
    res[0] = corner_to_face(cube->corners[7],cube->twisted[7]);
    res[1] = edge_to_face(cube->edges[10],cube->flipped[10]);
    res[2] = corner_to_face(cube->corners[4],cube->twisted[4]);
    res[3] = edge_to_face(cube->edges[7],cube->flipped[7]);
    res[4] = 'U';
    res[5] = edge_to_face(cube->edges[4],cube->flipped[4]);
    res[6] = corner_to_face(cube->corners[3],cube->twisted[3]);
    res[7] = edge_to_face(cube->edges[3],cube->flipped[3]);
    res[8] = corner_to_face(cube->corners[0],cube->twisted[0]);
    //face F
    res[9] = corner_to_face(cube->corners[3],(cube->twisted[3]+1)%3);
    res[10] = edge_to_face(cube->edges[3],(cube->flipped[3]+1)%2);
    res[11] = corner_to_face(cube->corners[0],(cube->twisted[0]+2)%3);
    res[12] = edge_to_face(cube->edges[2],(cube->flipped[2]+1)%2);
    res[13] = 'F';
    res[14] = edge_to_face(cube->edges[0],(cube->flipped[0]+1)%2);
    res[15] = corner_to_face(cube->corners[2],(cube->twisted[2]+2)%3);
    res[16] = edge_to_face(cube->edges[1],(cube->flipped[1]+1)%2);
    res[17] = corner_to_face(cube->corners[1],(cube->twisted[1]+1)%3);
    //face R
    res[18] = corner_to_face(cube->corners[0],(cube->twisted[0]+1)%3);
    res[19] = edge_to_face(cube->edges[4],(cube->flipped[4]+1)%2);
    res[20] = corner_to_face(cube->corners[4],(cube->twisted[4]+2)%3);
    res[21] = edge_to_face(cube->edges[0],(cube->flipped[0]+0)%2);
    res[22] = 'R';
    res[23] = edge_to_face(cube->edges[5],(cube->flipped[5]+0)%2);
    res[24] = corner_to_face(cube->corners[1],(cube->twisted[1]+2)%3);
    res[25] = edge_to_face(cube->edges[6],(cube->flipped[6]+1)%2);
    res[26] = corner_to_face(cube->corners[5],(cube->twisted[5]+1)%3);
    //face B
    res[27] = corner_to_face(cube->corners[4],(cube->twisted[4]+1)%3);
    res[28] = edge_to_face(cube->edges[10],(cube->flipped[10]+1)%2);
    res[29] = corner_to_face(cube->corners[7],(cube->twisted[7]+2)%3);
    res[30] = edge_to_face(cube->edges[5],(cube->flipped[5]+1)%2);
    res[31] = 'B';
    res[32] = edge_to_face(cube->edges[8],(cube->flipped[8]+1)%2);
    res[33] = corner_to_face(cube->corners[5],(cube->twisted[5]+2)%3);
    res[34] = edge_to_face(cube->edges[11],(cube->flipped[11]+1)%2);
    res[35] = corner_to_face(cube->corners[6],(cube->twisted[6]+1)%3);
    //face L
    res[36] = corner_to_face(cube->corners[7],(cube->twisted[7]+1)%3);
    res[37] = edge_to_face(cube->edges[7],(cube->flipped[7]+1)%2);
    res[38] = corner_to_face(cube->corners[3],(cube->twisted[3]+2)%3);
    res[39] = edge_to_face(cube->edges[8],(cube->flipped[8]+0)%2);
    res[40] = 'L';
    res[41] = edge_to_face(cube->edges[2],(cube->flipped[2]+0)%2);
    res[42] = corner_to_face(cube->corners[6],(cube->twisted[6]+2)%3);
    res[43] = edge_to_face(cube->edges[9],(cube->flipped[9]+1)%2);
    res[44] = corner_to_face(cube->corners[2],(cube->twisted[2]+1)%3);
    //face D
    res[45] = corner_to_face(cube->corners[2],cube->twisted[2]);
    res[46] = edge_to_face(cube->edges[1],cube->flipped[1]);
    res[47] = corner_to_face(cube->corners[1],cube->twisted[1]);
    res[48] = edge_to_face(cube->edges[9],cube->flipped[9]);
    res[49] = 'D';
    res[50] = edge_to_face(cube->edges[6],cube->flipped[6]);
    res[51] = corner_to_face(cube->corners[6],cube->twisted[6]);
    res[52] = edge_to_face(cube->edges[11],cube->flipped[11]);
    res[53] = corner_to_face(cube->corners[5],cube->twisted[5]);
    return res;
}

void afficher_cube_default(rubik* cube) {
    char* str = cube_to_chaine(cube);
    printf("       %c %c %c\n", str[0], str[1], str[2]);
    printf("       %c %c %c\n", str[3], str[4], str[5]);
    printf("       %c %c %c\n", str[6], str[7], str[8]);
    printf("%c %c %c  %c %c %c  %c %c %c  %c %c %c\n", str[36], str[37], str[38], str[9], str[10], str[11], str[18], str[19], str[20], str[27], str[28], str[29]);
    printf("%c %c %c  %c %c %c  %c %c %c  %c %c %c\n", str[39], str[40], str[41], str[12], str[13], str[14], str[21], str[22], str[23], str[30], str[31], str[32]);
    printf("%c %c %c  %c %c %c  %c %c %c  %c %c %c\n", str[42], str[43], str[44], str[15], str[16], str[17], str[24], str[25], str[26], str[33], str[34], str[35]);
    printf("       %c %c %c\n", str[45], str[46], str[47]);
    printf("       %c %c %c\n", str[48], str[49], str[50]);
    printf("       %c %c %c\n", str[51], str[52], str[53]);
    free(str);
}

char face_to_color(char c) {
    switch (c) {
        case 'U': return 'Y'; case 'F': return 'G'; case 'R': return 'O'; case 'B': return 'B'; case 'L': return 'R'; case 'D': return 'W'; default: return 'I';
    }
}

void afficher_cube_couleur(rubik* cube) {
    char* str1 = cube_to_chaine(cube);
    for(int i = 0; i < 54; i++) {
        str1[i] = face_to_color(str1[i]);
    }
    printf("       %c %c %c\n", str1[0], str1[1], str1[2]);
    printf("       %c %c %c\n", str1[3], str1[4], str1[5]);
    printf("       %c %c %c\n", str1[6], str1[7], str1[8]);
    printf("%c %c %c  %c %c %c  %c %c %c  %c %c %c\n", str1[36], str1[37], str1[38], str1[9], str1[10], str1[11], str1[18], str1[19], str1[20], str1[27], str1[28], str1[29]);
    printf("%c %c %c  %c %c %c  %c %c %c  %c %c %c\n", str1[39], str1[40], str1[41], str1[12], str1[13], str1[14], str1[21], str1[22], str1[23], str1[30], str1[31], str1[32]);
    printf("%c %c %c  %c %c %c  %c %c %c  %c %c %c\n", str1[42], str1[43], str1[44], str1[15], str1[16], str1[17], str1[24], str1[25], str1[26], str1[33], str1[34], str1[35]);
    printf("       %c %c %c\n", str1[45], str1[46], str1[47]);
    printf("       %c %c %c\n", str1[48], str1[49], str1[50]);
    printf("       %c %c %c\n", str1[51], str1[52], str1[53]);
    free(str1);
}

int get_corners(int n) {
    switch (n) {
        case 30: return 0;
        case 195: return 1;
        case 429: return 2;
        case 66: return 3;
        case 70: return 4;
        case 455: return 5;
        case 1001: return 6;
        case 154: return 7;
        default: return -1;
    }
}

int get_edges(int n) {
    switch (n) {
        //(U = 2, F = 3, R = 5, B = 7, L = 11, D = 13)
        case 15: return 0;
        case 39: return 1;
        case 33: return 2;
        case 6: return 3;
        case 10: return 4;
        case 35: return 5;
        case 65: return 6;
        case 22: return 7;
        case 77: return 8;
        case 143: return 9;
        case 14: return 10;
        case 91: return 11;
        default: return -1;
    }
}

int f2p_aux(char c) {
    switch (c) {
        case 'U': return 2; case 'F': return 3; case 'R': return 5; case 'B': return 7; case 'L': return 11; case 'D': return 13; default: return -1;
    }
}

rubik* import_cube(char* str_brut) { // forme str_brut : "WW..WW GG..GG ... YY..YY"

    char* str = malloc(sizeof(char)*54);  // forme str : "WW..WWGG..GG...YY..YY"
    assert(str != NULL);
    int j = 0;
    for (int i = 0; str_brut[i] != '\0'; i++) {
        if (str_brut[i] >= 'A' && str_brut[i] <= 'Z') {
            str[j] = str_brut[i];
            j++;
        }
    }
    assert(j == 54);
    rubik* cube = init_cube();
    
    char* str_std = malloc(sizeof(char)*54); // forme str_std : "UU..UUFF..FF...DD..DD" (forme standard)
    assert(str_std != NULL);
    char centre_U = str[4];
    char centre_F = str[13];
    char centre_R = str[22];
    char centre_B = str[31];
    char centre_L = str[40];
    char centre_D = str[49];
    for (int i = 0; i < 54; i++) {
        if (str[i] == centre_U) str_std[i] = 'U';
        if (str[i] == centre_F) str_std[i] = 'F';
        if (str[i] == centre_R) str_std[i] = 'R';
        if (str[i] == centre_B) str_std[i] = 'B';
        if (str[i] == centre_L) str_std[i] = 'L';
        if (str[i] == centre_D) str_std[i] = 'D';
    }
    //cube.corners
    int aretes0[12] = {21, 46, 41, 7, 5, 23, 50, 3, 39, 48, 1, 52};
    int aretes1[12] = {14, 16, 12, 10, 19, 30, 25, 37, 32, 43, 28, 34};
    cube->corners[0] = get_corners(f2p_aux(str_std[8]) * f2p_aux(str_std[11]) * f2p_aux(str_std[18]));
    cube->corners[1] = get_corners(f2p_aux(str_std[17]) * f2p_aux(str_std[24]) * f2p_aux(str_std[47]));
    cube->corners[2] = get_corners(f2p_aux(str_std[15]) * f2p_aux(str_std[44]) * f2p_aux(str_std[45]));
    cube->corners[3] = get_corners(f2p_aux(str_std[6]) * f2p_aux(str_std[9]) * f2p_aux(str_std[38]));
    cube->corners[4] = get_corners(f2p_aux(str_std[2]) * f2p_aux(str_std[20]) * f2p_aux(str_std[27]));
    cube->corners[5] = get_corners(f2p_aux(str_std[26]) * f2p_aux(str_std[33]) * f2p_aux(str_std[53]));
    cube->corners[6] = get_corners(f2p_aux(str_std[35]) * f2p_aux(str_std[42]) * f2p_aux(str_std[51]));
    cube->corners[7] = get_corners(f2p_aux(str_std[0]) * f2p_aux(str_std[36]) * f2p_aux(str_std[29]));
    //cube.edges
    for(int i = 0; i < 12; i++) {
        cube->edges[i] = get_edges(f2p_aux(str_std[aretes0[i]]) * f2p_aux(str_std[aretes1[i]]));
    }
    //cube.flipped
    for(int i = 0; i < 12; i++) {
        if (str_std[aretes0[i]] == 'U' || str_std[aretes0[i]] == 'D') {cube->flipped[i] = 0;}
        else {if (str_std[aretes0[i]] == 'R' || str_std[aretes0[i]] == 'L') {
            if (str_std[aretes1[i]] == 'U' || str_std[aretes1[i]] == 'D') {cube->flipped[i] = 1;}
            if (str_std[aretes1[i]] == 'F' || str_std[aretes1[i]] == 'B') {cube->flipped[i] = 0;}
            }
            else {cube->flipped[i] = 1;}
        }
    }
    //cube.twisted
    if (str_std[8] == 'U' || str_std[8] == 'D') { cube->twisted[0] = 0; } if(str_std[11] == 'U' || str_std[11] == 'D') { cube->twisted[0] = 1; } if(str_std[18] == 'U' || str_std[18] == 'D') { cube->twisted[0] = 2; }
    if (str_std[17] == 'U' || str_std[17] == 'D') { cube->twisted[1] = 2; } if(str_std[24] == 'U' || str_std[24] == 'D') { cube->twisted[1] = 1; } if(str_std[47] == 'U' || str_std[47] == 'D') { cube->twisted[1] = 0; }
    if (str_std[15] == 'U' || str_std[15] == 'D') { cube->twisted[2] = 1; } if(str_std[44] == 'U' || str_std[44] == 'D') { cube->twisted[2] = 2; } if(str_std[45] == 'U' || str_std[45] == 'D') { cube->twisted[2] = 0; }
    if (str_std[6] == 'U' || str_std[6] == 'D') { cube->twisted[3] = 0; } if(str_std[9] == 'U' || str_std[9] == 'D') { cube->twisted[3] = 2; } if(str_std[38] == 'U' || str_std[38] == 'D') { cube->twisted[3] = 1; }
    if (str_std[2] == 'U' || str_std[2] == 'D') { cube->twisted[4] = 0; } if(str_std[20] == 'U' || str_std[20] == 'D') { cube->twisted[4] = 1; } if(str_std[27] == 'U' || str_std[27] == 'D') { cube->twisted[4] = 2; }
    if (str_std[26] == 'U' || str_std[26] == 'D') { cube->twisted[5] = 2; } if(str_std[33] == 'U' || str_std[33] == 'D') { cube->twisted[5] = 1; } if(str_std[53] == 'U' || str_std[53] == 'D') { cube->twisted[5] = 0; }
    if (str_std[35] == 'U' || str_std[35] == 'D') { cube->twisted[6] = 2; } if(str_std[42] == 'U' || str_std[42] == 'D') { cube->twisted[6] = 1; } if(str_std[51] == 'U' || str_std[51] == 'D') { cube->twisted[6] = 0; }
    if (str_std[0] == 'U' || str_std[0] == 'D') { cube->twisted[7] = 0; } if(str_std[36] == 'U' || str_std[36] == 'D') { cube->twisted[7] = 2; } if(str_std[29] == 'U' || str_std[29] == 'D') { cube->twisted[7] = 1; }
    
    
    if (is_solvable(cube) == 0) {
        printf("Cube non solvable\n");
    }
    free(str);
    free(str_std);
    return cube;
}