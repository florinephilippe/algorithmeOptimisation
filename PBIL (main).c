#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const long long int poid_max=9906309440;
const float TA=0.95; // taux d'apprentissage de 95%
const float TAM=0.1;// taux de mutation de 10%

int** creation_tab_bdr(int nb_objet);
///création d'un tableau contenant la valeur et le poid pour chaque objet

void initialisation_bdr(int** tab_bdr, char * fichier);
///Récupère les valeurs et les poids contenus dans le fichier csv

float * creation_vecteur_de_prob(int nb_objet);
///vecteur de probabilité de la taille de n objets initialisés à 0.5

int ** creation_population(int nb_objet, int population);
///Création d'une matrice (population;nb_objet) : pour chaque individu une combinaison propre des objets de la base

int ** nouvelle_generation(int ** tab_generation, float * vecteur_de_prob, int population, int nb_objet);
///Cré une génération d'individus en fonction du vecteur de probabilité

int selection_individus(int ** tab_generation, int ** tab_bdr, int population, int nb_objet);
///Evalue et sélectionne le meilleur individu

float * actualisation_vecteur(int indice, int ** individus_valeur, float * vecteur_de_prob, int nb_objet);
///Met à jour le vecteur de probabilités en fonction du taux d’apprentissage et de l'indice du meilleur individu de la génération

float * mutation_vecteur(float * vecteur_de_prob, int nb_objet);
///Met à jour le vecteur de probabilités en fonction du taux d’apprentissage pour la mutation


int main()
{
    srand(time(NULL));
    int nb_objet=200; //il y a 200 objets à évaluer
    int i;
    int j;
    int population=1000; //nous choisissons une population de 1000 individus
    int ** tab_bdr; // déclaration d'un tableau permettant d'obtenir un tableau répertoriant la valeur et le poid pour chaque objet
    tab_bdr=creation_tab_bdr(nb_objet);//création du tableau bdr
    initialisation_bdr(tab_bdr,"valeurpoid.prn");
    /* Affichage du tableau des valeurs et des poids pour chacun des 200 objets*/
    printf("poids\t\tvaleur\n");
    for(i=0;i<nb_objet;i++)
    {
        printf("%d\t", tab_bdr[i][0]);
        printf("%d\n", tab_bdr[i][1]);
    }

    int indice; // nous déclarons un indice qui permettra d'obtenir le meilleur individu d'une génération
    long long int somme_valeur=0; // somme_valeur nous permettra d'obtenir le score final du meilleur individu d'une génération
                        // initialisé à 0 pour être sûr de pouvoir rentrer dans la boucle while
    long long int somme_poids=0;

    float * vecteur_prob; // nous déclarons un vecteur qui répertorira la probabilité pour chaque objet d'être sélectionné
                            // pour chaque nouvelle génération
    vecteur_prob=creation_vecteur_de_prob(nb_objet); // crée et initialise à 0.5 le vecteur de probabilité

    int ** tab_generation; // tableau créant une génération : pour chaque individu, il y a une combinaison particulière des objets
                            //de tab_bdr
    tab_generation=creation_population(nb_objet, population); // création d'une population

    while (somme_valeur<55000000){ //la somme des valeurs optimales est 74 443 865
        somme_valeur=0;
        somme_poids=0;
        tab_generation=nouvelle_generation(tab_generation,vecteur_prob,population, nb_objet); //création d'une génération grâce au vecteur

        indice=selection_individus(tab_generation, tab_bdr, population, nb_objet); // évalue chaque individu de la génération et séléctionne
                                                                                    // l'indice du meilleur individu

        vecteur_prob=actualisation_vecteur(indice, tab_generation, vecteur_prob, nb_objet); // actualise le vecteur de probabilité grâce à

        vecteur_prob=mutation_vecteur(vecteur_prob, nb_objet); // mutation du vecteur de probabilité

        for(j=0;j<nb_objet;j++){
            if(tab_generation[indice][j]==1){
                somme_valeur=somme_valeur+tab_bdr[j][1]; // compte la somme des valeurs de chaque objet
            }
        }
       printf("%lli\n", somme_valeur);
    }

    printf("\n\n\n\n");
    for(i=0;i<nb_objet;i++)
    {
        printf("objet %d : %d\n", i, tab_generation[indice][i]);
    }
    printf("valeur : %lli\tpoids : %lli\t%lli\n",somme_valeur, somme_poids, poid_max);

    return 0;
}



/*Création d'une matrice (nb_objet;2) comportant le poid et la valeur pour chaque objet*/

int ** creation_tab_bdr(int nb_objet){
    int ** tab_bdr;
    tab_bdr=(int **)malloc(sizeof(int*)*nb_objet);
    for (int i=0; i<nb_objet ; i++){
        tab_bdr[i]=(int *)malloc(sizeof(int)*2);
    }
    return tab_bdr;
}

/*Initialisation du tableau comportant le poid et la valeur pour chaque objet*/
void initialisation_bdr(int** tab_bdr, char * fichier){
    int i=0;
    int c;
    FILE * open_file=NULL;
        open_file=fopen(fichier,"r");
        if (open_file==NULL)
        {
            printf("Erreur lors de la creation du fichier.");
            exit(EXIT_FAILURE);
        }

        while (c!=EOF)
        {
            c=fscanf(open_file,"%d", &tab_bdr[i][0]);
            c=fscanf(open_file, "%d",&tab_bdr[i][1]);
            i++;
        }
        fclose(open_file);
}

/*création et initialisation du vecteur de probabilité de taille n objet*/

float * creation_vecteur_de_prob(int nb_objet){
    float * vecteur=(float *)malloc(sizeof(float)*nb_objet);
    int i;
    for (i=0; i<nb_objet; i++){
        vecteur[i]=0.5;
    }
    return vecteur;
}

/*création d'une population d'individus : chaque individu de taille n objet est une combinaison de ces n objets
la population comporte population individus*/

int ** creation_population(int nb_objet, int population){
    int ** tab_generation;
    int i;
    tab_generation=(int **)malloc(sizeof(int*)*population);
    for (i=0; i<population; i++){
        tab_generation[i]=(int *)malloc(sizeof(int)*nb_objet);
    }
    return tab_generation;
}

/*création d'une nouvelle génération d'individus grâce au vecteur de probabilité*/

int ** nouvelle_generation(int ** tab_generation, float * vecteur_de_prob, int population, int nb_objet){
    int i, j;
    float k;
    for (i=0; i<population; i++){
        for (j=0; j<nb_objet;j++){
            k=rand()/(RAND_MAX+1.0);
            if(k<vecteur_de_prob[j]) k=0;
            else k=1;
            tab_generation[i][j]=k;
        }
    }
    return tab_generation;
}

// sélection du meilleur individu de chaque génération
int selection_individus(int ** tab_generation, int ** tab_bdr, int population, int nb_objet)
{
    int individu, objet;
    long long int somme_valeur=0;
    long long int somme_poid=0;
    int meilleur_ind;
    long long int score_max=0;
    for (individu=0; individu<population;individu++){
        for (objet=0; objet<nb_objet;objet++){
            if (tab_generation[individu][objet]==1){
                somme_valeur+=tab_bdr[objet][1];
                somme_poid+=tab_bdr[objet][0];
            }
        }
        if(somme_poid<poid_max){
            if(somme_valeur>score_max){
                score_max=somme_valeur;
                meilleur_ind=individu;
            }
        }
        somme_poid=0;
        somme_valeur=0;
    }
    return meilleur_ind;
}

// actualisation du vecteur de probabilité en fonction du meilleur individu
float * actualisation_vecteur(int meilleur_ind, int ** tab_generation, float* vecteur_de_prob, int nb_objet)
{
    int objet;
    for (objet=0; objet<nb_objet; objet++){
        vecteur_de_prob[objet]=vecteur_de_prob[objet]*(1-TA)+TA*tab_generation[meilleur_ind][objet];
    }
    return vecteur_de_prob;
}

// mutation du vecteur de probabilié afin d'éviter que le score des populations stagne
float * mutation_vecteur(float* vecteur_de_prob, int nb_objet)
{
    for(int objet=0;objet<nb_objet;objet++)
    {
        vecteur_de_prob[objet]=vecteur_de_prob[objet]*(1-TAM)+TAM*(rand()/(RAND_MAX+1.0));
    }
    return vecteur_de_prob;
}

