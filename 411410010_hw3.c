#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #define DEBUG 1

struct edge{
    int node;
    struct edge *next;
};
struct req{
    int node_head;
    int node_tail;
    int lowest_height;
};
struct queue{
    int node;
    int level;
    struct queue *next;
};
struct tree{
    int n1;
    int n2;
    int height;
    int leaf;
    struct tree *left;
    struct tree *right;
};
struct queue_cbt{
  struct tree *node;
  int height;
  int ID;
  int new_ID;
  struct queue_cbt *next;  
};
//function----------------------------------------------------------------
int cmp_num(const void *a, const void *b,int level){
    int A = (*(int (*)[2])a)[level];
    int B = (*(int (*)[2])b)[level];
    if(A < B){
        return -1;
    }
    else if(A > B){
        return 1;
    }
    else{
        return cmp_num(a,b,level+1);
    }
}
int cmp(const void *a, const void *b){
    int A = (*(int (*)[2])a)[0];
    int B = (*(int (*)[2])b)[0];
    int level = 0;
    if (A < B){
        return -1;
    } else if (A > B){
        return 1;
    } else {
        // 如果花色相同，按照數字排序
        return cmp_num(a,b,level+1);
    }
}
void edge_push_back(struct edge **list,int node){
    struct edge *new_node = malloc(sizeof(struct edge));
    new_node->node = node;
    new_node->next = NULL;
    if(*list == NULL){
        *list = new_node;
        return;
    }
    else{
        struct edge *now = *list;
        while(now->next != NULL){
            now = now->next;
        }
        now->next = new_node;
        return;
    }
}
void queue_push_back(struct queue **list,int node,int level){
    struct queue *new_node = malloc(sizeof(struct queue));
    new_node->node = node;
    new_node->level = level;
    new_node->next = NULL;
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

struct queue *q_clear(struct queue *q){
    struct queue *temp;
    while(q){
        temp = q;
        q = q->next;
        free(temp);
    }
}
void queuecbt_push_back(struct queue_cbt **list,struct tree *node,int height,int ID,int new_ID){
    struct queue_cbt *new_node = malloc(sizeof(struct queue_cbt));
    new_node->node = node;
    new_node->height = height;
    new_node->ID = ID;
    new_node->new_ID = new_ID;
    new_node->next = NULL;
    if(*list == NULL){
        *list = new_node;
        return;
    }
    else{
        struct queue_cbt *now = *list;
        while(now->next != NULL){
            now = now->next;
        }
        now->next = new_node;
        return;
    }
}
struct queue_cbt *queuecbt_pop(struct queue_cbt *queue){
    struct queue_cbt *temp = queue;
    queue = queue->next;
    free(temp);
    return queue;
}
struct queue_cbt *qcbt_clear(struct queue_cbt *q){
    struct queue_cbt *temp;
    while(q){
        temp = q;
        q = q->next;
        free(temp);
    }
}
struct queue *BFS(struct queue *q,int *bool_check,int end,struct edge *edge[],int NodeCount){
    struct queue *tempq;
    int now,level;
    now = q->node;
    level = q->level;
    #ifdef DEBUG
    printf("%d %d\n",now,level);
    #endif
    struct edge *tempe;
    q = queue_pop(q);
    if(bool_check[now] != -1){
        q = BFS(q,bool_check,end,edge,NodeCount);
    }
    else{
        bool_check[now] = level;
        if(now == end){
            #ifdef DEBUG
            tempq = q;
            while(tempq){
                printf("%d \n",tempq->node);
                tempq = tempq->next;
            }
            #endif
            return 0;
        }
        else{
            tempe = edge[now];
            while(tempe){
                if(bool_check[tempe->node] == -1){
                    queue_push_back(&q,tempe->node,level+1);
                }
                tempe = tempe->next;
            }
            if(q){
                #ifdef DEBUG
                for(int i = 0;i<NodeCount;i++){
                    printf("%d ",bool_check[i]);
                }
                printf("\n");
                #endif
                q = BFS(q,bool_check,end,edge,NodeCount);
            }
        }
    }
    return q;
}

void trace(int *bool_check,int head,int now,int Nodecount,struct edge *edge[],int *path_record){
    int level = bool_check[now];
    struct edge *path;
    path_record[level+1] = now;
    path_record[0] = level+1;
    for(int i=level-1;i>=0;i--){
        path = edge[now];
        while(1){
            if(bool_check[path->node] == i){
                path_record[i+1] = path->node;
                now = path->node;
                break;
            }
            path = path->next;
        }

    }
}
struct tree *tree_newnode(){
    struct tree *new_node = malloc(sizeof(struct tree));
    new_node->height = -1;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->leaf = 0;
    new_node->n1 = -1;
    new_node->n2 = -1;
    return new_node;
}

struct tree *tree_build(int parent){
    // printf("-------%d--------\n",parent);
    struct tree *root = tree_newnode();
    root->height = (int)floor(log2(parent+1+parent))+1;
    struct tree *tempt;
    struct queue_cbt *q = malloc(sizeof(struct queue_cbt));
    q->node = root;
    q->next = NULL;
    for(int i = 0;i<parent;i++){
        tempt = q->node;
        q = queuecbt_pop(q);
        tempt->leaf = 0;
        tempt->left = tree_newnode();
        tempt->left->height = tempt->height-1;
        tempt->right = tree_newnode();
        tempt->right->height = tempt->height-1;
        queuecbt_push_back(&q,tempt->left,0,-1,-1);
        queuecbt_push_back(&q,tempt->right,0,-1,-1);
    }
    qcbt_clear(q);
    return root;
}

void postorder_input(struct tree *root,int *leaf_order,int check[],int path[]){
    if(root){
        postorder_input(root->left,leaf_order,check,path);
        postorder_input(root->right,leaf_order,check,path);
        if(root->left == NULL && root->right == NULL){//leaf
            check[*leaf_order] -= 1;
            check[*leaf_order+1] -= 1;
            // printf("%d:%d %d\n",*leaf_order,path[*leaf_order+1],path[*leaf_order+2]);
            root->n1 = path[*leaf_order+1];
            root->n2 = path[*leaf_order+2];
            root->leaf = 1;
            if(check[*leaf_order] == 0){
                *leaf_order += 1;
            }
        }
        else{
            root->n1 = root->left->n1;
            root->n2 = root->right->n2;
        }

    }
}
void postorder_check(struct tree *root){
    if(root){
        postorder_check(root->left);
        postorder_check(root->right);
        printf("%d:%d %d\n",root->height,root->n1,root->n2);
    }
}
void postorder_output(struct tree *root,int h){
    if(root){
        postorder_output(root->left,h);
        postorder_output(root->right,h);
        if(root->leaf == 1){
            printf("%d %d %d\n",root->n1,root->n2,h+root->height);
        }
        else{
            printf("%d %d ",root->n1,root->n2);
            printf("%d %d %d %d %d\n",root->left->n1,root->left->n2,root->right->n1,root->right->n2,h+root->height);
        }
    }
}

int tmt(struct tree *root,int **memories,int *check,int height){//tree memory test
    if(root){
        tmt(root->left,memories,check,height);
        tmt(root->right,memories,check,height);
        if(*check == -1){
            return *check;
        }
        // printf("%d\n",root->height);
        memories[root->n1][root->height+height]-=1;
        memories[root->n2][root->height+height]-=1;
        if(root->leaf == 1){
            memories[root->n1][root->height+height-1]-=1;
            memories[root->n1][root->height+height-1]-=1;
        }
        if(memories[root->n1][root->height+height+1]<0 || memories[root->n2][root->height+height+1]<0 || memories[root->n1][root->height+height-1]<0 || memories[root->n1][root->height+height-1]<0){
            *check = -1;
        }
    }
    return *check;
}
void tree_clear(struct tree *root){
    if(root){
        tree_clear(root->left);
        tree_clear(root->right);
        free(root);
    }
}
//intitialize--------------------------------------------------------------
void input_initialize(int NodeCount,int EdgeCount,int ReqCount,int *Nodememories,struct edge *edge[],struct req *requests){
    int temp,a,b;
    for(int i = 0;i<NodeCount;i++){
        scanf("%d",&temp);
        scanf("%d",&Nodememories[i]);
    }
    for(int i = 0;i<EdgeCount;i++){
        scanf("%d",&temp);
        scanf("%d %d",&a,&b);
        edge_push_back(&edge[a],b);
        edge_push_back(&edge[b],a);
    }
    for(int i = 0;i<ReqCount;i++){
        scanf("%d",&temp);
        scanf("%d %d",&requests[i].node_head,&requests[i].node_tail);
    }
}
void BFS_intialize(int *bool_check,int NodeCount,int *pair_bfs){
    for(int i=0;i<NodeCount;i++){
        bool_check[i] = -1;
        pair_bfs[i] = -1;
    }
}
void check_leaforder_initialize(int *check,int n){
    for(int i=0;i<n;i++){
        if(i == 0 || i == n-1){
            check[i] = 1;
        }
        else{
            check[i] = 2;
        }
    }
}
void memories_temp_initialize(int **memories,int **temp,int n,int t){
    for(int i=0;i<n;i++){
        for(int j=0;j<t;j++){
            temp[i][j] = memories[i][j];
        }
    }
}

int main(){
    //-------------------------------basic variables
    int NodeCount,EdgeCount,Timeslots,ReqCount; 
    int temp;
    //-------------------------------basic arrays
    scanf("%d %d %d %d",&NodeCount,&EdgeCount,&Timeslots,&ReqCount);
    int Nodememories[NodeCount];
    struct edge *edge[EdgeCount],*tempe;
    struct req reqests[ReqCount];
    //-------------------------------BFS
    struct queue *BFS_q = NULL,*tempq = NULL;
    int bool_check[NodeCount];//bool_check
    int pair_bfs[ReqCount][NodeCount+2];//注意第一個地方存的是length
    //-------------------------------memory + timeslot
    int **memories;
    int **tempm;
    //-------------------------------tree build
    int req_now = 0;
    struct tree *tree_root = NULL;
    //-------------------------------tree-memory test
    int count = 0;
    struct queue_cbt *output = NULL;
    //-------------------------------initialization
    for(int i=0;i<NodeCount;i++){
        edge[i] = NULL;
    }
    input_initialize(NodeCount,EdgeCount,ReqCount,Nodememories,edge,reqests);
    memories = malloc(NodeCount*sizeof(int *));
    tempm = malloc(NodeCount*sizeof(int *));
    for(int i = 0;i<NodeCount;i++){
        memories[i] = malloc(Timeslots*sizeof(int));
        tempm[i] = malloc(Timeslots*sizeof(int));
        for(int j = 0;j<Timeslots;j++){
            memories[i][j] = Nodememories[i];
        }
    }
    //------------------------------BFS
    for(int i=0;i<ReqCount;i++){
        BFS_q = malloc(sizeof(struct queue));
        BFS_q->node = reqests[i].node_head;
        BFS_q->next = NULL;
        BFS_q->level = 0;
        BFS_intialize(bool_check,NodeCount,pair_bfs[i]);
        BFS_q = BFS(BFS_q,bool_check,reqests[i].node_tail,edge,NodeCount);
        //trace
        trace(bool_check,reqests[i].node_head,reqests[i].node_tail,NodeCount,edge,pair_bfs[i]);
        pair_bfs[i][NodeCount+1] = i;
        #ifdef DEBUG
        printf("%d: ",i);
        for(int j=0;j<NodeCount;j++){
            printf("%d ",pair_bfs[i][j]);
        }
        printf("\n");   
        #endif

        BFS_q = q_clear(BFS_q);
    }

    qsort(pair_bfs,ReqCount,sizeof(pair_bfs[0]),cmp);

    #ifdef DEBUG
    for(int i=0;i<ReqCount;i++){
        printf("%d: ",i);
        for(int j = 0;j<NodeCount;j++){
            printf("%d ",pair_bfs[i][j]);
        }
        printf("\n"); 
    }
    #endif

    //-------------------------------------------建tree
    for(int i=2;i<=pair_bfs[ReqCount-1][0];i++){//按照不同length去計算(i = length)
        if(pair_bfs[req_now][0]>i){
            continue;
        }
        for(int j=req_now;j<ReqCount;j++){
            if(pair_bfs[j][0]>i){
                req_now = j;
                break;
            }
            int check_leaforder[pair_bfs[j][0]];
            check_leaforder_initialize(check_leaforder,pair_bfs[j][0]);
            tree_root = tree_build(i-2);
            temp = 0;
            postorder_input(tree_root,&temp,check_leaforder,pair_bfs[j]);
            // postorder_check(tree_root);
            // printf("%d\n",tree_root->height);
            for(int h = 0; h < Timeslots; h++){
                if((h+tree_root->height+1)>Timeslots){
                    break;
                }
                memories_temp_initialize(memories,tempm,NodeCount,Timeslots);
                temp = 0;
                if(tmt(tree_root,tempm,&temp,h) != -1){
                    // printf("yes\n");
                    count++;
                    queuecbt_push_back(&output,tree_root,h+1,pair_bfs[j][NodeCount+1],j);
                    memories_temp_initialize(tempm,memories,NodeCount,Timeslots);
                    break;
                }
                else{
                    // printf("None\n");
                    if(h == Timeslots-1){
                        tree_clear(tree_root);
                    }
                }
            }
            
        }
    }
    printf("%d\n",count);
    while(output){
        printf("%d ",output->ID);
        for(int i=1;i<=pair_bfs[output->new_ID][0];i++){
            printf("%d ",pair_bfs[output->new_ID][i]);
        }
        printf("\n");
        postorder_output(output->node,output->height);
        output = output->next;
    }
}