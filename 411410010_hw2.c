#include <stdio.h>
#include <stdlib.h>
// #define DEBUG 1
struct path{
    int node;
    int shrink;
    struct path *next;
};
//initialized------------------------------------------------------------------------------------------
void path_push_back(struct path **list,int node,int shrink){//initalizes currentpath used
    struct path *new_node = malloc(sizeof(struct path));
    new_node->node = node;
    new_node->shrink = shrink;
    new_node->next = NULL;
    if(*list == NULL){
        *list = new_node;
        return;
    }
    else{
        struct path *now = *list;
        while(now->next != NULL){
            now = now->next;
        }
        now->next = new_node;
        return;
    }
}

void path_initialize(struct path **path,int table[],int n,int final_table[],struct path *table_record_node[]){//initializes currentpath
    int path_track = 0;
    if(table[0]!=final_table[0]){
        path_push_back(path,0,0);
    }
    else{
        path_push_back(path,0,1);
    }
    while(path_track != n-1){
        if(table[path_track]!=final_table[path_track]){
            path_push_back(path,table[path_track],0);
        }
        else{
            path_push_back(path,table[path_track],1);
        }
        path_track = table[path_track];
    }
}

void table_unused_initialize(int table_unused[],int n){
    for(int i=0;i<n;i++){
        table_unused[i] = 0;
    }
}

void table_record_node_initialize(struct path *table_record_node[],int n,int table[],struct path *currentpath){
    while(currentpath){
        table_record_node[currentpath->node] = currentpath;
        currentpath = currentpath->next;
    }
    for(int i=0;i<n;i++){
        if(table_record_node[i] == NULL){
            table_record_node[i] = malloc(sizeof(struct path));
            table_record_node[i]->next = NULL;
            table_record_node[i]->node = i;
            table_record_node[i]->shrink = 1;
        }
    }
}
void finalpath_initialize(int finaltable[],int finalpath[],int n){
    int i = 0,j = 0;
    while(i != n-1){
        finalpath[j] = i;
        i = finaltable[i];
        j++;
    }
    finalpath[j] = n-1;
}
//function---------------------------------------------------------------------------------------------
void table_unused_check_initialize(int table[],int table_unused[],int n){
    for(int i=0;i<n;i++){
        if(table[i] == -1 && i!=(n-1)){
            table_unused[i] = 1;
        }
        else if(i == n-1){
            table_unused[i] = -1;
        }
    }
}

void table_unused_check(int table[],int table_unused[],int n,struct path *currentpath,int finaltable[],struct path *table_record_node[]){
    int table_check[n];
    for(int i = 0; i<n; i++){
        table_check[i] = 0;
    }
    while(currentpath){
        table_check[currentpath->node] = 1;
        currentpath= currentpath->next;
    }
    for(int i = 0;i<n;i++){
        if(table_check[i] == 1){
            table_unused[i] = 0;
        }
        else{
            if(finaltable[i] == -1){
                table_unused[i] = 0;
                table_record_node[i]->shrink = 1;
            }
            else{
                table_unused[i] = 1;
            }
        }
    }

}

int table_finished_check(struct path *currentpath,int finalpath[]){
    int i = 0;
    while(currentpath){
        if(finalpath[i] != currentpath->node){
            return 0;
        }
        i++;
        currentpath = currentpath->next;
    }
    return 1;
}

void update_eventime(int *table,int final_table[],int n,int table_unused[],struct path *table_record_node[],int **finaloutput,int stepsum){
    for(int i=0;i<n;i++){
        if(table_unused[i]==1){
            table[i] = final_table[i];
            table_record_node[i]->next = table_record_node[final_table[i]];
            table_record_node[i]->shrink = 1;
        }
        finaloutput[stepsum][i] = table[i];
    }
}
void update_oddtime(struct path *currentpath,int *table,int final_table[],int n,struct path *table_record_node[],int table_unused[],int **finaloutput,int stepsum){
    int table_check[n];//用於檢察新路徑有沒有迴圈
    int table_node_skip[n];//用於計算新路徑跳過的node數量(須配合shrink參數)
    int table_node_left[n];//為bool確認多少點是被跳過的
    int table_temp[n];//暫存用
    struct path *temp = NULL;
    int test_store[n],test = 0;//有幾條新路徑可用
    int error_check;
    int node_skip_count;
    int max_skip_node;//用於紀錄跳過最多點的node
    int finalpath;

    for(int i = 0; i < n; i++){
        table_check[i] = 0;
        if(table_record_node[i]->shrink == 1 || i == n-1){
            table_temp[i] = 0;
            table_node_skip[i] = -1;
        }
        else{
            // printf("%d\n", i);
            table_temp[i] = 1;
            table_node_skip[i] = -1;
            if(final_table[i] != -1){
                table_node_skip[i] = 0;
                test_store[test] = i;
                test++;
            }
        }
    }
    
    #ifdef DEBUG
    printf("table_node_left\n");
    for(int i = 0; i < n; i++){
        printf("%d ", table_node_left[i]);
    }
    printf("\ntable_node_skip\n");
    for(int i = 0; i < n; i++){
        printf("%d ",table_node_skip[i]);
    }
    printf("\ntest_stored\n");
    for(int i = 0; i < test; i++){
        printf("%d ",test_store[i]);
    }
    printf("\n");
    #endif
    // for(int i = 0; i < test; i++){
    //     printf("%d ", test_store[i]);
    // }
    // printf("test:%d\n", test);
    for(int i = 0; i < test; i++){
        temp = currentpath;
        error_check = 0;
        node_skip_count = 0;
        for(int j = 0; j < n; j++){
            table_check[j] = 0;
            table_node_left[j] = table_temp[j];
        }
        while(temp){
            // printf("%d ",temp->node);
            if(table_check[temp->node] == 1){
                error_check = 1;
                break;
            }
            table_node_left[temp->node] = 0;
            table_check[temp->node] = 1;
            if(temp->node == test_store[i]){
                temp = table_record_node[final_table[temp->node]];
                // printf("\n%d\n",table_record_node[final_table[temp->node]]->node);
            }
            else{
                temp = temp->next;
            }
        }
        // printf("\n");
        #ifdef DEBUG
        for(int j = 0; j < n; j++){
            printf("%d ",table_node_left[j]);
        }
        printf("\n");
        #endif
        if(error_check==1){
            table_node_skip[test_store[i]] = -1;
        }
        else{
            for(int j = 0; j < n; j++){
                if(table_node_left[j] == 1){
                    node_skip_count++;
                }
            }
            table_node_skip[test_store[i]] = node_skip_count;
        }
    
    //     printf("node:%d\ntable_check\n");
    //     for(int i=0;i<n;i++){
    //         printf("%d ",table_check[i]);
    //     }
    //     printf("\ntable node left\n");
    //     for(int i=0;i<n;i++){
    //         printf("%d ",table_node_left[i]);
    //     }        
    //     printf("\n--------------------------------------------\n");
    //     for(int i=0;i<n;i++){
    //         printf("%d ",table_node_skip[i]);
    //     }
    //     printf("\n");
    // printf("%d----------------\n",table_node_skip[i]);
    }
        
    // for(int i=0;i<n;i++){
    //     printf("%d ",table_node_skip[i]);
    // }

    //-----選定要更動的路線
    max_skip_node = 0;
    for(int i = 0; i < n; i++){
        if(table_node_skip[i]>table_node_skip[max_skip_node]){
            max_skip_node = i;
        }
    }

    //更動table、currentpath(table_record_node)、finaloutput
    table[max_skip_node] = final_table[max_skip_node];
    table_record_node[max_skip_node]->next = table_record_node[final_table[max_skip_node]];
    table_record_node[max_skip_node]->shrink = 1;
    for(int i = 0;i<n;i++){
        finaloutput[stepsum][i] = table[i];
    }
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
    for(int i = 0; i < n; i++){
        finaloutput[i] = malloc(n*sizeof(int));
    }
    int stepsum = 1;//累積output更動的path次數
    int finalpath[n];
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
    table_record_node_initialize(table_record_node,n,table,currentpath);
    finalpath_initialize(finaltable,finalpath,n);
    #ifdef DEBUG
    for(int i=0;i<n;i++){
        if(table_record_node[i]->next == NULL){
            printf("node: %d shrink: %d next_node:x\n",table_record_node[i]->node,table_record_node[i]->shrink);
        }
        else{
            printf("node: %d shrink: %d next_node:%d\n",table_record_node[i]->node,table_record_node[i]->shrink,table_record_node[i]->next->node);            
        }
    }
    #endif
    //algorithm---------------------------
    table_unused_check_initialize(table,table_unused,n);//要注意d 也被視為unused
    // for(int i=0;i<9;i++){
    //     printf("%d ",finalpath[i]);
    // }
    // printf("\n");
    while(!(table_finished)){
        //eventime------------------------
        table_unused_check(table,table_unused,n,currentpath,finaltable,table_record_node);
        update_eventime(table,finaltable,n,table_unused,table_record_node,finaloutput,stepsum);
        stepsum++;
        // printf("eventime:%d\n",stepsum);
        
        // for(int i=0;i<n;i++){
        //     printf("%d ",finaloutput[stepsum-1][i]);
        // }
        // printf("\n");
        //oddtime-------------------------
        update_oddtime(currentpath,table,finaltable,n,table_record_node,table_unused,finaloutput,stepsum);
        stepsum++;
        // printf("oddtime:\n");
        // for(int i=0;i<n;i++){
        //     printf("%d ",finaloutput[stepsum-1][i]);
        // }
        // printf("\n");
        table_finished = table_finished_check(currentpath,finalpath);
    }
    for(int i = 0;i<n;i++){
        finaloutput[stepsum][i] = finaltable[i];
    }
    stepsum++;
    printf("%d\n",stepsum);
    for(int i=0;i<stepsum;i++){
        for(int j = 0;j<n;j++){
            printf("%d ",finaloutput[i][j]);
        }
        printf("\n");
    }

    
}