#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>
#include <pthread.h>

#define NUMBEROFQUANTUM 100
#define NUMBEROFFILESPRIORITY 11
#define NUMBEROFQUANTUMEXECUTED 25

int numberOfThread = 0,threadBusy = 0;
pthread_mutex_t mutexThreadBusy = PTHREAD_MUTEX_INITIALIZER;

//struct who has all the parameters needed to manipulate the thread
struct threadProperties{
	int idThread;
	int submissionDate;
	int duration;
	int priority;
	pthread_t thread;
	pthread_cond_t condThread;
	pthread_mutex_t mutexCond;
};

//struct that contains a specific thread
struct Element{
	struct threadProperties *threadProperties;
	struct Element *nextElement;
};

//Beginning of the queue
struct Queue{
	struct Element *firstElement;
};

//function that's accomplish by each thread
void *thread(void *arg){
	struct threadProperties *structArg=arg;
	structArg->condThread=(pthread_cond_t)PTHREAD_COND_INITIALIZER;//cond and mutex initializer 
	structArg->mutexCond=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	printf("Je suis le thread n°%hd! subDate : %hd duration : %hd priority : %hd \n",structArg->idThread,structArg->submissionDate,structArg->duration,structArg->priority);
	while(structArg->duration > 0){//while the thread has to be executed
		pthread_mutex_lock (&structArg->mutexCond);
		printf("Thread %d s'endort\n",structArg->idThread);
		pthread_cond_wait (&structArg->condThread,&structArg->mutexCond);//the thread sleep here
		printf("Thread %d se réveil\n",structArg->idThread);
		pthread_mutex_unlock (&structArg->mutexCond);
		sleep(1);//equivalent to the execution of a task (and also to have a better dispaly)
		pthread_mutex_lock(&mutexThreadBusy);
		threadBusy=0;//flag used to know if the execution is finished =0 free =1 a thread is running
		pthread_mutex_unlock(&mutexThreadBusy);
	}
	printf("LE THREAD SE DETRUIT\n");
	pthread_exit(NULL);//kill the thread
}

//function to know how many thread the user want
void queryNbThread(void){
	srand(time(NULL));//initialization of time for rand function

	printf("Please select the number of thread that you want to create : ");
	scanf("%d",&numberOfThread);
	rewind(stdin);//free the input buffer
	while(numberOfThread < 1  || numberOfThread > 10000){//the fact that we use a %d forbid character so if you enter a character you'll get '0'
		printf("Invalid input choose a number between 1 and 1000 : %hd",numberOfThread);
		numberOfThread=1;
		scanf("%d",&numberOfThread);
		rewind(stdin);
	}
}

//function to initialize all the thread
void initThread(struct threadProperties *tabStructThreadProperties[]){
	if(numberOfThread!=7){//you go there in every case except 7 which an easter egg 
		for(int i=0;i<numberOfThread;i++){//to initialize the structure of all the threads

			tabStructThreadProperties[i]=malloc(sizeof(tabStructThreadProperties[i]));
			tabStructThreadProperties[i]->idThread=i;
			tabStructThreadProperties[i]->submissionDate=rand() % 3;
			tabStructThreadProperties[i]->duration=(rand() % 10)+1;
			tabStructThreadProperties[i]->priority=rand() % NUMBEROFFILESPRIORITY;
			//not working here so you do it inside the thread
			//tabStructThreadProperties[i]->condThread=(pthread_cond_t)PTHREAD_COND_INITIALIZER;
			//tabStructThreadProperties[i]->mutexCond=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
			if(pthread_create(&tabStructThreadProperties[i]->thread,NULL, thread, tabStructThreadProperties[i])){//you create all threads
				perror("ERROR ON THREAD_CREATION");
				//return EXIT_FAILURE;
			}
		}
	}else{//easter egg jeu de data du cours
		for(int i=0;i<numberOfThread;i++){

			tabStructThreadProperties[i]=malloc(sizeof(tabStructThreadProperties[i]));
			tabStructThreadProperties[i]->idThread=i;
			
		//} if you don't do that with if it's not working
		if(i==0){
			tabStructThreadProperties[0]->submissionDate=0;
			tabStructThreadProperties[0]->duration=7;
			tabStructThreadProperties[0]->priority=0;
		}else if (i==1){
			tabStructThreadProperties[1]->submissionDate=0;
			tabStructThreadProperties[1]->duration=4;
			tabStructThreadProperties[1]->priority=1;
		}else if(i==2){
			tabStructThreadProperties[2]->submissionDate=1;
			tabStructThreadProperties[2]->duration=6;
			tabStructThreadProperties[2]->priority=1;
		}else if(i==3){
			tabStructThreadProperties[3]->submissionDate=2;
			tabStructThreadProperties[3]->duration=1;
			tabStructThreadProperties[3]->priority=2;
		}else if(i==4){
			tabStructThreadProperties[4]->submissionDate=1;
			tabStructThreadProperties[4]->duration=2;
			tabStructThreadProperties[4]->priority=3;
		}else if(i==5){
			tabStructThreadProperties[5]->submissionDate=1;
			tabStructThreadProperties[5]->duration=4;
			tabStructThreadProperties[5]->priority=0;
		}else if(i==6){
			tabStructThreadProperties[6]->submissionDate=2;
			tabStructThreadProperties[6]->duration=1;
			tabStructThreadProperties[6]->priority=2;
		}
		//for(int i=0;i<numberOfThread;i++){
			if(pthread_create(&tabStructThreadProperties[i]->thread,NULL, thread, tabStructThreadProperties[i])){
				perror("ERROR ON THREAD_CREATION");
			}
		}
	}
	printf("\n\n");
	sleep(1);//just for a better display
}

//function to initialize  the process alloc table
int * initProcessAllocTable(int processAllocTable_Quantum []){

	int processAllocTable_Percentage [2][NUMBEROFFILESPRIORITY];//tabAllocCPU in percentage
	int setOfValue [NUMBEROFFILESPRIORITY] = {26,20,15,10,8,6,5,4,3,2,1}; //a basic set of value if you don't want to generate one
	int percentageRemaining=100,sumPercentage=0,cptFilePriority=0;

	char inputChar='0';

	srand(time(NULL));// to initialize time for rand func'

	//easter egg suite to have the percentage of TD
	if(numberOfThread==7){
		for(int i=0;i<NUMBEROFFILESPRIORITY;i++)setOfValue[i]=0;
		setOfValue[0]=40;
		setOfValue[1]=30;
		setOfValue[2]=20;
		setOfValue[3]=10;
	} 

	printf("Allocation Table Process :\n\nYou have 0 to 10 priority levels\nHere is the basic distribution :\n");
	printf("(Format : priority-percentage of time allocated tab)\n");
	for(int i=0;i<NUMBEROFFILESPRIORITY;i++){
		processAllocTable_Percentage[0][i]=i;//id of the queue
		processAllocTable_Percentage[1][i]=setOfValue[i];//basic values
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
				//inputChar=getchar();
				scanf("test %c",&inputChar);//-------------------------------------------------------------------------- on part dans le néant ici | non bloquant ca depend
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
	//to convert a table of a percentage into quantum
	cptFilePriority=0;
	for(int i=0;i<NUMBEROFQUANTUM;i++){//-------------------------------------------------------------------problem only work with a hundred quantum and delete the tab CPU_percentage
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

//function to add an element in a specific queue at the end
void insertElement(struct Queue *queue, struct threadProperties *threadProperties){
	struct Element *newElement = malloc(sizeof(*newElement));

	newElement->threadProperties=threadProperties;
	newElement->nextElement = queue->firstElement;
	queue->firstElement = newElement;
}

//function to get and delete the oldest element of a queue you return a pointer on the struct of the thread elected (-1 if the queue is empty)
struct threadProperties *getOldestElement(struct Queue *queue){
	struct Element *currentElement = queue->firstElement;
	int nb=0;
	struct threadProperties *Thread;

	if(queue->firstElement == NULL){//if there is nobody in the queue you transform the first element into a null one by changing it's id at -1
		printf("Queue empty\n");
		Thread=malloc(sizeof(struct threadProperties));
		Thread->idThread=-1;
		return Thread; 
	}
	if(queue->firstElement->nextElement ==NULL){//case of the thread collected is the last element of the queue
		Thread=queue->firstElement->threadProperties;
		printf("Thread Elected : %hd\n",queue->firstElement->threadProperties->idThread);
		queue->firstElement=NULL;
		free(queue->firstElement);
		return Thread;
	}
	while(currentElement->nextElement->nextElement!=NULL){//to go at the end of the queue (to get the oldest one)
		currentElement = currentElement->nextElement;
	}
	Thread=currentElement->nextElement->threadProperties;
	printf("Thread Elected : %hd\n",currentElement->nextElement->threadProperties->idThread);
	free(currentElement->nextElement);
	currentElement->nextElement=NULL;
	return Thread;
}

//function to display all the element of the queue
void displayQueue(struct Queue *queue){
    struct Element *currentElement = queue->firstElement;

    while (currentElement != NULL)//you travel all the elements
    {
        printf("%d -> ", currentElement->threadProperties->idThread);
        currentElement = currentElement->nextElement;
    }
    printf("END\n");
}

//function that check the entrance of all the thread that are waiting in queues
void executeWaitingQueue(struct Queue *waitForEnterringQueue,struct Queue *queue[NUMBEROFFILESPRIORITY-1]){
	struct Element *currentElement = waitForEnterringQueue->firstElement;
	struct Element *pastElement=currentElement;

	//you have to do an other procedure for the first element of the queue because he should not be delete otherwise you delete the queue
	if(currentElement == NULL){//nobody left in the waitEnterringQueue
		printf("Waitenteringqueue is empty ");
	}else if(currentElement->nextElement == NULL && currentElement->threadProperties->submissionDate<=0){//if this is the last element and it go out
		for(int y=0; y<NUMBEROFFILESPRIORITY ; y++){
    		if(currentElement->threadProperties->priority == y){
    			insertElement( queue[y] ,currentElement->threadProperties);
    		}
    	}
		waitForEnterringQueue->firstElement=NULL;
		free(currentElement->nextElement);
	}else if(currentElement->threadProperties->submissionDate<=0){//if the first element need to g out but they other left
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
    while (currentElement != NULL){//you do the same for all the element

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
    displayQueue(waitForEnterringQueue);//at the end to display what the next queues are
}

/*---------------------------------------------------------MAIN---------------------------------------------------------*/

int main() {
	int *processAllocTable_Quantum=malloc(sizeof(int)*NUMBEROFQUANTUM);
	
	int currentQuantum=0,currentQueue=0,cptQueue=0;
	struct threadProperties * currentThread;

	queryNbThread();

	struct threadProperties *mainTabStructThreadProperties[numberOfThread];// you declare at this time bc before you don't know the nb of thread wanted and so its a static var
	
	initThread(mainTabStructThreadProperties);

    initProcessAllocTable(processAllocTable_Quantum);

    //creation and initialization of queues
    struct Queue *queue[NUMBEROFFILESPRIORITY-1];
    struct Queue *waitForEnterringQueue = malloc(sizeof(*waitForEnterringQueue));
    for(int i = 0; i<NUMBEROFFILESPRIORITY ; i++) { queue[i] = malloc(sizeof(*queue));}

    //to fill the queues the first time
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

    //to display the init queues
    printf("Init Queues : \n\n");
    displayQueue(waitForEnterringQueue);
    for(int i = 0; i<NUMBEROFFILESPRIORITY ; i++){
    	printf("Queue %hd : \n\n",i);
    	displayQueue(queue[i]);
    }
	
	//to display the tab CPU
	/*
    for(int i=0;i<NUMBEROFQUANTUM;i++){
    	printf("i : %hd , tab : %hd\n",i,processAllocTable_Quantum[i]);
    }*/
	
    while(currentQuantum!=NUMBEROFQUANTUMEXECUTED){//you choose the number of quantum that you want to execute at the top with NUMBEROFQUANTUMEXECUTED
    	printf("CURRENT QUANTUM %hd\n",currentQuantum);
    	if(currentQuantum>=NUMBEROFQUANTUM){
    		currentQuantum=0;
    	}
    	currentQueue=processAllocTable_Quantum[currentQuantum];//to get the queue that should be elected at the current quantum
    	currentThread = getOldestElement(queue[currentQueue]);//to get the oldest element of the queue (or the next ones)
    	
    	while(currentThread->idThread==-1 && cptQueue<NUMBEROFFILESPRIORITY-1){//if the queue is empty you go in the next queue until youfind one else if all te queu are empty 
    		cptQueue++;
    		free(currentThread);
    		if(currentQueue<=NUMBEROFFILESPRIORITY-2)currentQueue++;
    		else currentQueue=0;
    		currentThread = getOldestElement(queue[currentQueue]);
    	}

    	executeWaitingQueue(waitForEnterringQueue,queue);//to low and get the new elements that were waiting
    													 // you insert them before addig the actual thread running according to TD

    	if(cptQueue>=NUMBEROFFILESPRIORITY-1){//if all the queues are empty
    		printf("Nobody is remaining in queues\n");
    		currentQuantum++;
	    	executeWaitingQueue(waitForEnterringQueue,queue);
    		cptQueue=0;
    		continue;//jump
    	}
    	cptQueue=0;//reset the cpt
    	printf("CURRENT QUEUE %hd CURRENT THREAD %hd\n",currentQueue,currentThread->idThread);
    	

    	mainTabStructThreadProperties[currentThread->idThread]->duration--;//decrease the duration remaining of the thread running

    	pthread_mutex_lock(&mutexThreadBusy);
    	threadBusy=1;//flag that mean that a thread is running so th eprogram is busy
    	pthread_mutex_unlock(&mutexThreadBusy);

    	pthread_mutex_lock(&currentThread->mutexCond);
    	pthread_cond_signal(&currentThread->condThread);//you wake up the thread concerned
    	pthread_mutex_unlock(&currentThread->mutexCond);

    	while(threadBusy==1);//wait until the thread has finished

    	if(mainTabStructThreadProperties[currentThread->idThread]->duration > 0){//if the thread isn't finished you put it back in the queue corresponfing

    		if(currentQueue<=NUMBEROFFILESPRIORITY-2)insertElement(queue[currentQueue+1],mainTabStructThreadProperties[currentThread->idThread]);
    		else insertElement(queue[0],mainTabStructThreadProperties[currentThread->idThread]);
    	}else{//if th ethread is finished
    		printf("THREAD NB %hd IS DELETE\n",currentThread->idThread);
       	}
		
		printf("\n");
    	for(int i = 0; i<NUMBEROFFILESPRIORITY ; i++){//you print all the queues to have a summary
    		printf("Queue %hd : \n",i);
    		displayQueue(queue[i]);
    	}
    	currentQuantum++;//end of the quantum go on the next one
    	printf("\n\n");
    }
    return 0;
}


//optionnel ajouter des threads sur signal
//faire le main dans un processus