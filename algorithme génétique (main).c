#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int nb_objet=200;
const int population=10000; // population de 10 000 individus
const long long int poids_max=9906309440;

// creation d'un tableau contenant le poids et la valeur pour chaque objet
int ** creation_tab_bdr(){
    int ** tab_bdr;
    tab_bdr=(int **)malloc(sizeof(int*)*nb_objet);
    for (int i=0; i<nb_objet ; i++){
        tab_bdr[i]=(int *)malloc(sizeof(int)*2);
    }
    return tab_bdr;
}

// intialisation du tableau contenant le poids et la valeur de chaque objet
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

// création d'une première population d'individus de taille n objets
int ** creation_population(){
    int ** tab_generation;
    int i, j;
    tab_generation=(int **)malloc(sizeof(int*)*population);
    for (i=0; i<population; i++){
        tab_generation[i]=(int *)malloc(sizeof(int)*nb_objet);
    }
    float k;
    for (i=0; i<population; i++){
        for (j=0; j<nb_objet;j++){
            k=rand()%2;
            tab_generation[i][j]=k;
        }
    }
    return tab_generation;
}

// tableau répertoriant le score de chaque individu
long long int * score_individus(long long int * score_ind, int ** tab_generation, int ** tab_bdr)
{
    int individu, objet;
    long long int somme_valeur=0;
    long long int somme_poid=0;
    for (individu=0; individu<population;individu++){
        for (objet=0; objet<nb_objet;objet++){
            if (tab_generation[individu][objet]==1){
                somme_valeur+=tab_bdr[objet][1];
                somme_poid+=tab_bdr[objet][0];
            }
        }
        if(somme_poid<poids_max){
            score_ind[individu]=somme_valeur;
        }
        else {
            score_ind[individu]=0;
        }
        somme_poid=0;
        somme_valeur=0;
    }
    return score_ind;
}

//tableau sélectionnant la meilleure moitié des individus
int * meilleure_moitie(long long int * score_ind, int * selection_moitie)
{
    int meilleur_score=0;
    int meilleur_individu;
    for(int i=0; i<population/2;i++){
        for(int individu=0; individu<population;i++){
            if (score_ind[individu]>meilleur_score){
                meilleur_individu=individu;
                meilleur_score=score_ind[individu];
            }
        }
        selection_moitie[i]=meilleur_individu;
        meilleur_score=0;
        score_ind[meilleur_individu]=0;
    }
    return selection_moitie;
}

//croisement des meilleures individus afin d'obtenir une nouvelle génération basée sur la moitié
//la plus performante dela génération précédente
int ** croisement_individus(int ** tab_generation, int * selection_moitie)
{
    for (int i=0; i<population/2;i++){
        for (int j=0; j<nb_objet; j++){
            tab_generation[i][j]=tab_generation[selection_moitie[i]][j];
        }
    }
    for (i=population/2; i<population;i++){
        for (j=0; j<nb_objet/2; j++){
            tab_generation[i][j]=tab_generation[selection_moitie[i-population/2]][j];
        }
        for (j=nb_objet/2;j<nb_objet; j++){
            tab_generation[i][j]=tab_generation[selection_moitie[population/2-i]][j];
        }
    }
    return tab_generation;
}

/*int ** croisement_individus1(int ** tab_generation, int * selection_moitie)
{
    int i, j;
    for (i=0; i<population;i++){
        for (j=0; j<nb_objet/2; j++){
            tab_generation[i][j]=tab_generation[selection_moitie[i]][j];
        }
        for (j=nb_objet/2;j<nb_objet; j++){
            tab_generation[i][j]=tab_generation[selection_moitie[population-i]][j];
        }
    }
    return tab_generation;
}*/

//mutation de la génération afin que les individus ne stagne pas (taux de mutation de 10%)
int ** mutation_generation(int ** tab_generation)
{
    int k;
    int i, j;
    for (i=0; i<population; i++){
        for (j=0; j<nb_objet; j++){
            k=rand()%101;
            if (k<=10){
                if (tab_generation[i][j]==1)tab_generation[i][j]=0;
                else tab_generation[i][j]=1;
            }
        }
    }
    return tab_generation;
}

int main()
{
    srand(time(NULL));
    int ** tab_bdr;
    int i, j;
    tab_bdr=creation_tab_bdr(nb_objet);
    initialisation_bdr(tab_bdr,"valeurpoid.prn");
    printf("poids\t\tvaleur\n");
    for(i=0;i<nb_objet;i++)
    {
        printf("%d\t", tab_bdr[i][0]);
        printf("%d\n", tab_bdr[i][1]);
    }
    long long int somme_valeur=0;
    long long int somme_poids=0;
    int ** tab_generation;
    tab_generation=creation_population(nb_objet, population);
    long long int * score_ind=(long long int *)malloc(sizeof(long long int)*population);
    int * selection_moitie=(int *)malloc(sizeof(int)*population/2);

     while (somme_valeur<55000000){
        somme_valeur=0;
        somme_poids=0;
        score_ind=score_individus(score_ind, tab_generation, tab_bdr);
        selection_moitie=meilleure_moitie(score_ind, selection_moitie);
        for(j=0;j<nb_objet;j++){
            if(tab_generation[selection_moitie[0]][j]==1){
                somme_valeur=somme_valeur+tab_bdr[j][1];
            }
        }
        printf("%lld\n", somme_valeur);
        tab_generation=croisement_individus(tab_generation, selection_moitie);
        tab_generation=mutation_generation(tab_generation);
     }
     printf("\n\n\n\n");
    for(i=0;i<nb_objet;i++)
    {
        printf("objet %d : %d\n", i, tab_generation[selection_moitie[0]][i]);
    }
    printf("valeur : %lld\tpoids : %lld\t%lld\n",somme_valeur, somme_poids, poids_max);
    return 0;
}
