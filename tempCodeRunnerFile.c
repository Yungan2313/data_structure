#include <stdlib.h>
#include <stdio.h>
#define DEBUG 1
struct queue{
    int x;
    int y;
    int level;
    int prev_move;
    struct queue *next;
};

struct step{
    int move;
    struct step *next;
};

typedef struct{
    int x;
    int y;
}xy;

void queue_push(struct queue **list,int y,int x,int level,int prev_move){
    struct queue *new_node = malloc(sizeof(struct queue));
    new_node->next = NULL;
    new_node->x = x;
    new_node->y = y;
    new_node->level = level;
    new_node->prev_move = prev_move;
    if(*list == NULL){
        *list = new_node;
        return;
    }
    else{
        struct queue *now = *list;
        while(now->next != NULL){
            now = now->next;
        }
        now->next = new_node;
        return;
    }
}

struct queue *queue_pop(struct queue *queue){
    struct queue *temp = queue;
    queue = queue->next;
    free(temp);
    return queue;
}

void step_push_second(struct step **list,int move){
    struct step *new_node = malloc(sizeof(struct step));
    new_node->move = move;
    if(*list == NULL){
        *list = new_node;
        return;
    }
    else{
        struct step *now = *list;
        while(now->next != NULL){
            now = now->next;
        }
        now->next = new_node;
        return;
    }
}

void step_push_first(struct step **list,int move){
    struct step *new_node = malloc(sizeof(struct step));
    new_node->next = *list;
    new_node->move = move;
    *list = new_node;
}

//queue指向第一個位置
int BFS(struct queue *queue, xy *hole_1,xy *hole_2,int **graph,int **trace){
    int x = queue->x;
    int y = queue->y;
    int level = queue->level;
    int prev_move = queue->prev_move;
    #ifdef DEBUG
    printf("%d %d %d %d\n", x, y, level,prev_move);
    #endif
    queue = queue_pop(queue);
    if(graph[y][x] != 0){
        BFS(queue,hole_1,hole_2,graph,trace);
    }
    else{
        graph[y][x] = level;
        //trace map
        trace[y][x] = prev_move;
        //---------------------------------
        #ifdef DEBUG
        for(int i=0; i<11; i++){
            for(int j=0; j<11; j++){
                printf("%d ",graph[i][j]);
            }
            printf("\n");
        }
        #endif
        if(x == hole_1->x && y == hole_1->y){
            hole_1->x = 0;
            hole_1->y = 0;
            // printf("%d",level);
            // printf("%d %d\n", hole_2->x, hole_2->y);
            return level-1;
        }
        else if(x == hole_2->x && y == hole_2->y){
            hole_2->x = 0;
            hole_2->y = 0;
            // printf("%d",level);
            // printf("%d %d\n", hole_1->x, hole_1->y);
            return level-1;
        }
        //move check
        if(graph[y-1][x] == 0){//up 0
            queue_push(&queue,y-1,x,level+1,1);
        }
        if(graph[y][x+1] == 0){//right 1
            queue_push(&queue,y,x+1,level+1,2);
        }
        if(graph[y+1][x] == 0){//down 2
            queue_push(&queue,y+1,x,level+1,3);
        }
        if(graph[y][x-1] == 0){//left 3
            queue_push(&queue,y,x-1,level+1,4);
        }
        if(queue!=NULL){
            #ifdef DEBUG
            for(int i=0; i<11; i++){
                for(int j=0; j<11; j++){
                    printf("%d ",graph[i][j]);
                }
                printf("\n");
            }
            printf("--------------------------------");
            #endif
            BFS(queue,hole_1,hole_2,graph,trace);
        }
    }
}

struct step *step_move(struct step *step,int **trace,xy hole_1){
    int trace_move = trace[hole_1.y][hole_1.x];
    int now_x = hole_1.x;
    int now_y = hole_1.y;
    while(trace_move){
        step_push_first(&step,trace_move-1);
        switch(trace_move-1){
            case 0:
                trace_move = trace[now_y+1][now_x];
                now_x = now_x;
                now_y = now_y+1;
                break;
            case 1:
                trace_move = trace[now_y][now_x-1];
                now_x = now_x-1;
                now_y = now_y;
                break;
            case 2:
                trace_move = trace[now_y-1][now_x];
                now_x = now_x;
                now_y = now_y-1;
                break;
            case 3:
                trace_move = trace[now_y][now_x+1];
                now_x = now_x+1;
                now_y = now_y;
                break;
        }
    }
    return step;
}

void initialize(int n,xy *ball_1,xy *ball_2,xy *hole_1,xy *hole_2,xy *tracexy1,xy *tracexy2){
    scanf("%d",&ball_1->x);
    scanf("%d",&ball_1->y);
    ball_1->y = n-ball_1->y-1;
    scanf("%d",&ball_2->x);
    scanf("%d",&ball_2->y);
    ball_2->y = n-ball_2->y-1;
    scanf("%d",&hole_1->x);
    scanf("%d",&hole_1->y);
    hole_1->y = n-hole_1->y-1;
    scanf("%d",&hole_2->x);
    scanf("%d",&hole_2->y);
    hole_2->y = n-hole_2->y-1;
    tracexy1->x = hole_1->x;
    tracexy1->y = hole_1->y;
    tracexy2->x = hole_2->x;
    tracexy2->y = hole_2->y;
    // printf("%d %d %d %d\n",ball_1->x,ball_1->y,ball_2->x,ball_2->y);
    // printf("%d %d %d %d\n",hole_1->x,hole_1->y,hole_2->x,hole_2->y);
    // printf("%d %d %d %d\n",tracexy1->x,tracexy1->y,tracexy2->x,tracexy2->y);
}

int main(){
    int n;//matrix range
    int **graph;//graph
    int **graph_2;//graph2
    int **trace;//trace map up1 right2 down3 left4
    int total_steps;
    xy ball_1, ball_2;//ball x y
    xy hole_1, hole_2;//hole x y
    xy trace_hole_1, trace_hole_2;
    xy hole_s_1, hole_s_2;
    xy lastracehole;
    struct queue *queue = malloc(sizeof(queue));//queue
    // struct queue *queue_2 = malloc(sizeof(struct queue));//queue
    struct step *step = NULL;//step
    struct step *step_2 = NULL;//step2
    struct step *step_3 = NULL;//step3
    struct step *step_4 = NULL;//

    struct step *temp = NULL;

    scanf("%d",&n);
    // 初始化
    graph = malloc(n*sizeof(int *));
    for(int i=0; i<n; i++){
        graph[i] = malloc(n*sizeof(int));
        for(int j=0; j<n; j++){
            scanf("%d",&graph[i][j]);
        }
    }
    graph_2 = malloc(n*sizeof(int *));
    for(int i=0; i<n; i++){
        graph_2[i] = malloc(n*sizeof(int));
        for(int j=0; j<n; j++){
            graph_2[i][j] = graph[i][j];
        }
    }
    trace = malloc(n*sizeof(int *));
    for(int i=0; i<n; i++){
        trace[i] = malloc(n*sizeof(int));
        for(int j=0; j<n; j++){
            trace[i][j] = 0;
        }
    }
    initialize(n,&ball_1, &ball_2,&hole_1,&hole_2,&trace_hole_1,&trace_hole_2);
    hole_s_1.x = hole_1.x;
    hole_s_1.y = hole_1.y;
    hole_s_2.x = hole_2.x;
    hole_s_2.y = hole_2.y;
    if(queue == NULL){
        queue = malloc(n*sizeof(int));
    }
    queue->x = ball_2.x;
    queue->y = ball_2.y;
    queue->next = NULL;
    queue->level = 1;
    queue->prev_move = 0;
    total_steps = BFS(queue,&hole_1,&hole_2,graph_2,trace);
    #ifdef DEBUG
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%d ",trace[i][j]);
        }
        printf("\n");
    }
    #endif
    if(hole_1.x == 0 && hole_1.y == 0){
        step = step_move(step,trace,trace_hole_1);
        lastracehole.x = trace_hole_2.x;
        lastracehole.y = trace_hole_2.y;
    }
    else{
        step = step_move(step,trace,trace_hole_2);
        lastracehole.x = trace_hole_1.x;
        lastracehole.y = trace_hole_1.y;
    }
    temp = step;
    while(step){
        printf("%d",step->move);
        step = step->next;
    }

    // temp = step;
    #ifdef DEBUG
    printf("------------------------------------------------------");
    #endif
    while(temp){
        switch(temp->move){
            case 0:
                if(graph_2[ball_1.y-1][ball_1.x]==0){
                    ball_1.y--;
                }
                break;
            case 1:
                if(graph_2[ball_1.y][ball_1.x+1]==0){
                    ball_1.x++;
                }
                break;
            case 2:
                if(graph_2[ball_1.y+1][ball_1.x]==0){
                    ball_1.y++;
                }
                break;
            case 3:
                if(graph_2[ball_1.y][ball_1.x-1]==0){
                    ball_1.x--;
                }
                break;
        }
        if(ball_1.x == hole_1.x && ball_1.y == hole_1.y){
            break;
        }
        temp = temp->next;
    }
    //-----------------------------------------------------------------BFS2
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            trace[i][j] = 0;
        }
    }
    for(int i=0; i<n; i++){
        graph_2[i] = malloc(n*sizeof(int));
        for(int j=0; j<n; j++){
            graph_2[i][j] = graph[i][j];
        }
    }
    if(queue == NULL){
        queue = malloc(sizeof(queue));
    }
    queue->x = ball_1.x;
    queue->y = ball_1.y;
    queue->next = NULL;
    queue->level = 1;
    queue->prev_move = 0;
    #ifdef DEBUG
    printf("\n");
    for(int i=0; i<11; i++){
        for(int j=0; j<11; j++){
            printf("%d ",graph_2[i][j]);
        }
        printf("\n");
    }       
    #endif
    total_steps+=BFS(queue,&hole_1,&hole_2,graph_2,trace);
    #ifdef DEBUG
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%d ",trace[i][j]);
        }
        printf("\n");
    }
    #endif
    step_2 = step_move(step_2,trace,lastracehole);
    while(step_2){
        printf("%d",step_2->move);
        step_2 = step_2->next;
    }
    // printf("\n%d\n",total_steps);
    printf("\n");
//-------------------------------------------------------------------------------------------------------------------
    hole_1.x = hole_s_1.x;
    hole_1.y = hole_s_1.y;
    hole_2.x = hole_s_2.x;
    hole_2.y = hole_s_2.y;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            trace[i][j] = 0;
        }
    }
    for(int i=0; i<n; i++){
        graph_2[i] = malloc(n*sizeof(int));
        for(int j=0; j<n; j++){
            graph_2[i][j] = graph[i][j];
        }
    }
    if(queue == NULL){
        queue = malloc(n*sizeof(int));
    }
    queue->x = ball_1.x;
    queue->y = ball_1.y;
    queue->next = NULL;
    queue->level = 1;
    queue->prev_move = 0;
    total_steps = BFS(queue,&hole_1,&hole_2,graph_2,trace);
    #ifdef DEBUG
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%d ",trace[i][j]);
        }
        printf("\n");
    }
    #endif
    if(hole_1.x == 0 && hole_1.y == 0){
        step_4 = step_move(step_4,trace,trace_hole_1);
        lastracehole.x = trace_hole_2.x;
        lastracehole.y = trace_hole_2.y;
    }
    else{
        step_4 = step_move(step_4,trace,trace_hole_2);
        lastracehole.x = trace_hole_1.x;
        lastracehole.y = trace_hole_1.y;
    }
    temp = step_4;
    while(temp){
        printf("%d",temp->move);
        temp = temp->next;
    }
    temp = step_4;
    // temp = step;
    #ifdef DEBUG
    printf("------------------------------------------------------");
    #endif
    while(temp){
        switch(temp->move){
            case 0:
                if(graph_2[ball_1.y-1][ball_1.x]==0){
                    ball_1.y--;
                }
                break;
            case 1:
                if(graph_2[ball_1.y][ball_1.x+1]==0){
                    ball_1.x++;
                }
                break;
            case 2:
                if(graph_2[ball_1.y+1][ball_1.x]==0){
                    ball_1.y++;
                }
                break;
            case 3:
                if(graph_2[ball_1.y][ball_1.x-1]==0){
                    ball_1.x--;
                }
                break;
        }
        if(ball_1.x == hole_1.x && ball_1.y == hole_1.y){
            break;
        }
        temp = temp->next;
    }
    //-----------------------------------------------------------------BFS2
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            trace[i][j] = 0;
        }
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            graph_2[i][j] = graph[i][j];
        }
    }
    if(queue == NULL){
        queue = malloc(sizeof(queue));
    }
    else{
        while(queue){
            queue = queue_pop(queue);
        }
        queue = malloc(sizeof(queue));
    }
    queue->x = ball_2.x;
    queue->y = ball_2.y;
    queue->next = NULL;
    queue->level = 1;
    queue->prev_move = 0;
    #ifdef DEBUG
    printf("\n");
    for(int i=0; i<11; i++){
        for(int j=0; j<11; j++){
            printf("%d ",graph_2[i][j]);
        }
        printf("\n");
    }       
    #endif
    total_steps+=BFS(queue,&hole_1,&hole_2,graph_2,trace);
    #ifdef DEBUG
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%d ",trace[i][j]);
        }
        printf("\n");
    }
    #endif
    // if(step_2 == NULL){
    //     printf("\nok\n");
    // }
    step_3 = step_move(step_3,trace,lastracehole);
    temp = step_3;
    printf("\nok\n");
    while(temp){
        printf("%d",temp->move);
        temp = temp->next;
    }
}