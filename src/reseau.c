#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define NBCERCLE 3
#define NBEQUIP 5
#define NBTOUR 10000
#define NBCAN 2
#define NBSF 3

/*structure qui contient les infos de chaque tour*/
typedef struct {
	int numSf;
	int numCan;
	float valeur;

}Ttour;
/*structure qui retrace l'historique de chaque equimement*/
typedef struct
{
	int fois[NBCAN][NBSF]; //conbient de fois chaque combinaison a été utilisé
	Ttour tour[NBTOUR]; //les valeurs obtenues pour chaque tour
	float moyenne[NBCAN][NBSF]; //moyenne de chaque combinaison au tour t
}Tgain;



void init_tab(int tab[NBCAN][NBSF], int val);
void init_moy(Tgain gainTour[NBEQUIP], int pos[NBEQUIP]);
float calc_moyenne(Tgain gainTour, int count);
void calc_best_can_sf(int* can,int* sf, Tgain gain, int count);


int main() {
	Tgain gainTour[NBEQUIP]; 
	int nbPaquet=0, count=1, can, sf;
	int position[NBEQUIP]; 
	int choix[NBCAN][NBSF]; // Connaitre si une combinaison est choisi deux fois dans un tour
	srand(time(NULL)); 
	/*Initialisation position des equipement sur un cercle*/
	for(int i=0;i<NBEQUIP;i++) {
		position[i]=rand()%NBCERCLE;
	}
	
	init_tab(choix, 0);
	/*On met que chaque combinaison a été utilisé une fois et que ca moyenne est de 1
	 * pour être sûre quelles vont toutes être choisit au moins une fois.*/
	for(int i=0;i<NBEQUIP;i++) 
		init_tab(gainTour[i].fois, 1);
	init_moy(gainTour,position);
	
	while(count<=NBTOUR){
		init_tab(choix,0); 
		/*A chque tour tous les equipements doivent envoyer un paquet*/
		for(int j=0;j<NBEQUIP;j++) {
			/*Calcul la meilleur combinaison */
			calc_best_can_sf(&can,&sf, gainTour[j], count);
			/*Pas de collision on transmet paquet, on ajoute la valeur et
			 * on "dit" que cette combinaison a déjà été choisie*/
			if(choix[can][sf] == 0){
				nbPaquet++;
				gainTour[j].tour[count].valeur=0.9;
				choix[can][sf]=1; 
			}
			/*Collision on ajoute juste la valeur*/
			else{
				gainTour[j].tour[count].valeur=0.1;
			}
			/*Dans tout les cas on ajoute la combinaison du tour, on calcul
			 * sa nouvelle moyenne et on augmente le nombre de fois qu'elle 
			 * a été choisit de 1*/
			gainTour[j].tour[count].numCan=can;
			gainTour[j].tour[count].numSf=sf;
			gainTour[j].moyenne[can][sf]=calc_moyenne(gainTour[j],count);
			gainTour[j].fois[can][sf]++;
		}
		count++;
	}
	printf("%d \n",nbPaquet);
	return 0;
}

/*Calcul de la moyenne d'une combinaison d'un équipement donc on cherche 
 *pour chaque tout si c'est cette combinaison qui a été utilisé. */
float calc_moyenne(Tgain gainTour, int count){
	float result=0;
	int nb=0;
	for(int i=0;i<=count;i++)
	{
		if(gainTour.tour[i].numCan==gainTour.tour[count].numCan && gainTour.tour[i].numSf==gainTour.tour[count].numSf) {
			result+=gainTour.tour[i].valeur;
			nb++;
		}
	}
	result=result/nb;
	return result;
}

void init_tab(int tab[NBCAN][NBSF], int val) {
	for( int i=0;i<NBCAN;i++){
		for(int j=0;j<NBSF;j++)
			tab[i][j]=val;
	}
}

void init_moy(Tgain gainTour[NBEQUIP], int position[NBEQUIP]) {
	for(int j=0;j<NBEQUIP;j++) {
		for( int k=0;k<NBCAN;k++){
			for(int l=0;l<NBSF;l++) {
				if (j<position[j])
					gainTour[j].moyenne[k][l]=-1;
				else
					gainTour[j].moyenne[k][l]=1;
			}
		}
	}
}

/*Calcul de la combinaison a choisir utilisation de la formule donnée dans 
 * le sujet.*/
void calc_best_can_sf(int *ptCan,int *ptSf, Tgain gain, int count) {
	float tmp;
	float result=0;
	int test;
	for(int i=0;i<NBCAN;i++) {
		for(int j=0;j<NBSF;j++) {
			tmp= gain.moyenne[i][j] + sqrt( (2*log(count) )/ gain.fois[i][j]);
			/*Si les deux resultats sont égaux on en garde un au hasard cela permet de reduire le nombre de collision*/
			if(result==tmp) {
				test=rand()%2;
				if(test==0){
					result=tmp;
					*ptCan=i;
					*ptSf=j;
				}
			}			
			else if(result<tmp){
				result=tmp;
				*ptCan=i;
				*ptSf=j;
			}
		}
	}
}
