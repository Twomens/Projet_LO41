#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBEROFQUANTUM 100
#define NUMBEROFFILESPRIORITY 11

int * initProcessAllocTable(void);

int main()
{
    initProcessAllocTable();
    return 0;
}

int * initProcess(void){

}

int * initProcessAllocTable(void){

	int processAllocTable_Percentage [2][NUMBEROFFILESPRIORITY];//no need of 2 lines enough with the indice
	int processAllocTable_Quantum [NUMBEROFQUANTUM];
	int setOfValue [NUMBEROFFILESPRIORITY] = {26,20,15,10,8,6,5,4,3,2,1};
	int percentageRemaining=100,sumPercentage=0;

	char inputChar='0';

	srand(time(NULL));

	printf("Allocation Table Process :\n\nYou have 0 to 10 priority levels\nHere is the basic distribution :\n");
	printf("(Format : priority-percentage of time allocated tab)\n");
	for(int i=0;i<NUMBEROFFILESPRIORITY;i++){
		processAllocTable_Percentage[0][i]=i;
		processAllocTable_Percentage[1][i]=setOfValue[i];
		printf("%i-%i%\t",processAllocTable_Percentage[0][i],processAllocTable_Percentage[1][i]);		
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
			for(int i=0;i<NUMBEROFFILESPRIORITY;i++){
				processAllocTable_Percentage[1][i]=rand() % (int)(percentageRemaining/2.5);//not a real alea but with it you'll have better set of values
				percentageRemaining-=processAllocTable_Percentage[1][i];
				printf("%i-%i%\t",processAllocTable_Percentage[0][i],processAllocTable_Percentage[1][i]);		
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
			for(int i=0;i<NUMBEROFFILESPRIORITY;i++){
				printf("%hd : ",i);
				scanf("%d",&processAllocTable_Percentage[1][i]);	
			}
			printf("Bilan : \n");
			for(int i=0;i<NUMBEROFFILESPRIORITY;i++){
				sumPercentage+=processAllocTable_Percentage[1][i];
				printf("%hd : %hd%\t",i,processAllocTable_Percentage[1][i]);
			}
			if(sumPercentage!=100)printf("Invalid set of value the sum does'nt make 100%\n");
		}while(sumPercentage!=100);
	}

	
	printf("\n\nYOUR SET OF VALUES HAS BEEN CORRECTLY SETTlED\n\n");
	return 1;
}

int * initQueue(void){

}