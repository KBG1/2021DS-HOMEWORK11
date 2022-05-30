#include<stdio.h>
#include<stdlib.h>

#define MAX_VERTEX 11 //0이 표기가 안되서 1증가 시켜 1~10으로 만들었습니다 ㅠ
#define MAX_QUEUESIZE 20 //큐사이즈 20으로 설정

typedef struct graphNode{
    int vertex; //정점 변수
    struct graphNode *link; //링크값을 담는 변수
}graphNode;

typedef struct graphType{ 
    int n; //정점 수를 표현하기 위한 정수형 변수 n
    graphNode *adj_list[MAX_VERTEX]; //인접 리스트를 최대 정점 갯수만큼 할당 받을 수 있도록 설정
}graphType; 

typedef struct queueType{
    int queue[MAX_QUEUESIZE]; //20사이즈 만큼의 큐 선언
    int front, rear; //빈큐인지 아닌지 구별하기위한 front와 rear 선언
}queueType;

void createQueue(queueType *q);
int visit[MAX_VERTEX];
int initialize(graphType *g);
void insertVertex(graphType *g, int v);
void insertEdge(graphType *g, int u, int v);
void printGraph(graphType *g);
void dfsGraph(graphType *g, int v);
void bfsGraph(graphType *g, int v);
int isEmpty(queueType *q);
int isFull(queueType *q);
void enQueue(queueType *q, int item);
int deQueue(queueType *q);
int freeQueue(queueType *q);
int freeGraph(graphType *g);


int main()
{
	char command; //문자형 변수 command 선언
	int u, v; //정수형 변수 key선언
	graphType g; //graphType 구조체 g 선언

	do{\
		printf("----------------------------------------------------------------\n");
        printf("           2019062022                     Kim Beom Gyu\n");
		printf("                         Graph-Search                           \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize    = z                           \n");
		printf(" Insert Vertex = v            Insert Edge = e \n");
		printf(" Depth First Search = d      Breath First Search = b\n");
		printf(" Print Graph = p             Quit = q\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //scanf를 통해 입력받은 문자를 command에 저장

		switch(command) { //각 커맨드에 맞는 기능 실행
		case 'z': case 'Z':
			initialize(&g); //그래프 초기화 함수
			break;
		case 'p': case 'P':
			printGraph(&g); //그래프 출력함수
			break;
		case 'v': case 'V':
			insertVertex(&g, v); //정점 추가함수
			break;
		case 'd': case 'D':
			dfsGraph(&g, v); //깊이 우선 탐색 그래프 함수 
			break;
		case 'e': case 'E':
			printf("insert two Vertex = ");
			scanf("%d %d", &u, &v); //이을 엣지를 선택함(두개, 겹치지 않도록)
			if(u == v){
				printf("please command different number\n");
				break;
			}
			else{
				insertEdge(&g, u, v); //간선 추가 함수
				break;
			}
		case 'b': case 'B':
			bfsGraph(&g, v); //너비우선탐색 함수
			break;
		case 'q': case 'Q':
			freeGraph(&g); //그래프 메모리 초기화하는 함수
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(graphType *g){
    int v; 
    g->n = 0; //그래프의 정점갯수 초기화
    for(v = 0; v < MAX_VERTEX ; v++){ //v가 0부터 1씩 증가하면서 인접리스트 값을 모두 초기화 시켜줌
        g->adj_list[v] = NULL;
    }
	return 1;
}

void insertVertex(graphType *g, int v){
    if(((g->n) + 1) > MAX_VERTEX){ //정점갯수에 1을 더했을때 최대 정점갯수보다 많다면
        printf(" overflow! \n"); //오버플로우
        return;
    }
   	g->n++; //아니라면 정점 갯수 1개추가
}

void insertEdge(graphType *g, int u, int v){
    graphNode *node; //그래프노드 구조체 포인터변수 node선언
    if(u >= g->n || v >= g->n){ //입력받은 두 정점중 하나라도 입력받았을때 정점갯수를 초과하면
        printf("vertex index error\n"); //오류 발생
        return;
    }
    node = (graphNode *)malloc(sizeof(graphNode)); //node에 그래프노드만큼의 동적 메모리 할당
    node -> vertex = v; //v값을 node의 정점값에 넣어주고
    node -> link = g -> adj_list[u]; // 인접 리스트를 link에 할당함
    g -> adj_list[u] = node; //node값은 인접리스트에 할당
}

void printGraph(graphType *g){
	for(int k = 0; k < g->n ; k++){ 
    	graphNode *key = g->adj_list[k]; //g의 인접리스트 값들을 정점 갯수만큼 graphNode구조체 포인터변수 key에 할당
        while(key != NULL){ //key에 값이 없어질때까지
			printf("%2d -> ", key -> vertex); //key에있는 vertex값 출력
        	key = key -> link; //출력한 다음 key값에 key의 link값 할당
		}
    }
	printf("\n");
}

void dfsGraph(graphType *g, int v){
    graphNode *w; //graphNode구조체 포인터 변수w선언
    visit[v] = 1; //1이 방문했다는 뜻, 처음은 방문하였으므로 1로 초기화
    printf("%d ", v); //방문한 vertex 출력
    for (w = g->adj_list[v]; w ; w = w -> link){ //g의 인접 리스트부터 시작해서 자신이 될때까지
        if(!visit[w -> vertex]){ //방문하지 않았다면
            dfsGraph(g, w -> vertex); //dfsgraph함수 재실행
        }
    }
}

void bfsGraph(graphType *g, int v){
    graphNode *w; //graphNode구조체 포인터 변수w선언
    queueType q; //queueType 구조체 q 변수 선언
    createQueue(&q); //큐 초기화 함수
    visit[v] = 1; //위와같이 첫 정점은 방문한것으로...
    printf("%d ", v); // 방문한 정점 출력하고
    enQueue(&q, v); //그 값을 큐에 넣어줌

    while(!isEmpty(&q)){ //큐가 비지 않을때 까지
        v = deQueue(&q); //v를 큐에서 제외하고
        for(w = g->adj_list[v]; w ; w = w -> link){ //인접리스트부터 시작해서 자기자신 까지
            if(!visit[w -> vertex]){ //방문하지 않은 정점이라면
                visit[w -> vertex] = 1; //기록하고
                printf("%d ->", w -> vertex); //그 값을 출력해준다
                enQueue(&q, w -> vertex); //그 다음 큐에 값을 넣어준다.
            }
        }   
    }
	freeQueue(&q); //큐에 할당된 메모리를 해제한다.
}

void createQueue(queueType *q)
{
	q->front = 0; 
	q->rear = 0; //front와 rear 0으로 초기화
}

int isEmpty(queueType *q)
{
	return (q->front == q->rear); //front와 rear가 같은지 검사해봄
		
}

int isFull(queueType *q)
{
	if (((q->rear+1) % MAX_QUEUESIZE) == q->front) { //rear값에서 +1한 값에서 20으로 나눈 나머지가 front값과 같으면 가득 찬 것으로 간주
		printf(" Queue is full!"); //오류
		return 1;
	}
	else return 0;
}

void enQueue(queueType *q, int item)
{
	if(isFull(q)) return; //꽉 차있으면 실행 안함
	else {
		q->rear = (q->rear + 1) % MAX_QUEUESIZE;  //q의 rear값에 rear + 1 % 20값 대입
		q->queue[q->rear] = item; //  q의 rear의 위치에 item값 할당
	}
}

int deQueue(queueType *q)
{
	if(isEmpty(q)) return; //비어있으면 실행 X
	else {
		q->front = (q->front + 1)% MAX_QUEUESIZE; //enQueue와 내용 동일
		return q -> queue[q->front];
	}
}

int freeQueue(queueType *q)
{
    if(q == NULL) return 1; //q가 빈 queue인 경우, 1값 리턴
    free(q); //q 메모리 반환
    return 1;
}

int freeGraph(graphType *g){
    if(g == NULL) return 1; //그래프에 값 없으면 리턴1
    free(g); //그래프에 할당한 메모리 초기화
    return 1;
}