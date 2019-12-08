#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>
#include <pthread.h>

#define NUMBEROFQUANTUM 100
#define NUMBEROFFILESPRIORITY 11

int * initProcessAllocTable(void);
int * initProcess(void);
//void insertElement(struct Queue *queue, int nbre);
//int getOldestElement(struct Queue *queue);


struct threadProperties{
	int idThread;
	int submissionDate;
	int duration;
	int priority;
};
struct Element{
	int nombre;
	struct Element *nextElement;
};
struct Queue{
	struct Element *firstElement
};

int main()
{
	initProcess();
    //ProcessAllocTable();
    struct Queue *queue = malloc(sizeof(*queue));
    insertElement(queue,1);
    insertElement(queue,2);
    insertElement(queue,3);
    insertElement(queue,4);
    insertElement(queue,5);
    getOldestElement(queue);
    getOldestElement(queue);
    return 0;
}

void *thread(void *arg){
	struct threadProperties *structArg=arg;
	printf("Je suis le thread n°%hd! subDate : %hd duration : %hd priority : %hd \n",structArg->idThread,structArg->submissionDate,structArg->duration,structArg->priority);
	
	pthread_exit(NULL);
}

int * initProcess(void){
	int numberOfProcessus = 0;

	srand(time(NULL));

	printf("Please select the number of processus that you want to create : ");
	scanf("%d",&numberOfProcessus);
	rewind(stdin);
	while(numberOfProcessus < 1  || numberOfProcessus > 10000){//the fact that we use a %d forbid character so if you enter a character you'll get '0'
		printf("Invalid input choose a number between 1 and 1000 : %hd",numberOfProcessus);
		numberOfProcessus=1;
		scanf("%d",&numberOfProcessus);//---------------------------------------------------scanf non bloquant pk? prblm avec le int devient inutilisable
		rewind(stdin);	
		//return 1;
	}
	
	pthread_t tabThread[numberOfProcessus];
	struct threadProperties *tabStructThreadProperties[numberOfProcessus];

	for(int i=0;i<numberOfProcessus;i++){
		tabStructThreadProperties[i]=malloc(sizeof(int)*4);

		tabStructThreadProperties[i]->idThread=i;
		tabStructThreadProperties[i]->submissionDate=rand() % 5;
		tabStructThreadProperties[i]->duration=rand() % 11;
		tabStructThreadProperties[i]->priority=rand() % 11;
		
		if(pthread_create(&tabThread[i],NULL, thread, tabStructThreadProperties[i])){
			perror("ERROR ON THREAD_CREATION");
			return EXIT_FAILURE;
		}

		if(pthread_join(tabThread[i],NULL)){
			perror("ERROR ON THREAD_JOIN");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int * initProcessAllocTable(void){

	int processAllocTable_Percentage [2][NUMBEROFFILESPRIORITY];//no need of 2 lines enough with the indice
	int processAllocTable_Quantum [NUMBEROFQUANTUM];
	int setOfValue [NUMBEROFFILESPRIORITY] = {26,20,15,10,8,6,5,4,3,2,1};
	int percentageRemaining=100,sumPercentage=0,cptFilePriority=0;

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

	if(inputChar=='n'){} 
	else{

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
	}

	for(int i=0;i<NUMBEROFQUANTUM;i++){//-------------------------------------------------------------------problem only work with a hundred quantum and deleate the tab CPU_percentage
		while(processAllocTable_Percentage[1][cptFilePriority] == 0){//if you don't have anymore value in the tab cpu
			if(cptFilePriority>=NUMBEROFFILESPRIORITY-1) cptFilePriority=0;//to avoid outpassing the max value of list
			else cptFilePriority++;//if you don't ahve anymore quantum remaining in this file you skip it
		}
		processAllocTable_Quantum[i]=cptFilePriority;//give the indice of the file that is working on each quantum
		processAllocTable_Percentage[1][cptFilePriority]--;//delete the quantum affected
		cptFilePriority++;//go to the next file
		if(cptFilePriority>=NUMBEROFFILESPRIORITY-1) cptFilePriority=0;//to go back on the first file
	}

	printf("\n\nYOUR SET OF VALUES HAS BEEN CORRECTLY SETTlED\n\n");
	return EXIT_SUCCESS;
}

void insertElement(struct Queue *queue, int nbre){
	struct Element *newElement = malloc(sizeof(*newElement));

	newElement->nombre = nbre;
	newElement->nextElement = queue->firstElement;
	queue->firstElement = newElement;
}

int getOldestElement(struct Queue *queue){
	struct Element *currentElement = queue->firstElement;
	int nb=0;

	if(queue->firstElement == NULL){
		printf("nobody\n");
		return -1;
	}
	if(queue->firstElement->nextElement ==NULL){
		nb=queue->firstElement->nombre;
		queue->firstElement=NULL;
		free(queue->firstElement);
		printf(" nb : %hd\n",nb);
		return nb;
	}
	while(currentElement->nextElement->nextElement!=NULL){
		currentElement = currentElement->nextElement;
	}
	nb=currentElement->nextElement->nombre;
	free(currentElement->nextElement);
	currentElement->nextElement=NULL;
	printf(" nb : %hd\n",nb);
	return nb;
}