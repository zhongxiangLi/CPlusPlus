#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED				1
#define BLACK 			2

typedef int KEY_TYPE;


typedef struct _rbtree_node {
	unsigned char color;
	struct _rbtree_node *right;
	struct _rbtree_node *left;
	struct _rbtree_node *parent;
	KEY_TYPE key;
	void *value;
} rbtree_node;



typedef struct _rbtree {
	rbtree_node *root;
	rbtree_node *nil;
} rbtree;

void rebtree_insert_fixup(rbtree*T, rbtree_node*z);

void rbtree_left_rotate(rbtree*T, rbtree_node *x) {

	rbtree_node *y = x->right;  
	x->right = y->left; //1 1
	if (y->left != T->nil) { //1 2
		y->left->parent = x;
	}

	y->parent = x->parent; //1 3
	if (x->parent == T->nil) { //1 4
		T->root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}

	y->left = x; //1 5
	x->parent = y; //1 6

}

void rbtree_right_rotate(rbtree *T, rbtree_node *y) {

	rbtree_node *x = y->left;

	y->left = x->right;
	if (x->right != T->nil) {
		x->right->parent = y;
	}

	x->parent = y->parent;
	if (y->parent == T->nil) {
		T->root = x;
	}
	else if (y == y->parent->right) {
		y->parent->right = x;
	}
	else {
		y->parent->left = x;
	}

	x->right = y;
	y->parent = x;
}

void rbtree_insert(rbtree *T, rbtree_node*z) {
	
	rbtree_node*y = T->nil;
	rbtree_node*x = T->root;

	while (x != T->nil)
	{
		y = x;
		if (z->key < x->key) {
			x = x->left;
		}else if (z->key > x->key) {
			x = x->right;
		}else
			return;
	}

	z->parent = y;
	if (y == T->nil) {
		T->root = z;
	}
	else if (z->key < y->key) {
		y->left = z;
	}
	else {
		y->right = z;
	}

	z->left = z->right = T->nil;
	z->color = RED;

	rebtree_insert_fixup(T, z);

}
//插入  分为以下几种情况
//叔父节点是红色，此时 只需要改变父节点 叔父节点颜色为黑色，祖父节点为红色，回溯到祖父节点继续
//叔父节点是黑色，节点属于父节点的右（左）子树，并且父节点也属于祖父节点的右（左）子树，改变父节点为黑色，祖父为红色，左（右）旋操作；
//叔父节点是黑色，节点属于父节点的右（左）子树，父节点属于祖父的左（右）子树，节点回溯到父节点，进行左（右）旋；
//循环结束，根节点置为黑
void rebtree_insert_fixup(rbtree*T, rbtree_node*z) {
	while (z->parent->color == RED){
		if (z->parent == z->parent->parent->left){
			rbtree_node *y = z->parent->parent->right;//  叔父节点
			if (y->color == RED){ //-------------------------------------情况1.1
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}else {
				if (z == z->parent->right){//-----------------------------情况2
					z = z->parent;
					rbtree_left_rotate(T, z);
				}
				z->parent->color = BLACK;//-------------------------------情况3
				z->parent->parent->color = RED;
				rbtree_right_rotate(T, z->parent->parent);
			}
		}else {
			rbtree_node * y = z->parent->parent->left;
			if (y->color == RED) { //----------------------------------情况1.2
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}else {
				if (z == z->parent->left) {//---------------------------情况4
					z = z->parent;
					rbtree_right_rotate(T, z);
				}
				z->parent->color = BLACK;//----------------------------情况5
				z->parent->parent->color = RED;
				rbtree_left_rotate(T, z->parent->parent);
			}
		}
	}
	T->root->color = BLACK;
}
//删除是4种情况 y为黑色，x是y的右子树，如果x是红色，把x变成黑色；如果x是黑色，需要进行调整
//覆盖节点 z
//删除节点 y
//轴心节点 x
//
//
rbtree_node *rbtree_mini(rbtree*T, rbtree_node*x) {
	while (x->left != T->nil)
	{
		x = x->left;

	}
	return x;
}
rbtree_node*rbtree_maxi(rbtree*T, rbtree_node *x) {
	while (x->right != T->nil)
	{
		x = x->right;
	}
	return x;
}
//找出节点x的后继节点，
//如果x的右子树不为空，则后继节点是x的右子树的最小节点
//如果x的右子树节点为空，则回溯，知道x节点不是其父节点的右子树时，则父节点就是后继的节点
rbtree_node* rbtree_successor(rbtree*T, rbtree_node*x) {
	
	rbtree_node *y = x->parent;
	if (x->right != T->nil)
	{
		return rbtree_mini(T, x->right);
	}

	while (y != T->nil && x == y->right)
	{
		x = y;
		y = y->parent;
	}
	return y;
}
void retree_delete_fixup(rbtree*T, rbtree_node*x) {

	while (x != T->root && x->color == BLACK)
	{
		if (x == x->parent->left)
		{
			rbtree_node* w = x->parent->right;
			if(w->color == RED)//1
			{
				w->color = BLACK;
				x->parent->color = RED;
				rbtree_left_rotate(T, x->parent);
				w = x->parent->right;
			}
			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			}else {
				//
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rbtree_right_rotate(T, w);
					w = x->parent->right;
				}
				//兄弟节点右子树时红色 6
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rbtree_left_rotate(T, x->parent);

				x = T->root;
			}
		}
		else {
			//兄弟节点颜色如果是红色， 调整为黑色，并且将父节点颜色设置为红色
			rbtree_node *w = x->parent->left;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				rbtree_right_rotate(T, x->parent);
				w = x->parent->left;
			}
			//兄弟节点的左右孩子都为黑色，将兄弟节点调整为红色，x节点回溯 删除节点情况3
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}
			else {
				// 兄弟节点左节点为黑色，将右节点也设置为黑色进行左旋
				if (w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					rbtree_left_rotate(T, w);
					w = x->parent->left;
				}
				
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rbtree_right_rotate(T, x->parent);

				x = T->root;
			}
		}
	}

	x->color = BLACK;

}
//删除节点存在4中情况
//删除的节点左右子树都非空，找到后继节点，将后继节点转移到此节点，将后继节点作为删除的节点；然后做相应调整
//删除的节点左右子树都为空
//删除的节点左子树为空，右子树不空
//删除的节点右子树为空，左子树不空
//

rbtree_node *rbtree_delete_node(rbtree*T, rbtree_node *z) {

	rbtree_node * y = z;
	rbtree_node* x = T->nil;

	if (z->left != T->nil && z->right != T->nil)
		y = rbtree_successor(T, z);
	
	x->parent = y->parent;

	if (y->left != T->nil)
	{
		x = y->left;
	}
	else if (y->right != T->nil) {
		x = y->right;
	}

	if (y->parent == T->nil)
	{
		x = T->root;
	}
	else {
		if (y == y->parent->left)
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}

	}

	if (y != z) {
		z->key = y->key;
		z->value = y->value;
	}

	if (y->color == BLACK)
	{
		retree_delete_fixup(T, x);
	}

	return y;
}



















rbtree_node*rbtree_delete(rbtree*T, rbtree_node*z) {

	rbtree_node *y = T->nil;//最终要被删除的节点
	rbtree_node *x = T->nil;//删除节点的子节点


	if (z->left == T->nil || z->right==T->nil)
	{
		y = z;
	}
	else
	{
		y = rbtree_successor(T, z);//情况3 左右子树都存在，找到后继节点
	}

	if (y->left != T->nil)
	{
		x = y->left;
	}
	else if (y->right != T->nil) {
		x = y->right;
	}


//begin 将删除节点的子节点与父节点关联起来
	x->parent = y->parent;

	if (y->parent == T->nil)
	{
		T->root = x;
	}
	else if (y == y->parent->left) {
		y->parent->left = x;
	}
	else {
		y->parent->right = x;
	}
//end
	//这种情况出现在删除节点存在左右子树，情况3；将后继节点y付给本应删除的z节点，真正删除的是y节点。
	if (y != z) {
		z->key = y->key;
		z->value = y->value;
	}

	//删除节点为黑色，破坏了红黑树的性质，需要做调整
	if (y->color == BLACK)
	{
		retree_delete_fixup(T, x);
	}
	return y;

}

void rbtree_traversal(rbtree *T, rbtree_node *node) {
	if (node != T->nil) {
		rbtree_traversal(T, node->left);
		printf("key:%d, color:%d\n", node->key, node->color);
		rbtree_traversal(T, node->right);
	}
}

rbtree_node *rbtree_search(rbtree *T, KEY_TYPE key) {

	rbtree_node *node = T->root;
	while (node != T->nil) {
		if (key < node->key) {
			node = node->left;
		}
		else if (key > node->key) {
			node = node->right;
		}
		else {
			return node;
		}
	}
	return T->nil;
}


#define MAX_LEN 1024*32




int main()
{
	int keyArray[20] = { 24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15 };

	rbtree *T = (rbtree *)malloc(sizeof(rbtree));
	if (T == NULL) {
		printf("malloc failed\n");
		return -1;
	}

	T->nil = (rbtree_node*)malloc(sizeof(rbtree_node));
	T->nil->color = BLACK;
	T->root = T->nil;

	rbtree_node *node = T->nil;
	int i = 0;
	for (i = 0; i < 20; i++) {
		printf("node %d\n", keyArray[i]);
		node = (rbtree_node*)malloc(sizeof(rbtree_node));
		node->key = keyArray[i];
		node->value = NULL;

		rbtree_insert(T, node);

	}
	rbtree_traversal(T, T->root);
	printf("----------------------------------------\n");

	int keyArray1[20] = { 24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15 };
	for (i = 0; i < 20; i++) {

		rbtree_node *node = rbtree_search(T, keyArray1[i]);
		printf("delete node value is %d\n", keyArray1[i]);
		rbtree_node *cur = rbtree_delete(T, node);
		free(cur);

		rbtree_traversal(T, T->root);
		printf("----------------------------------------\n");
	}
	return 0;
}
