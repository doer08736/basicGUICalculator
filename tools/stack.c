/* Stack Tools*/

typedef struct operatorNode{
    char data;
    struct operatorNode *next;
}nodeOperator;


typedef struct operandNode{
    double data;
    struct operandNode *next;
}nodeOperand;


nodeOperator *topOperator = NULL;
nodeOperand *topOperand = NULL;


unsigned short int isOperator(char ch){
	return ch=='+' || ch=='-' || ch=='x' || ch=='/' || ch=='%';
}

unsigned short int precedence(char op){
    switch(op){
        case 'x':
        case '%':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

unsigned short int isEmpty(char stackType){
    if((stackType=='r' && topOperator==NULL) || topOperand==NULL)
        return 1;
    return 0;
}

void pushOperator(char ch){
    nodeOperator *newnode = (nodeOperator*)malloc(sizeof(nodeOperator));
    newnode->data = ch;
    newnode->next = topOperator;
    topOperator = newnode;
}

void pushOperand(double num){
    nodeOperand *newnode = (nodeOperand*)malloc(sizeof(nodeOperand));
    newnode->data = num;
    newnode->next = topOperand;
    topOperand = newnode;
}

char popOperator(){
    char ch;
    nodeOperator *temp = topOperator;
    ch = topOperator->data;
    topOperator = topOperator->next;
    free(temp);
    return ch;
}

double popOperand(){
    nodeOperand *temp = topOperand;
    double num = topOperand->data;
    topOperand = topOperand->next;
    free(temp);
    return num;
}

double infixEval(){
    char op = popOperator();
    double x = popOperand(), y = popOperand();
    
    switch(op){
        case '+':
            return x+y;
        case '-':
            return y-x;
        case 'x':
            return x*y;
        case '/':
            return y/x;
        case '%':
            return y/100*x;
        default:
            return 0;
    }
}

void evaluate(char *expr){
    char ch, num[30];
    for(register unsigned short int i=0; *(expr+i)!='\0'; ++i){
        ch = *(expr+i);
        if(
            isdigit(ch) || 
            ch == '.' ||
            (ch == '-' && i == 0) || 
            (ch == '-' && !isEmpty('r') && (topOperator->data == 'x' || topOperator->data == '/' || topOperator->data == '%'))
        ){
            memset(num, 0, sizeof(num));
            while(
                isdigit(ch) || 
                ch == '.' ||
                (ch == '-' && i == 0) ||
                (ch == '-' && !isEmpty('r') && (topOperator->data == 'x' || topOperator->data == '/' || topOperator->data == '%'))
            ){
                strncat(num, &ch, 1);
                i++;
                if(i<strlen(expr)){
                    ch = *(expr+i);
                    continue;
                }break;
            }i--;
            pushOperand(strtod(num, NULL));

        }
        else if(isOperator(ch)){
            while(
                !isEmpty('r') && 
                (precedence(ch)<=precedence(topOperator->data))
            )
                pushOperand(infixEval());
            pushOperator(ch);
        }
    }
    while(!isEmpty('r'))
        pushOperand(infixEval());
}