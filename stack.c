#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*=========20194223 권정연==========*/


//Stack과 Node 구조체 만들기//
struct Node{
  int element; 
  struct Node* next;
};


typedef struct {
  struct Node* top; 
} Stack;


//함수 선언하기//
int push(Stack* stack,int new_element);
int top(Stack* stack); 
int pop(Stack* stack);
int isPalindrome(const char* word);
int isBalanced(const char* str);




//================================================================// 
//Stack관련 함수 - createStack//
Stack* CreateStack(){ 
  Stack* stack = (Stack*) malloc(sizeof(Stack));
  stack->top = NULL; 
  return stack;
}

//Stack 관련 함수들 - push,pop,top // 
int push(Stack* stack,int new_element){
  if(!stack){
    printf("Overflow"); 
    return -1; }
  //1. Make a new node 
  struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
  new_node->element = new_element; //새로운 element를 new_node의 element에 넣는다. 
  new_node->next = stack->top; //top의 주소를 new_node의 next에 넣는다. 
  //2.attach the new node to the stack
  stack->top = new_node; //new_node의 주소를 top에 넣는다. 
    return 0; // 성공
};



int pop(Stack* stack){
  if(!(stack->top)){  //만약 stack->top이 NULL이라면 Underflow를 출력한다. 
    printf("Underflow");
    return -1;  
  } 
  //1.stack의 top element 뽑기 
  int popped_value = stack->top->element; 
  //2. 다음 노드 가리키게 하기 
  struct Node* temp = stack->top; //temp노드를 새롭게 만들고 top의 주소를 temp에 넣는다. 
  stack->top = temp->next; //temp가 가리키는 next 주소를 top에 넣는다. 
  //3. 메모리 해제 
  free(temp); //메모리를 해제시킨다. 
  return popped_value;
}


int top(Stack* stack){
  if(!(stack->top)){  //만약 stack->top이 NULL이라면 Underflow를 출력한다. 
    printf("Underflow");
    return -1; 
  } 
  return stack->top->element; // stack의 top의 element를 return한다. 
};


//================================================================
//True False 관련 함수들 - isPalindrome + toLower(소문자로 변환), isBalanced // 


//isPalindrome을 위해 소문자로 변환하는 함수 
char toLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return c;
}

int isPalindrome(const char* word) { 
    //1. 준비과정
    Stack* stack = CreateStack();  //stack만들기
    int length = (int)strlen(word); //word의 길이(기존길이)
    char* new_word = malloc((length + 1) * sizeof(char)); //새로운 배열을 new_word에 할당한다. 
    int new_length = 0; //new_word의 길이(새로운 길이)
    int i; //new_word의 index로 사용됨 
    int check = 0; //만약 회문이 아닌경우 return 하게 만드는 변수 
    

    //2. 새로운 문자열 new_word 생성 
    for (int j = 0; j < length; j++) {
    if (word[j] != ' ' && word[j] != '\n' && word[j] != '!' &&
        word[j] != '\'' && word[j] != '*' && word[j] != ',' && 
        word[j] != '.'){
        //특수기호가 있다면 new_word에 있던 배열을 복사 후 다시 new_word에 할당한다.
        new_word = realloc(new_word, (new_length + 1) * sizeof(char)); 
        //기존 배열의 j번째 글자를 새로운 newm_word에 넣는다. 
        new_word[new_length++] = toLower(word[j]);
    }
}
    
    // 3. 문자열 new_word의 반절을 스택에 넣기
    for (i = 0; i < new_length/2; i++) { //새로운 배열의 길이의 반절까지
        push(stack, new_word[i]);   //new_word의 i번째 원소를 stack에 넣는다. 
    }

    //4. 홀수라면 new_word의 index인 i를 한번 증가시킨다. 
    if(new_length%2!=0){
      i++;
    }

    //5. 남은 문자열 new_word의 반절을 스택에서 뽑아낸 것과 비교하기 
    while(i < new_length) {
        char temp1 = (char)pop(stack); //stack에서 pop한 것 
        char temp2 = new_word[i]; //new_word의 i번째 문자 
        if (temp1 != temp2) {
            // 회문이 아닌 경우 chek를 1로 변환 
            check = 1;
          }
         i++;
      }
    
    //stack과 new_word를 free시킨다. 
    free(stack);
    free(new_word);

    if(check == 1) 
        return 0; 
    else 
        return 1;
}

// 주어진 문자열이 균형잡혔는지 확인하는 함수
int isBalanced(const char* str) {
    //1. stack만들기 
    Stack* stack = CreateStack();

    //2. 여는 괄호면 push , 닫는 괄호일 때 조건에 따라 0을 반환 
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '(' || str[i] == '{' || str[i] == '[') {
            push(stack, str[i]); //stack에 여는 괄호 집어넣기 
        } else if (str[i] == ')' || str[i] == '}' || str[i] == ']') {
            if (stack->top == NULL) {
                return 0; // 스택이 비어있는데 닫는 괄호가 나온 경우
            }
            char top_char = pop(stack);
            if ((str[i] == ')' && top_char != '(') ||
                (str[i] == '}' && top_char != '{') ||
                (str[i] == ']' && top_char != '[')) {
                return 0; // 닫는 괄호와 대응하는 여는 괄호가 아닌 경우
            }
        }
    }

    //3. 만약 그럼에도 top에 남아있다면 0을 반환 
    if (stack->top != NULL) {
        free(stack);
        return 0; // 스택에 여는 괄호가 남아있는 경우
    }

    free(stack);
    return 1; // 모든 괄호가 균형을 이룬 경우
}


//============================================================================
// Stack의 모든 요소를 출력하는 함수(새로 만든 함수) 
void printStack(Stack* stack, FILE* output_file) {
    Stack* temp_stack = CreateStack();

    // 원래 스택의 모든 요소를 pop한 후 temp_stack에 저장 
    while (stack->top != NULL) {
        int element = pop(stack); 
        push(temp_stack, element);
    }

    // temp_stack에서 요소를 pop한 후 print한 뒤 다시 원래 스택에 저장하여 복원
    while (temp_stack->top != NULL) {
        int element = pop(temp_stack);
        fprintf(output_file, "%d ", element);
        push(stack, element);
    }
    fprintf(output_file, "\n");

    // 임시 스택 해제
    free(temp_stack);
}



//메인함수 
int main(int argc, char** argv){
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    //1. stack 생성하기
    Stack* stack = CreateStack(); // Stack 생성

    //2. 파일 불러들이고 내보내기 
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE* output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    //3. 버퍼 설정 
    char line[100]; 

    //4. 케이스에 따라 output file에 쓰는 것을 달리 한다. 
    while (fgets(line, sizeof(line), input_file) != NULL) {
        char command;  //명령
        int value; //값 
        sscanf(line, "%c\t%d", &command, &value); //각 줄에서 명령과 값을 스캔한다. 
        switch (command) {
                case 'H':
                    // 스택에 값(value)을 push하고, 그 결과 print 
                    if (push(stack, value) == 0) {
                        printStack(stack, output_file);
                    } else {
                        fprintf(output_file, "Overflow\n");
                    }
                    break;
                case 'O':
                    // 스택에서 pop하고, 그 결과 print 
                    if (pop(stack) != -1) {
                        printStack(stack, output_file);
                    } else {
                        fprintf(output_file, "Underflow\n");
                    }
                    break;
                case 'T':
                    // top의 값만 불러오고 그 값만 print한다. 
                    value = top(stack);
                    if (value != -1) {
                        fprintf(output_file, "%d\n", value); 
                    } else {
                        fprintf(output_file, "Underflow\n");
                    }
                    break;
                case 'P':
                    // 입력된 문자열이 회문인지 확인하고, 결과를 출력 파일에 기록한다. 
                    if (isPalindrome(line+2)) {
                        fprintf(output_file, "T\n");
                    } else {
                        fprintf(output_file, "F\n");
                    }
                    break;
                case 'B':
                    //입력된 문자열이 균형잡힌 괄호인지 확인하고, 결과를 출력 파일에 기록한다.
                    if (isBalanced(line+2)) {
                        fprintf(output_file, "T\n");
                    } else {
                        fprintf(output_file, "F\n");
                    }
                    break;

                default:
                    //// 알 수 없는 명령일 경우 오류 메시지를 출력 파일에 기록한다.
                    fprintf(output_file, "Unknown command: %c\n", command);
            }
        }

    // 파일을 닫고, 프로그램 종료
    fclose(input_file); // 입력 파일 닫기
    fclose(output_file); // 출력 파일 닫기
    return 0; // 프로그램 종료
  }


