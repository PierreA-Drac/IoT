#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define NBCERCLE 3
#define NBEQUIP 5
#define NBTOUR 10
#define NBCAN 2
#define NBSF 3

typedef struct {
	int numSf;
	int numCan;
	float valeur;

}Ttour;

typedef struct
{
	int fois[NBCAN][NBSF];
	Ttour tour[NBTOUR];
	float moyenne[NBCAN][NBSF];
}Tgain;



void init_tab(int tab[NBCAN][NBSF]);
float calc_moyenne(Tgain gainTour, int count);
void calc_best_can_sf(int* can,int* sf, Tgain gain, int count);


int main() {
	Tgain gainTour[NBEQUIP]; 
	int nbPaquet=0, count=0, can, sf;
	int position[NBEQUIP];
	int choix[NBCAN][NBSF];
	srand(time(NULL)); 
	/*Initialisation position des equipement*/
	for(int i=0;i<NBEQUIP;i++) {
		position[i]=rand()%NBCERCLE;
	}
	init_tab(choix);
	for(int i=0;i<NBEQUIP;i++) 
		init_tab(gainTour[i].fois);
		
	for(int j=0;j<NBEQUIP;j++) {
		for( int k=0;k<NBCAN;k++){
			for(int l=0;l<NBSF;l++) {
				gainTour[j].moyenne[k][l]=0;
			}
		}
	}
	while(count<NBTOUR){
		init_tab(choix);
		for(int j=0;j<NBEQUIP;j++) {
			/*choix du canal et de SF pour j*/
			if (count==0) {
				can=rand()%NBCAN;
				sf=rand()%(NBSF-position[j])+position[j];
			}
			else {
				calc_best_can_sf(&can,&sf, gainTour[j], count);
			}
			if(choix[can][sf] == 0){ //pas de collision
				nbPaquet++;
				gainTour[j].tour[count].valeur=0.9;
				choix[can][sf]=1; 
			}
			else{
				gainTour[j].tour[count].valeur=0.1;
			}
			
			gainTour[j].tour[count].numCan=can;
			gainTour[j].tour[count].numSf=sf;
			gainTour[j].moyenne[can][sf]=calc_moyenne(gainTour[j],count);
			gainTour[j].fois[can][sf]++;
		}
		count++;
	}
	printf("nbPaquet: %d \n",nbPaquet);
	return 0;
}

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

void init_tab(int tab[NBCAN][NBSF]) {
	for( int i=0;i<NBCAN;i++){
		for(int j=0;j<NBSF;j++)
			tab[i][j]=0;
	}
}


void calc_best_can_sf(int *ptCan,int *ptSf, Tgain gain, int count) {
	float tmp;
	int result=0;
	for(int i=0;i<NBCAN;i++) {
		for(int j=0;j<NBSF;j++) {
			if(gain.fois[i][j]!=0) {
				tmp= gain.moyenne[i][j] + sqrt( (2*log(count) )/ gain.fois[i][j]);
				if(result<tmp){
					*ptCan =i;
					*ptSf =j;
				}
			}
		}
	}
}
