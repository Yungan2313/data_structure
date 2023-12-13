#include <stdio.h>
#include <stdlib.h>

// 定義二叉樹的節點結構
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 創建一個新的二叉樹節點
TreeNode *createNode(int data) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 以完全二叉樹的形式構建二叉樹
TreeNode *buildCompleteBinaryTree(int vertices) {
    // 使用陣列儲存樹的節點
    TreeNode **nodes = (TreeNode **)malloc(vertices * sizeof(TreeNode *));
    
    // 創建所有的節點
    for (int i = 0; i < vertices; i++) {
        nodes[i] = createNode(i + 1);
    }

    // 連接每個節點的左右子樹
    for (int i = 0; i < vertices / 2 - 1; i++) {
        nodes[i]->left = nodes[2 * i + 1];
        nodes[i]->right = nodes[2 * i + 2];
    }

    // 連接最後一個父節點的左子樹
    nodes[vertices / 2 - 1]->left = nodes[2 * (vertices / 2 - 1) + 1];

    // 釋放中間使用的陣列
    free(nodes);

    // 返回樹的根節點
    return nodes[0];
}

// 中序遍歷二叉樹
void inorderTraversal(TreeNode *root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

int main() {
    int vertices;
    printf("Enter the number of vertices for the complete binary tree: ");
    scanf("%d", &vertices);

    if (vertices <= 0) {
        printf("Invalid number of vertices.\n");
        return 1;
    }

    // 建立完全二叉樹
    TreeNode *root = buildCompleteBinaryTree(vertices);

    // 中序遍歷並輸出樹的結構
    printf("Inorder Traversal of the Complete Binary Tree:\n");
    inorderTraversal(root);
    printf("\n");

    return 0;
}
