#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int * initProcessAllocTable(void);

int main()
{
    initProcessAllocTable();
    return 0;
}

int * initProcess(void){

}

int * initProcessAllocTable(void){

	int processAllocTable [2][11];
	int setOfValue [11] = {26,20,15,10,8,6,5,4,3,2,1};
	int percentageRemaining=100,sumPercentage=0;

	char inputChar='0';

	srand(time(NULL));

	printf("Allocation Table Process :\n\nYou have 0 to 10 priority levels\nHere is the basic distribution :\n");
	printf("(Format : priority-percentage of time allocated tab)\n");
	for(int i=0;i<11;i++){
		processAllocTable[0][i]=i;
		processAllocTable[1][i]=setOfValue[i];
		printf("%i-%i%\t",processAllocTable[0][i],processAllocTable[1][i]);		
	}
	printf("\n");

	printf("If you want to change the set of value tap 'y' either tap 'n'\n");
	scanf("%c",&inputChar);
	while(inputChar!='y' && inputChar!='n'){
		printf("Invalid input type 'y' for yes or 'n' for no : ");
		scanf("%c",&inputChar);
	}

	if(inputChar=='n') return 1;

	printf("If you want a random attribution tap 'r'\nIf you want to enter the value manually tap 'm' : ");
	scanf("%c",&inputChar);//problème pour vider le buffer stdin artificiellement corrigé----------------------------------------------
	scanf("%c",&inputChar);
	while(inputChar!='r' && inputChar!='m'){
		printf("Invalid input type 'r' or 'm' : ");
		scanf("%c",&inputChar);
	}
	
	if(inputChar=='r'){

		do{
			for(int i=0;i<11;i++){
				processAllocTable[1][i]=rand() % (int)(percentageRemaining/2.5);//not a real alea but with it you'll have better set of values
				percentageRemaining-=processAllocTable[1][i];
				printf("%i-%i%\t",processAllocTable[0][i],processAllocTable[1][i]);		
			}
			percentageRemaining=100;
			printf("Again ? [y/n]");
			scanf("%c",&inputChar);//same problem--------------------------------------------------------------------------
			scanf("%c",&inputChar);
		}while(inputChar!='n');

	}else if(inputChar=='m'){
		do{
			sumPercentage = 0;
			printf("Rentrez le pourcentage correspondant au niveau de priorité :\n");
			for(int i=0;i<11;i++){
				printf("%hd : ",i);
				scanf("%d",&processAllocTable[1][i]);	
			}
			printf("Bilan : \n");
			for(int i=0;i<11;i++){
				sumPercentage+=processAllocTable[1][i];
				printf("%hd : %hd%\t",i,processAllocTable[1][i]);
			}
			if(sumPercentage!=100)printf("Invalid set of value the sum does'nt make 100%\n");
		}while(sumPercentage!=100);
	}

	printf("\n\n YOUR SET OF VALUES HAS BEEN CORRECTLY SETTlED\n\n");
	return 1;
}

int * initQueue(void){

}