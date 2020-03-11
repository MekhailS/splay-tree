#include "gtest/gtest.h"
#include "..\splay-tree\splay-tree.c"


NODE* GetEmptyNode(int key) {
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->leftChild = node->rightChild = node->parent = NULL;
    node->key.primaryKey = key;
    node->key.string = NULL;
    return(node);
}


SPL_TREE GetSimpleTreeWSevenNodes() {
    SPL_TREE tree;
    NODE* node_1 = GetEmptyNode(1);
    NODE* node_3 = GetEmptyNode(3);
    NODE* node_4 = GetEmptyNode(4);
    NODE* node_5 = GetEmptyNode(5);
    NODE* node_6 = GetEmptyNode(6);
    NODE* node_8 = GetEmptyNode(8);
    NODE* node_10 = GetEmptyNode(10);

    node_5->leftChild = node_3;
    node_5->rightChild = node_8;

    node_3->leftChild = node_1;
    node_3->rightChild = node_4;
    node_3->parent = node_5;

    node_8->leftChild = node_6;
    node_8->rightChild = node_10;
    node_8->parent = node_5;

    node_1->parent = node_3;
    
    node_4->parent = node_3;

    node_6->parent = node_8;

    node_10->parent = node_8;

    tree.root = node_5;
    return(tree);
}


TEST(RotateLeft, SimpleTree_RotateNode5) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    RotateLeft(&tree, tree.root);
    ASSERT_EQ(8, tree.root->key.primaryKey);
    ASSERT_EQ(5, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(10, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(3, tree.root->leftChild->leftChild->key.primaryKey);
    ASSERT_EQ(6, tree.root->leftChild->rightChild->key.primaryKey);
    ASSERT_EQ(8, tree.root->leftChild->parent->key.primaryKey);
    ASSERT_EQ(5, tree.root->leftChild->rightChild->parent->key.primaryKey);
}


TEST(RotateLeft, SimpleTree_RotateNode8) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    RotateLeft(&tree, tree.root->rightChild);
    ASSERT_EQ(5, tree.root->key.primaryKey);
    ASSERT_EQ(3, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(10, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(NULL, tree.root->rightChild->rightChild);
    ASSERT_EQ(8, tree.root->rightChild->leftChild->key.primaryKey);
    ASSERT_EQ(NULL, tree.root->rightChild->leftChild->rightChild);
    ASSERT_EQ(6, tree.root->rightChild->leftChild->leftChild->key.primaryKey);
}


TEST(RotateRight, SimpleTree_RotateNode5) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    RotateRight(&tree, tree.root);
    ASSERT_EQ(3, tree.root->key.primaryKey);
    ASSERT_EQ(5, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(1, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(4, tree.root->rightChild->leftChild->key.primaryKey);
    ASSERT_EQ(8, tree.root->rightChild->rightChild->key.primaryKey);
    ASSERT_EQ(3, tree.root->rightChild->parent->key.primaryKey);
    ASSERT_EQ(5, tree.root->rightChild->leftChild->parent->key.primaryKey);
}


TEST(RotateRight, SimpleTree_RotateNode3) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    RotateRight(&tree, tree.root->leftChild);
    ASSERT_EQ(5, tree.root->key.primaryKey);
    ASSERT_EQ(8, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(1, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(3, tree.root->leftChild->rightChild->key.primaryKey);
    ASSERT_EQ(NULL, tree.root->leftChild->rightChild->leftChild);
    ASSERT_EQ(4, tree.root->leftChild->rightChild->rightChild->key.primaryKey);
    ASSERT_EQ(NULL, tree.root->leftChild->leftChild);
}


TEST(DetectOperationCase, DetectNode5_NONE) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    SPL_OPERATION operation = DetectOperationCase(&tree, tree.root);
    ASSERT_EQ(NONE, operation);
}


TEST(DetectOperationCase, DetectNode3_ZIG) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    SPL_OPERATION operation = DetectOperationCase(&tree, tree.root->leftChild);
    ASSERT_EQ(ZIG, operation);
}


TEST(DetectOperationCase, DetectNode8_ZIG) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    SPL_OPERATION operation = DetectOperationCase(&tree, tree.root->rightChild);
    ASSERT_EQ(ZIG, operation);
}

TEST(DetectOperationCase, DetectNode1_ZIG_ZIG) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    SPL_OPERATION operation = DetectOperationCase(&tree, tree.root->leftChild->leftChild);
    ASSERT_EQ(ZIG_ZIG, operation);
}


TEST(DetectOperationCase, DetectNode10_ZIG_ZIG) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    SPL_OPERATION operation = DetectOperationCase(&tree, tree.root->rightChild->rightChild);
    ASSERT_EQ(ZIG_ZIG, operation);
}


TEST(DetectOperationCase, DetectNode4_ZIG_ZAG) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    SPL_OPERATION operation = DetectOperationCase(&tree, tree.root->leftChild->rightChild);
    ASSERT_EQ(ZIG_ZAG, operation);
}


TEST(DetectOperationCase, DetectNode6_ZIG_ZAG) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    SPL_OPERATION operation = DetectOperationCase(&tree, tree.root->rightChild->leftChild);
    ASSERT_EQ(ZIG_ZAG, operation);
}


SPL_TREE GetTreeW11Nodes() {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    NODE* nodeNeg1 = GetEmptyNode(-1);
    NODE* node2 = GetEmptyNode(2);
    NODE* node9 = GetEmptyNode(9);
    NODE* node12 = GetEmptyNode(12);

    NODE* node1 = tree.root->leftChild->leftChild;
    NODE* node10 = tree.root->rightChild->rightChild;

    node1->leftChild = nodeNeg1;
    node1->rightChild = node2;

    node10->leftChild = node9;
    node10->rightChild = node12;

    nodeNeg1->parent = node1;
    node2->parent = node1;

    node9->parent = node10;
    node12->parent = node10;

    return(tree);
}


TEST(Splay, SplayFrom5_None) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node5 = tree.root;
    Splay(&tree, node5);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(node5, tree.root);
    ASSERT_EQ(3, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(8, tree.root->rightChild->key.primaryKey);
}


TEST(Splay, SplayFrom3_OneZigFromLeft) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node3 = tree.root->leftChild;
    Splay(&tree, node3);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(node3, tree.root);
    ASSERT_EQ(1, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(5, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(4, tree.root->rightChild->leftChild->key.primaryKey);
    ASSERT_EQ(8, tree.root->rightChild->rightChild->key.primaryKey);
}


TEST(Splay, SplayFrom8_OneZigFromRight) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node8 = tree.root->rightChild;
    Splay(&tree, node8);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(node8, tree.root);
    ASSERT_EQ(5, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(3, tree.root->leftChild->leftChild->key.primaryKey);
    ASSERT_EQ(6, tree.root->leftChild->rightChild->key.primaryKey);
    ASSERT_EQ(10, tree.root->rightChild->key.primaryKey);
}

TEST(Splay, SplayFrom1_OneZigZigFromLeft) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node1 = tree.root->leftChild->leftChild;
    Splay(&tree, node1);
    ASSERT_EQ(node1, tree.root);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(-1, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(3, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(2, tree.root->rightChild->leftChild->key.primaryKey);
    ASSERT_EQ(5, tree.root->rightChild->rightChild->key.primaryKey);
    ASSERT_EQ(4, tree.root->rightChild->rightChild->leftChild->key.primaryKey);
    ASSERT_EQ(8, tree.root->rightChild->rightChild->rightChild->key.primaryKey);
}


TEST(Splay, SplayFrom10_OneZigZigFromRight) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node10 = tree.root->rightChild->rightChild;
    Splay(&tree, node10);
    ASSERT_EQ(node10, tree.root);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(12, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(8, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(9, tree.root->leftChild->rightChild->key.primaryKey);
    ASSERT_EQ(5, tree.root->leftChild->leftChild->key.primaryKey);
    ASSERT_EQ(6, tree.root->leftChild->leftChild->rightChild->key.primaryKey);
    ASSERT_EQ(3, tree.root->leftChild->leftChild->leftChild->key.primaryKey);
}


TEST(Splay, SplayFrom4_OneZigZagFromLeft) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node4 = tree.root->leftChild->rightChild;
    Splay(&tree, node4);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(node4, tree.root);
    ASSERT_EQ(3, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(5, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(1, tree.root->leftChild->leftChild->key.primaryKey);
    ASSERT_EQ(NULL, tree.root->leftChild->rightChild);
    ASSERT_EQ(NULL, tree.root->rightChild->leftChild);
    ASSERT_EQ(8, tree.root->rightChild->rightChild->key.primaryKey);
}


TEST(Splay, SplayFrom6_OneZigZagFromRight) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node6 = tree.root->rightChild->leftChild;
    Splay(&tree, node6);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(node6, tree.root);
    ASSERT_EQ(5, tree.root->leftChild->key.primaryKey);
    ASSERT_EQ(8, tree.root->rightChild->key.primaryKey);
    ASSERT_EQ(3, tree.root->leftChild->leftChild->key.primaryKey);
    ASSERT_EQ(NULL, tree.root->leftChild->rightChild);
    ASSERT_EQ(NULL, tree.root->rightChild->leftChild);
    ASSERT_EQ(10, tree.root->rightChild->rightChild->key.primaryKey);
}


TEST(Splay, SplayFrom2_ZigZagAndZig) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node2 = tree.root->leftChild->leftChild->rightChild;
    Splay(&tree, node2);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(node2, tree.root);
}


TEST(Splay, SplayFromNeg1_ZigZagAndZig) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* nodeNeg1 = tree.root->leftChild->leftChild->leftChild;
    Splay(&tree, nodeNeg1);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(nodeNeg1, tree.root);
}


TEST(Splay, SplayFrom9_ZigZagAndZig) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node9 = tree.root->rightChild->rightChild->leftChild;
    Splay(&tree, node9);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(node9, tree.root);
}


TEST(Splay, SplayFrom12_ZigZagAndZig) {
    SPL_TREE tree = GetTreeW11Nodes();
    NODE* node12 = tree.root->rightChild->rightChild->rightChild;
    Splay(&tree, node12);
    ASSERT_EQ(NULL, tree.root->parent);
    ASSERT_EQ(node12, tree.root);
}


TEST(Search, SearchTopElem5_Found) {
    SPL_TREE tree = GetTreeW11Nodes();
    int res = Search(&tree, tree.root->key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(5, tree.root->key.primaryKey);
}


TEST(Search, SearchBottomElem2_Found) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 2, NULL };
    int res = Search(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(2, tree.root->key.primaryKey);
}


TEST(Search, SearchBottomElem12_Found) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 12, NULL };
    int res = Search(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(12, tree.root->key.primaryKey);
}


TEST(Search, SearchMidElem3_Found) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 3, NULL };
    int res = Search(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(3, tree.root->key.primaryKey);
}


TEST(Search, SearchElem100_NotFound) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 100, NULL };
    int res = Search(&tree, key);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(5, tree.root->key.primaryKey);
}


TEST(Search, SearchInEmpty_NotFound) {
    SPL_TREE tree;
    tree.root = NULL;
    KEY key = { 1, NULL };
    int res = Search(&tree, key);
    ASSERT_EQ(res, 0);
}


TEST(Insert, Insert2NotInTree_SuccAdd) {
    DATA data = { 0 };
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 2, NULL };
    int res = Insert(&tree, key, data);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(2, tree.root->key.primaryKey);
}


TEST(Insert, Insert14NotInTree_SuccAdd) {
    DATA data = { 0 };
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 14, NULL };
    int res = Insert(&tree, key, data);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(14, tree.root->key.primaryKey);
}


TEST(Insert, Insert5InTree_FailAdd) {
    DATA data = { 0 };
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 5, NULL };
    int res = Insert(&tree, key, data);
    ASSERT_EQ(res, 0);
}


TEST(Insert, Insert1InTree_FailAdd) {
    DATA data = { 0 };
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 1, NULL };
    int res = Insert(&tree, key, data);
    ASSERT_EQ(res, 0);
}


TEST(Insert, InsertIntoEmpty_SuccAdd) {
    DATA data = { 0 };
    SPL_TREE tree;
    tree.root = NULL;
    KEY key = { 2, NULL };
    int res = Insert(&tree, key, data);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(2, tree.root->key.primaryKey);
}


TEST(Delete, DeleteRoot_SuccDel) {
    SPL_TREE tree = GetTreeW11Nodes();
    int res = Delete(&tree, tree.root->key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(4, tree.root->key.primaryKey);
}


TEST(Delete, DeleteMid3_SuccDel) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 3, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(2, tree.root->key.primaryKey);
}


TEST(Delete, DeleteMid8_SuccDel) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 8, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(6, tree.root->key.primaryKey);
}


TEST(Delete, DeleteLowerMid1_SuccDel) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 1, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(-1, tree.root->key.primaryKey);
}


TEST(Delete, DeleteLowerMid4_SuccDel) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 4, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(3, tree.root->key.primaryKey);
}


TEST(Delete, DeleteLowerMid6_SuccDel) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 6, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(5, tree.root->key.primaryKey);
}


TEST(Delete, DeleteLowerMid10_SuccDel) {
    SPL_TREE tree = GetTreeW11Nodes();
    KEY key = { 10, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(9, tree.root->key.primaryKey);
}


TEST(Delete, DeleteBottom1_SuccDel) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 1, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(3, tree.root->key.primaryKey);
}


TEST(Delete, DeleteBottom4_SuccDel) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 4, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(3, tree.root->key.primaryKey);
}


TEST(Delete, DeleteBottom6_SuccDel) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 6, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(5, tree.root->key.primaryKey);
}


TEST(Delete, DeleteBottom10_SuccDel) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 10, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 1);
    ASSERT_EQ(8, tree.root->key.primaryKey);
}


TEST(Delete, DeleteNodeNotInTree_FailDel) {
    SPL_TREE tree = GetSimpleTreeWSevenNodes();
    KEY key = { 50, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 0);
}


TEST(Delete, EmptyTree_FailDel) {
    SPL_TREE tree;
    tree.root = NULL;
    KEY key = { 1, NULL };
    int res = Delete(&tree, key);
    ASSERT_EQ(res, 0);
}