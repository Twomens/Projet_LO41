#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>
#include <pthread.h>

#define NUMBEROFQUANTUM 100
#define NUMBEROFFILESPRIORITY 11

int numberOfThread = 0;

struct threadProperties{
	int idThread;
	int submissionDate;
	int duration;
	int priority;
	pthread_t thread;
	pthread_cond_t condThread;
	pthread_mutex_t mutexCond;
};

struct Element{
	struct threadProperties *threadProperties;
	struct Element *nextElement;
};

struct Queue{
	struct Element *firstElement;
};


void *thread(void *arg){
	struct threadProperties *structArg=arg;
		structArg->condThread=(pthread_cond_t)PTHREAD_COND_INITIALIZER;
		structArg->mutexCond=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	printf("Je suis le thread n°%hd! subDate : %hd duration : %hd priority : %hd \n",structArg->idThread,structArg->submissionDate,structArg->duration,structArg->priority);
	while(structArg->duration > 0){
		pthread_mutex_lock (&structArg->mutexCond);
		printf("Thread %d s'endort\n",structArg->idThread);
		pthread_cond_wait (&structArg->condThread,&structArg->mutexCond);
		printf("Thread %d se réveil\n",structArg->idThread);
		pthread_mutex_unlock (&structArg->mutexCond);
	}
	pthread_exit(NULL);
}

void queryNbThread(void){
	srand(time(NULL));

	printf("Please select the number of thread that you want to create : ");
	scanf("%d",&numberOfThread);
	rewind(stdin);
	while(numberOfThread < 1  || numberOfThread > 10000){//the fact that we use a %d forbid character so if you enter a character you'll get '0'
		printf("Invalid input choose a number between 1 and 1000 : %hd",numberOfThread);
		numberOfThread=1;
		scanf("%d",&numberOfThread);//---------------------------------------------------scanf non bloquant pk? prblm avec le int devient inutilisable
		rewind(stdin);
	}
}

void initProcess(struct threadProperties *tabStructThreadProperties[]){

	for(int i=0;i<numberOfThread;i++){

		tabStructThreadProperties[i]=malloc(sizeof(tabStructThreadProperties[i]));
		tabStructThreadProperties[i]->idThread=i;
		tabStructThreadProperties[i]->submissionDate=rand() % 3;
		tabStructThreadProperties[i]->duration=rand() % 11;
		tabStructThreadProperties[i]->priority=rand() % 11;
		//tabStructThreadProperties[i]->condThread=(pthread_cond_t)PTHREAD_COND_INITIALIZER;
		//tabStructThreadProperties[i]->mutexCond=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
		if(pthread_create(&tabStructThreadProperties[i]->thread,NULL, thread, tabStructThreadProperties[i])){
			perror("ERROR ON THREAD_CREATION");
			//return EXIT_FAILURE;
		}
		/*if(pthread_join(tabStructThreadProperties[i]->thread,NULL)){
			perror("ERROR ON THREAD_JOIN");
			//return EXIT_FAILURE;
		}*/
	}
	printf("\n\n");
	sleep(1);
}

int * initProcessAllocTable(int processAllocTable_Quantum []){

	int processAllocTable_Percentage [2][NUMBEROFFILESPRIORITY];//no need of 2 lines enough with the indice
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
	cptFilePriority=0;
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

void insertElement(struct Queue *queue, struct threadProperties *threadProperties){
	struct Element *newElement = malloc(sizeof(*newElement));

	newElement->threadProperties=threadProperties;
	newElement->nextElement = queue->firstElement;
	queue->firstElement = newElement;
}

struct threadProperties *getOldestElement(struct Queue *queue){
	struct Element *currentElement = queue->firstElement;
	int nb=0;
	struct threadProperties *Thread;

	if(queue->firstElement == NULL){
		printf("nobody\n");
		Thread=malloc(sizeof(struct threadProperties));
		Thread->idThread=-1;
		return Thread;
	}
	if(queue->firstElement->nextElement ==NULL){
		Thread=queue->firstElement->threadProperties;
		printf("Thread executed : %hd\n",queue->firstElement->threadProperties->idThread);
		queue->firstElement=NULL;
		free(queue->firstElement);
		return Thread;
	}
	while(currentElement->nextElement->nextElement!=NULL){
		currentElement = currentElement->nextElement;
	}
	Thread=currentElement->nextElement->threadProperties;
	printf("Thread nb : %hd\n",currentElement->nextElement->threadProperties->idThread);
	free(currentElement->nextElement);
	currentElement->nextElement=NULL;
	return Thread;
}

void displayQueue(struct Queue *queue){
    struct Element *currentElement = queue->firstElement;

    while (currentElement != NULL)
    {
        printf("%d -> ", currentElement->threadProperties->idThread);
        currentElement = currentElement->nextElement;
    }
    printf("END\n");
}

void executeWaitingQueue(struct Queue *waitForEnterringQueue,struct Queue *queue[NUMBEROFFILESPRIORITY-1]){ 
	struct Element *currentElement = waitForEnterringQueue->firstElement;
	struct Element *pastElement=currentElement;

	if(currentElement == NULL){
		printf("Waitenteringqueue is empty");
	}else if(currentElement->nextElement == NULL && currentElement->threadProperties->submissionDate<=0){
		for(int y=0; y<NUMBEROFFILESPRIORITY ; y++){
    		if(currentElement->threadProperties->priority == y){
    			insertElement( queue[y] ,currentElement->threadProperties);
    		}
    	}
		waitForEnterringQueue->firstElement=NULL;
		free(currentElement->nextElement);
	}else if(currentElement->threadProperties->submissionDate<=0){
		waitForEnterringQueue->firstElement=currentElement->nextElement;
		printf("\n %hd is sup_ \n",currentElement->threadProperties->idThread);
        for(int y=0; y<NUMBEROFFILESPRIORITY ; y++){
    		if(currentElement->threadProperties->priority == y){
    			insertElement( queue[y] ,currentElement->threadProperties);
    		}
    	}
		currentElement = currentElement->nextElement;
		pastElement=currentElement;
	}
    while (currentElement != NULL){

        if(currentElement->threadProperties->submissionDate<=0){
        	if(pastElement==currentElement){
        		waitForEnterringQueue->firstElement=currentElement->nextElement;
        		pastElement=waitForEnterringQueue->firstElement;
        	}
        	else pastElement->nextElement=currentElement->nextElement;
        	printf("\n %hd is sup \n",currentElement->threadProperties->idThread);
        	for(int y=0; y<NUMBEROFFILESPRIORITY ; y++){
    			if(currentElement->threadProperties->priority == y){
    				insertElement( queue[y] ,currentElement->threadProperties);
    			}
    		}
    		
        }else{
        	pastElement=currentElement;
    		currentElement->threadProperties->submissionDate--;
    	}
        currentElement = currentElement->nextElement;
    }
    displayQueue(waitForEnterringQueue);
}

/*---------------------------------------------------------MAIN---------------------------------------------------------*/

int main() {
	int *processAllocTable_Quantum=malloc(sizeof(int)*NUMBEROFQUANTUM);
	
	int currentQuantum=0,currentQueue=0;
	struct threadProperties * currentThread;

	queryNbThread();

	struct threadProperties *mainTabStructThreadProperties[numberOfThread];
	
	initProcess(mainTabStructThreadProperties);

    initProcessAllocTable(processAllocTable_Quantum);

    struct Queue *queue[NUMBEROFFILESPRIORITY-1];
    struct Queue *waitForEnterringQueue = malloc(sizeof(*waitForEnterringQueue));


    for(int i = 0; i<NUMBEROFFILESPRIORITY ; i++) { queue[i] = malloc(sizeof(*queue));}

    for(int i=0; i<numberOfThread ; i++){
    	if(mainTabStructThreadProperties[i]->submissionDate == 0){
    		for(int y=0; y<NUMBEROFFILESPRIORITY ; y++){
    			if(mainTabStructThreadProperties[i]->priority == y){
    				insertElement( queue[y] ,mainTabStructThreadProperties[i]);
    			}
    		}
    	}else if(mainTabStructThreadProperties[i]->submissionDate > 0){
    		mainTabStructThreadProperties[i]->submissionDate--;
    		insertElement( waitForEnterringQueue ,mainTabStructThreadProperties[i]);
    	}else{
    		printf("ERROR : a thread can't enter the queue during initialisation");
    	}
    }

    printf("Init_Wainting queue : \n\n");
    displayQueue(waitForEnterringQueue);
    for(int i = 0; i<NUMBEROFFILESPRIORITY ; i++){
    	printf("Queue %hd : \n\n",i);
    	displayQueue(queue[i]);
    }
	
	/*
    for(int i=0;i<NUMBEROFQUANTUM;i++){
    	printf("i : %hd , tab : %hd\n",i,processAllocTable_Quantum[i]);
    }*/
	
    while(currentQuantum!=10){
    	printf("CURRENT QUANTUM %hd\n",currentQuantum);
    	if(currentQuantum>=NUMBEROFQUANTUM){
    		currentQuantum=0;
    	}
    	currentQueue=processAllocTable_Quantum[currentQuantum];
    	currentThread = getOldestElement(queue[currentQueue]);
    	while(currentThread->idThread==-1){//																		PRBLM BOUCLE INFINIE SI PLUS PERSONNES DANS LES FILES
    		//																							on va a celle d'après suivre la table d'alloc processus ou pas?
    		free(currentThread);
    		if(currentQueue<=NUMBEROFFILESPRIORITY-2)currentQueue++;
    		else currentQueue=0;
    		currentThread = getOldestElement(queue[currentQueue]);
    	}
    	printf("CURRENT QUEUE %hd CURRENT THREAD %hd\n",currentQueue,currentThread->idThread);
    	

    	mainTabStructThreadProperties[currentThread->idThread]->duration--;
    	printf("LE THREAD VA ETRE REVEILLE\n");
    	pthread_cond_signal(&currentThread->condThread);
    	sleep(2);
    	if(mainTabStructThreadProperties[currentThread->idThread]->duration > 0){
    		if(currentQueue<=NUMBEROFFILESPRIORITY-2)insertElement(queue[currentQueue+1],mainTabStructThreadProperties[currentThread->idThread]);// 	PROVISOIRE NE FCT PAS
    		else insertElement(queue[0],mainTabStructThreadProperties[currentThread->idThread]);
    	}else{
    		printf("THREAD NB %hd IS DELETE\n",currentThread->idThread);
    		/*
    		SUP THE THREAD
    		*/
    	}

    	executeWaitingQueue(waitForEnterringQueue,queue);
		
    	for(int i = 0; i<NUMBEROFFILESPRIORITY ; i++){
    		printf("Queue %hd : \n",i);
    		displayQueue(queue[i]);
    	}
    	currentQuantum++;
    	printf("\n\n");
    }
    return 0;
}

//reste à faire : 
//trouver un oyen d'enlver l'attente active avec le sleep (au pire avec un flag que deverouille le thread sous protection mutex)
//regarder s'ils sont supprimés ( zone de suppression dans le main mais à priori thread se tue tout seul)
//corriger le prblm de la boucle infini si plus personne dans les files 


//optionnel ajouter des threads sur signal
//ajouter mutex pour protéger
//regarder l'intérêt de 1 mutex 1 cond par thread
//faire le main dans un processus
