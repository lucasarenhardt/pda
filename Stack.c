#include "Stack.h"

// Creates a new stack and initializes it
stack *createstack(){
    stack *s = (stack*)malloc(sizeof(stack));
    s->top = NULL;
    return s;
}

// Checks if the stack is empty
int isEmpty(stack *s){
    if(s->top == NULL) return 1; 
    else return 0;
}

// Pushes a new element onto the stack
void push(stack *s, char data){
    stackNode *new = (stackNode*)malloc(sizeof(stackNode));
    new->data = data;
    new->next = s->top;
    s->top = new;
}   

// Pops an element from the stack
char pop(stack *s){
    if(isEmpty(s)){
        return '\0';
    }
    stackNode *temp = s->top;
    char data = temp->data;
    s->top = temp->next;
    free(temp);
    return data;   
}

// Empties the stack
void empty(stack *s){
    while(s->top != NULL)
        pop(s);
}

// Copies the content of the stack to a string
char *stackToString(stack *s){
    stack *stackTemp = createstack();
    int i = 0;
    char data;
    char *stackChar;

    // While the stack is not empty, transfers the content to an auxiliary stack and counts the number of elements
    while(!isEmpty(s)){
        data = pop(s);
        push(stackTemp, data);
        i++;
    }

    // Allocates memory based on the number of elements counted (+1 for the \0)
    stackChar = (char*)malloc((i+1)*sizeof(char)); 

    // Copies the content to the string and at the same time transfers back to the original stack
    for(int j=i-1; j>=0; j--){
        data = pop(stackTemp);
        stackChar[j] = data;
        push(s, data);
    }
    stackChar[i] = '\0';

    empty(stackTemp);
    free(stackTemp);

    return stackChar;
}