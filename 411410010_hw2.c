#include <stdio.h>
#include <stdlib.h>
// #define DEBUG 1
struct path{
    int node;
    int shrink;
    struct path *next;
};
//initialized------------------------------------------------------------------------------------------
struct path *path_push_back(struct path **list,int node,int shrink){//initalizes currentpath used
    struct path *new_node = malloc(sizeof(struct path));
    new_node->node = node;
    new_node->shrink = shrink;
    new_node->next = NULL;
    if(*list == NULL){
        *list = new_node;
        return *list;
    }
    else{
        struct path *now = *list;
        while(now->next != NULL){
            now = now->next;
        }
        now->next = new_node;
        return new_node;
    }
}

void path_initialize(struct path **path,int table[],int n,int final_table[],struct path *table_record_node[]){//initializes currentpath
    int path_track = 0;
    if(table[0]!=final_table[0]){
        table_record_node[0] = path_push_back(path,0,0);
    }
    else{
        table_record_node[0] = path_push_back(path,0,1);
    }
    while(path_track != n-1){
        if(table[path_track]!=final_table[path_track]){
            table_record_node[path_track] = path_push_back(path,table[path_track],0);
        }
        else{
            table_record_node[path_track] = path_push_back(path,table[path_track],1);
        }
        path_track = table[path_track];
    }
}

void table_unused_initialize(int table_unused[],int n){
    for(int i=0;i<n;i++){
        table_unused[i] = 0;
    }
}

void table_record_node_initialize(struct path *table_record_node[],int n,int table[]){
    for(int i=0;i<n;i++){
        if(table_record_node[i] == NULL){
            table_record_node[i] = malloc(sizeof(struct path));
            table_record_node[i]->next = NULL;
            table_record_node[i]->node = table[i];
            table_record_node[i]->shrink = 1;
        }
    }
}
//function---------------------------------------------------------------------------------------------
void table_unused_check(int table[],int table_unused[],int n){
    for(int i=0;i<n;i++){
        if(table[i] == -1 && i!=(n-1)){
            table_unused[i] = 1;
        }
        else if(i == n-1){
            table_unused[i] = -1;
        }
    }

}

int table_finished_check(int table[],int final_table[],int n){
    for(int i=0;i<n;i++){
        if(table[i] != final_table[i]){
            return 0;
        }
    }
    return 1;
}

void update_eventime(int *table[],int final_table[],int n,int table_unused[],struct path *table_record_node[],int **finaloutput,int stepsum){
    for(int i=0;i<n;i++){
        if(table_unused[i]==1){
            table[i] = final_table[i];
            table_record_node[i]->node = final_table[i];
            table_record_node[i]->next = table_record_node[final_table[i]];
            table_record_node[i]->shrink = 1;
        }
        finaloutput[stepsum][i] = table[i];
    }
}
void update_oddtime(){

}

int main(){
    //data--------------------------------
    int n;//點的數量
    scanf("%d",&n);
    int table[n],finaltable[n];//s = 0,d = n-1
    struct path *currentpath = NULL;//linked list 用於試跑路徑(透過shrink參數去調整)
    struct path *temp;
    int table_finished = 0;//用於迴圈停止
    struct path *table_record_node[n];//記錄所有node的位置
    int **finaloutput = malloc(n*sizeof(int *));//紀錄output的更動
    *finaloutput = malloc(n*sizeof(int));
    int stepsum = 1;//累積output更動的path次數
    //algorithm_oddtime-------------------
    int table_check[n];//用於檢察新路徑有沒有迴圈(可在function宣告就好)
    int table_node_skip[n];//用於計算新路徑跳過的node數量(須配合shrink參數)(可在function宣告就好)
    //algorithm_eventime------------------
    int table_unused[n];//紀錄沒有試用過的點(可寫在function就好，但已經透過其他function弄好=>不更動)
    //initialize--------------------------
    for(int i=0;i<n;i++){
        scanf("%d",&table[i]);
    }
    for(int i=0;i<n;i++){
        scanf("%d",&finaltable[i]);
    }
    for(int i=0;i<n;i++){
        table_record_node[i] = NULL;
    }
    for(int i=0;i<n;i++){
        finaloutput[0][i] = table[i];
    }
    path_initialize(&currentpath,table,n,finaltable,table_record_node);
    table_unused_initialize(table_unused,n);
    table_record_node_initialize(table_record_node,n,table);
    
    #ifdef DEBUG
    for(int i=0;i<n;i++){
        printf("node: %d shrink: %d\n",table_record_node[i]->node,table_record_node[i]->shrink);
    }
    #endif
    //algorithm---------------------------
    while(!(table_finished)){
        //eventime------------------------
        table_unused_check(table,table_unused,n);//要注意d 也被視為unused
        update_eventime(&table,finaltable,n,table_unused,table_record_node,finaloutput,stepsum);
        stepsum++;
        //oddtime-------------------------


        table_finished_check(table,finaltable,n);
    }

    #ifdef DEBUG
    for(int i=0;i<n;i++){
        printf("%d ", table_unused[i]);
    }
    #endif



    #ifdef DEBUG
    temp = currentpath;
    while(temp){
        printf("%d",temp->node);
        temp = temp->next;
    }
    #endif
}