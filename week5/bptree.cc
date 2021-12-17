#include "bptree.h"
#include <vector>
#include <sys/time.h>

//プロトタイプ宣言
void print_tree_core(NODE *n);
void print_tree(NODE *node);
NODE *find_leaf(NODE *node, int key);
NODE *insert_in_leaf(NODE *leaf, int key, DATA *data);
NODE *alloc_leaf(NODE *parent);
void create_firstRoot(NODE *left, int key, NODE *right);
int slide_element_in_node(NODE *node, int key, NODE *left, NODE *right);
TEMP *createtemp(NODE *leaf);
int slide_element_in_tempnode(TEMP *tnode, int key);
void internal_split(TEMP *tnode, NODE *left);
void insert_in_parent(NODE *left, int key, NODE *right);
void split(int key, DATA *data, TEMP *tnode, NODE *leaf);
NODE delete_entry(NODE *leaf, int key, DATA *data);
void deletion(int key, DATA *data);
void insert(int key, DATA *data);
void init_root(void);
int interactive();
int main(int argc, char *argv[]);

#define SIZE 1000 * 1000
int data_array[SIZE];

struct timeval
cur_time(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

void print_tree_core(NODE *n)
{
	printf("[");
	for (int i = 0; i < n->nkey; i++)
	{
		if (!n->isLeaf)
			print_tree_core(n->chi[i]);
		printf("%d", n->key[i]);
		if (i != n->nkey - 1 && n->isLeaf)
			putchar(' ');
	}
	if (!n->isLeaf)
		print_tree_core(n->chi[n->nkey]);
	printf("]");
}

void print_tree(NODE *node)
{
	print_tree_core(node);
	printf("\n");
	fflush(stdout);
}

NODE *find_leaf(NODE *node, int key)
{
	int kid;

	if (node->isLeaf)
		return node;
	for (kid = 0; kid < node->nkey; kid++)
	{
		if (key < node->key[kid])
			break;
	}

	return find_leaf(node->chi[kid], key);
}

NODE *insert_in_leaf(NODE *leaf, int key, DATA *data)
{
	int i;
	if (key < leaf->key[0])
	{
		for (i = leaf->nkey; i > 0; i--)
		{
			leaf->chi[i] = leaf->chi[i - 1];
			leaf->key[i] = leaf->key[i - 1];
		}
		leaf->key[0] = key;
		leaf->chi[0] = (NODE *)data;
	}
	else
	{
		for (i = 0; i < leaf->nkey; i++)
		{
			if (key < leaf->key[i])
				break;
		}
		for (int j = leaf->nkey; j > i; j--)
		{
			leaf->chi[j] = leaf->chi[j - 1];
			leaf->key[j] = leaf->key[j - 1];
		}
		/* CodeQuiz*/
		leaf->key[i] = key;
		leaf->chi[i] = (NODE *)data;
	}
	leaf->nkey++;
	return leaf;
}

NODE *alloc_leaf(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE))))
		ERR;
	node->isLeaf = true;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

void create_firstRoot(NODE *left, int key, NODE *right)
{
	// Create a new root candidate R
	NODE *newRoot;
	newRoot = alloc_leaf(NULL);
	newRoot->isLeaf = false;

	newRoot->chi[0] = left;
	newRoot->chi[1] = right;
	newRoot->key[0] = key;
	newRoot->nkey++;
	right->parent = newRoot;
	left->parent = newRoot;
	// set R to root and return
	Root = newRoot;
	return;
}

int slide_element_in_node(NODE *node, int key, NODE *left, NODE *right)
{
	int i, j;
	// keyが一番小さい場合
	if (key < node->key[0])
	{
		for (i = N - 1; i > 0; i--)
		{
			node->key[i - 1] = node->key[i - 2];
			node->chi[i] = node->chi[i - 1];
		}
		return i = 0;
	} //それ以外の場合
	else
	{
		for (i = 0; i < N; i++)
		{
			if (key < node->key[i])
			{
				break;
			}
			// 0がそもそもダメな仕様になっていた。0がkeyでも大丈夫なように修正
			if (node->key[i] > node->key[i + 1])
			{
				i++;
				break;
			}
		}
		for (j = N - 1; j > i; j--)
		{
			node->chi[j] = node->chi[j - 1];
			node->key[j - 1] = node->key[j - 2];
		}
		return i;
	}
}

TEMP *createtemp(NODE *leaf)
{
	// temoporary nodeの作成・leafからコピー
	TEMP *tnode;
	tnode = (TEMP *)calloc(1, sizeof(TEMP));
	for (int i = 0; i < N - 1; i++)
	{
		tnode->key[i] = leaf->key[i];
		tnode->chi[i] = leaf->chi[i];
	}
	tnode->isLeaf = leaf->isLeaf;
	tnode->nkey = leaf->nkey;
	return (tnode);
}

int slide_element_in_tempnode(TEMP *tnode, int key)
{
	int i, j;
	//もしkeyが一番左に挿入される場合、temp nodeを一つずつ右に寄せる
	if (key < tnode->key[0])
	{
		for (int i = N; i > 0; i--)
		{
			tnode->key[i] = tnode->key[i - 1];
			tnode->chi[i] = tnode->chi[i - 1];
		}
		return i = 0;
	}
	else
	//それ以外の場合(keyがtnodeの真ん中か一番右に挿入される場合、挿入よりも右の領域を一つずつ右に寄せる)
	{
		for (i = 0; i < N; i++)
		{
			if (key < tnode->key[i])
			{
				break;
			}
			// 0がそもそもダメな仕様になっていた。0がkeyでも大丈夫なように修正
			if (tnode->key[i] > tnode->key[i + 1])
			{
				i++;
				break;
			}
		}
		for (j = N; j > i; j--)
		{
			tnode->key[j] = tnode->key[j - 1];
			tnode->chi[j] = tnode->chi[j - 1];
		}
		return i;
	}
}

void internal_split(TEMP *tnode, NODE *left)
{
	NODE *right;
	right = alloc_leaf(NULL);
	right->isLeaf = left->isLeaf;
	right->parent = left->parent;

	// clean up leaf
	left->nkey = 0;
	for (int i = 0; i < N; i++)
	{
		left->key[i] = 0;
		left->chi[i] = 0;
	}

	// copy from tnode to left
	for (int i = 0; i < N / 2; i++)
	{
		left->key[i] = tnode->key[i];
		left->nkey++;
	}

	for (int i = 0; i < N / 2 + 1; i++)
	{
		left->chi[i] = tnode->chi[i];
	}

	// copy from tnode to right
	right->key[0] = tnode->key[3];
	right->chi[0] = tnode->chi[3];
	right->chi[1] = tnode->chi[4];
	right->nkey++;

	// parentが変わるnodeたちのparentを修正(これだけに1時間かかってる)
	right->chi[0]->parent = right;
	right->chi[1]->parent = right;

	// insert_in_parent
	insert_in_parent(left, tnode->key[N / 2], right);

	free(tnode);
	return;
}

void insert_in_parent(NODE *left, int key, NODE *right)
{
	int i;
	if (left == Root)
	{
		// split(1回目)
		create_firstRoot(left, key, right);
	}
	else
	{
		NODE *parent = left->parent;
		// split(2回目以降)
		if (parent->nkey < N - 1)
		{
			i = slide_element_in_node(parent, key, left, right);

			// slideした場所にkeyを代入
			parent->key[i] = key;
			parent->nkey++;
			parent->chi[i] = left;
			parent->chi[i + 1] = right;
		}
		else
		{ // internal split(Root nodeの中身が全部埋まっている時)
			TEMP *tnode;
			tnode = createtemp(parent);

			//　この1行にも1時間かかった 　//　この行は少し汚い
			tnode->chi[N - 1] = parent->chi[N - 1];

			int i;
			i = slide_element_in_tempnode(tnode, key);

			// slideした場所にkeyを代入
			tnode->key[i] = key;
			tnode->chi[i] = left;
			tnode->chi[i + 1] = right;
			tnode->nkey++;

			internal_split(tnode, parent);
		}
	}
}

void split(int key, DATA *data, TEMP *tnode, NODE *leaf)
{
	int i;
	i = slide_element_in_tempnode(tnode, key);

	// slideした場所にkeyを代入
	tnode->key[i] = key;
	tnode->nkey++;

	//新しいnodeの生成
	NODE *new_leaf;
	new_leaf = alloc_leaf(NULL);
	new_leaf->isLeaf = true;
	new_leaf->parent = leaf->parent;

	// clean up leaf
	leaf->nkey = 0;
	for (int i = 0; i < N; i++)
	{
		leaf->key[i] = 0;
		leaf->chi[i] = 0;
	}

	// copy from tnode to leaf;
	for (int i = 0; i < N / 2; i++)
	{
		leaf->key[i] = tnode->key[i];
		leaf->chi[i] = tnode->chi[i];
		leaf->nkey++;
	}

	// copy from tnode to new_leaf:
	for (int i = N / 2; i < N; i++)
	{
		new_leaf->key[i - N / 2] = tnode->key[i];
		new_leaf->chi[i - N / 2] = tnode->chi[i];
		new_leaf->nkey++;
	}

	//ポインタの付け替え
	new_leaf->chi[N - 1] = tnode->chi[N];
	leaf->chi[N - 1] = new_leaf;

	free(tnode);

	// insert_in_parent
	insert_in_parent(leaf, new_leaf->key[0], new_leaf);
}

void insert(int key, DATA *data)
{
	NODE *leaf;

	if (Root == NULL)
	{
		leaf = alloc_leaf(NULL);
		Root = leaf;
	}
	else
	{
		leaf = find_leaf(Root, key);
	}

	if (leaf->nkey < (N - 1))
	{
		insert_in_leaf(leaf, key, data);
		// printf("[%d]", leaf->nkey);
		// DDD(leaf->nkey);
	}
	else
	{
		// temoporary nodeの作成
		TEMP *tnode;
		tnode = createtemp(leaf);

		// split
		split(key, data, tnode, leaf);
	}
}

NODE delete_entry(NODE *leaf, int key, DATA *data)
{
	// leaf nodeのkeyを消す
	int i, j;
	for (i = 0; i < N; i++)
	{
		if (leaf->key[i] == key)
		{
			break;
		}
	}

	for (j = i; i < N; i++)
	{
		leaf->chi[j] = leaf->chi[j + 1];
		leaf->key[j] = leaf->key[j + 1];
	}
	leaf->chi[N - 1] = 0;
	leaf->key[N - 2] = 0;
	leaf->nkey--;

	// if (N is the root and N has only one remaining child)
	if (leaf == Root && leaf->nkey == 0)
	{
		free(leaf);
		NODE *new_leaf;
		new_leaf = alloc_leaf(NULL);
		new_leaf = Root;
	}

	// else if (N has too few values/pointers)
	else if (leaf->nkey == 0)
	{
		NODE *newnode;
		newnode = alloc_leaf(NULL);
	}

	return *leaf;
}

void deletion(int key, DATA *data)
{
	NODE *leaf;
	leaf = find_leaf(Root, key);
	delete_entry(leaf, key, data);
}

void init_root(void)
{
	Root = NULL;
}

int interactive()
{
	int key;

	std::cout << "Key: ";
	std::cin >> key;

	return key;
}

void search(int key)
{
	NODE *node;
	node = find_leaf(Root, key);
	for (int i = 0; i < node->nkey; i++)
	{
		if (node->key[i] == key)
		{
			return;
		}
	}
	printf("%d", key);
	ERR;
}

void arraygenerate()
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < SIZE; i++)
	{
		data_array[i] = rand();
	}

	//配列の中身確認用
	/*for (i = 0; i < SIZE; ++i)
	{
		printf("%d\n", array[i]);
	}*/
}

int main(int argc, char *argv[])
{
	struct timeval begin, end;

	init_root();

	char order = '\n';
	cout << "[i] inputting order, [a] ascending order, [d] descending order, [r] random order:";
	cin >> order;

	//昇順にinsertする場合
	if (order == 'a')
	{
		begin = cur_time();
		for (int i = 0; i < SIZE; i++)
		{
			insert(i, NULL);
			// print_tree(Root);
		}
		for (int i = 0; i < SIZE; i++)
		{
			search(i);
		}
		end = cur_time();
		cout << "no error has detected.";
	}

	//降順にinsertする場合
	if (order == 'd')
	{
		begin = cur_time();
		for (int i = SIZE - 1; i >= 0; i--)
		{
			insert(i, NULL);
			// print_tree(Root);
		}
		for (int i = 0; i < SIZE; i++)
		{
			search(i);
		}
		end = cur_time();
		cout << "no error has detected.";
	}

	//ランダム順にinsertする場合
	if (order == 'r')
	{
		begin = cur_time();
		arraygenerate();
		for (int i = 0; i < SIZE; i++)
		{
			insert(data_array[i], NULL);
			// print_tree(Root);
		}
		for (int i = 0; i < SIZE; i++)
		{
			search(data_array[i]);
		}
		end = cur_time();
		cout << "no error has detected.";
	}

	//入力順にinsertする場合
	if (order == 'i')
	{
		printf("-----Insert-----\n");
		begin = cur_time();

		char operation = '\0';
		//入力順にinsert
		while (true)
		{
			cout << "[i] insert, [s] search, [d] delete:";
			cin >> operation;

			if (operation == 'i')
			{
				//入力順にinsert
				insert(interactive(), NULL);
				print_tree(Root);
			}
			if (operation == 'd')
			{
				deletion(interactive(), NULL);
				print_tree(Root);
			}
			if (operation == 's')
			{
				for (int i = 0; i < SIZE; i++)
				{
					search(i);
				}
			}
		}

		end = cur_time();
	}
	return 0;
}