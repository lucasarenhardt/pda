#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack.h"

//structure to create nodes of the linked list responsible for computation
typedef struct ComputationStep{
    struct ComputationStep *next;
    char word[101], stackStatus[10001];
    int state;
}compStep;

//structure to create a node of the adjacency list
typedef struct PathNode{
    struct PathNode *next;     //points to the next node
    char received, pop, push[101];   //stores what is received and what is to be pushed/popped  
    int index;                  //represents which state the node is
}pathNode;

//structure to create a Head of the adjacency list (states or left elements)
typedef struct StateNode{
    pathNode *paths;                //points to the nodes forming the adjacency list
    int flag;                   //represents if it's final or not
}stateNode;

typedef struct Graph{
    int nVertex;                //stores the number of states of the PDA
    stateNode **states;         //array of pointers to the Heads
}graph;

//creates a new element of the linked list
compStep *createCompStep(int state, char *word, char *stackStatus){
    compStep *new = (compStep*)malloc(sizeof(compStep));
    new->next = NULL;
    new->state = state;
    strcpy(new->word, word);
    strcpy(new->stackStatus, stackStatus);

    return new;
}

//creates and adds an element at the end of the list
void addComp(compStep *comp, int state, char *word, char *stackStatus){
    compStep *current = comp;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = createCompStep(state, word, stackStatus);
    current->next->next = NULL;
}

//deletes the last element of the linked list
void deleteComp(compStep **comp){
    compStep *current = *comp;
    while(current->next->next != NULL){
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}

//prints the entire linked list
void printComp(compStep *comp){
    compStep *current = comp;
    while(current != NULL){
        if(current->next != NULL)
            printf("(q%d, %s, %s) |-\n", current->state, current->word, current->stackStatus);
        else
            printf("(q%d, %s, %s).\n\n", current->state, current->word, current->stackStatus);
        current = current->next;
    }
}

//frees the elements of the linked list
void freeComp(compStep *comp){
    compStep *current = comp;
    while(current != NULL){
        compStep *next = current->next;
        free(current);
        current = next;
    }
}

//creates a new Head (state)
stateNode *createStateNode(){
    stateNode *new = (stateNode*)malloc(sizeof(stateNode));
    new->flag = 0;
    new->paths = NULL;
    return new;
}

//creates a new node of the Head's list (paths)
pathNode *createPathNode(){
    pathNode *new = (pathNode*)malloc(sizeof(pathNode));
    new->received = '\0';
    new->pop = '\0';
    new->push[0] = '\0';
    new->index = -1;
    new->next = NULL;
    return new;
}

//creates a graph
graph *createGraph(int Q){
    graph *new = (graph*)malloc(sizeof(graph));
    new->nVertex = Q;
    new->states = (stateNode**)malloc(Q * sizeof(stateNode*));

    for(int i = 0; i < Q; i++){
        new->states[i] = createStateNode();
    }

    return new;
}

//adds a path to the graph
graph *addPath(graph *g, int i, char rec, char pop, char *push, int f){
    pathNode *new = createPathNode(); //creates a new node of the list (path)

    new->received = rec;        
    new->pop = pop;
    strcpy(new->push, push);
    new->index = f;

    //checks if state i has any nodes. If so, assigns the new node as the first.
    //If not, iterates through the nodes of state i until finding one pointing to NULL, then assigns the new node as next
    if(g->states[i]->paths == NULL){
        g->states[i]->paths = new;
    } else {
        pathNode *temp = g->states[i]->paths;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new;
    }
    
    return g;
}

//modifies the final condition to True
graph *final(graph *g, int N){
    g->states[N]->flag = 1;
    return g;
}

//checks if the word is accepted or not
int verifyWord(stateNode **states, char *word, int wordIndex, int currentState, stack *s, compStep **comp){
    char temp;      
    char top = pop(s);                          //stores the information at the top of the stack
    push(s, top);                               //puts back the top
    int j = 0;

    char currentWord[101];                      //array to store the current word based on the wordIndex
    
    for (int i = wordIndex; i < strlen(word); i++) {    //copies the current word based on wordIndex to currentWord
        currentWord[j] = word[i];
        j++;
    }
    currentWord[j] = '\0';

    if(currentWord[0] == '\0'){                 //if the word is empty, prints & in the computation
        currentWord[0] = '&';                   
        currentWord[1] = '\0';
    }

    if(*comp == NULL){                      
        *comp = createCompStep(currentState, currentWord, stackToString(s));   //if comp has no elements, creates the first one
    } else {
        addComp(*comp, currentState, currentWord, stackToString(s));           //otherwise adds to the end of the list
    }

    //stopping criterion
    if(word[wordIndex] == '\0'){
        if((states[currentState]->flag == 1)) return 1;    //if the word is fully consumed and in a final state, accepts
        else if (states[currentState]->flag == 0 && states[currentState]->paths == NULL) return 0; //if not in a final state and has no more path options, rejects
    }

    char currentChar = word[wordIndex];             //variable that receives the current letter
    pathNode *current_node = states[currentState]->paths;        //variable that receives the list of possible paths of the state

    //loop that goes through the list of possible paths
    while(current_node != NULL){
        //1: the read letter is the same as to be consumed by the node
        if(current_node->received == currentChar){  
             //1.1: the pop of the node is not empty and matches the top of the stack
            if((current_node->pop != '&' && current_node->pop == top)){
                temp = pop(s); //temp receives the top of the stack in case it needs to be restored
        
                if(strcmp(current_node->push, "&") != 0){
                    for(int i = strlen(current_node->push) - 1; i >= 0; i--){         //if the node's push field is not empty, performs the push
                        push(s, current_node->push[i]);
                    }
                }

                //recursively calls and in case of success returns 1, otherwise restores the stack and deletes the last added computation
                if(verifyWord(states, word, wordIndex + 1, current_node->index, s, comp) == 1){
                    return 1;
                } else {
                    if(strcmp(current_node->push, "&") != 0) 
                        for(int i = 0; i < strlen(current_node->push) - 1; i++)
                            pop(s);
                    if(current_node->pop != '&') push(s, temp);
                    deleteComp(comp);
                }
            }
            //1.2: the node's pop field is empty
            if(current_node->pop == '&'){
                if(strcmp(current_node->push, "&") != 0){
                    for(int i = strlen(current_node->push) - 1; i >= 0; i--){         //if the node's push field is not empty, performs the push
                        push(s, current_node->push[i]);
                    }
                }
                if(verifyWord(states, word, wordIndex + 1, current_node->index, s, comp) == 1){
                    return 1;
                } else {
                    for(int i = 0; i < strlen(current_node->push) - 1; i++)
                            pop(s);
                    if (current_node->pop != '&') push(s, temp);
                    deleteComp(comp);
                }
            } 
        }
        //2: the read letter is empty
        //it remains practically the same, only changes that when calling recursively it passes wordIndex as a parameter and not wordIndex + 1
        if(current_node->received == '&'){
            if((current_node->pop != '&' && current_node->pop == top)){
                temp = pop(s);
                if(strcmp(current_node->push, "&") != 0){
                    for(int i = strlen(current_node->push) - 1; i >= 0; i--){
                        push(s, current_node->push[i]);
                    }
                }
                if(verifyWord(states, word, wordIndex, current_node->index, s, comp) == 1){
                    return 1;           
                } else {
                    for(int i = 0; i < strlen(current_node->push) - 1; i++)
                            pop(s);
                    if(current_node->pop != '&') push(s, temp);
                    deleteComp(comp);
                }
            }
            if(current_node->pop == '&'){
                if(strcmp(current_node->push, "&") != 0){
                    for(int i = strlen(current_node->push) - 1; i >= 0; i--){
                        push(s, current_node->push[i]);
                    }
                }
                if(verifyWord(states, word, wordIndex, current_node->index, s, comp) == 1){
                    return 1;
                } else {
                    for(int i = 0; i < strlen(current_node->push) - 1; i++)
                            pop(s);
                    if (current_node->pop != '&') push(s, temp);
                    deleteComp(comp);
                }
            }
        }
        //goes to the next node of the list
        current_node = current_node->next;
    }   
    //returns 0 if no path is found
    return 0;
}

//frees the memory of the graph
void freeGraph(graph *g){
    for(int i = 0; i < g->nVertex; i++){
        while(g->states[i]->paths != NULL){
            pathNode *temp = g->states[i]->paths;
            g->states[i]->paths = g->states[i]->paths->next;
            free(temp);
        }
        free(g->states[i]);
    }
    free(g);
}

int main(){
    int Q, T, src, dest, F, N;
    char rec, g_pop, g_push[101], word[101], quit = '*';

    for(int i = 0; i < 100; i++){
        word[i] = '\0';
    }

    //initializes the stack with a Z
    stack *s = createstack();
    push(s, 'Z');

    //gets the number of states and transitions
    printf("Number of states: ");
    scanf("%d", &Q);
    printf("Number of transitions: ");
    scanf("%d", &T); 

    graph *g = createGraph(Q);

    //creates the paths in the graph
    printf("Enter the transitions (format: source received pop push dest): \n('&' means empty)\n");
    for(int i = 0; i < T; i++){
        scanf("%d %c %c %s %d", &src, &rec, &g_pop, g_push, &dest);
        g = addPath(g, src, rec, g_pop, g_push, dest);
    }
   
    printf("Number of final states: ");
    //configures the final states
    scanf("%d", &F);
    printf("Enter which nodes will be the final states: \n");
    for(int i = 0; i < F; i++){
        scanf("%d", &N);
        g = final(g, N);
    }

    //loop for word verification
    while(1){
        printf("Enter words to verify ('*' to stop): ");
        scanf("%s", word);
        if(!strcmp(word, "*")) break;
        printf("%s: ", word);

        compStep *comp = NULL;      //initializes the computation linked list to null

        if(verifyWord(g->states, word, 0, 0, s, &comp)){
            printf("yes\n");
            printComp(comp);
        } else printf("no\n\n");

        freeComp(comp);
        comp = NULL;

        empty(s);
        push(s, 'Z');
    }

    empty(s);
    free(s);
    freeGraph(g);
    
    return 0;
}
