#include "bptree.h"
#include <vector>
#include <sys/time.h>

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

NODE *
find_leaf(NODE *node, int key)
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

NODE *
insert_in_leaf(NODE *leaf, int key, DATA *data)
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
		while (leaf->key[i] != 0)
		{
			i++;
		}
		leaf->key[i] = key;
		leaf->chi[i] = (NODE *)data;
	}

	leaf->nkey++;

	return leaf;
}

NODE *
alloc_leaf(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE))))
		ERR;
	node->isLeaf = true;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

void insert_in_parent(NODE *left, int key, NODE *right)
{
	if (left == Root)
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
}

TEMP *createtemp(NODE *leaf)
{
	// temoporary nodeの作成・leafからコピー
	TEMP *tnode;
	if (!(tnode = (TEMP *)calloc(1, sizeof(TEMP))))
		ERR;
	for (int i = 0; i < N - 1; i++)
	{
		tnode->key[i] = leaf->key[i];
		tnode->chi[i] = leaf->chi[i];
	}
	tnode->isLeaf = leaf->isLeaf;
	tnode->nkey = leaf->nkey;
	return (tnode);
}

void split(int key, DATA *data, TEMP *tnode, NODE *leaf)
{
	//もしkeyが一番小さい側から入る場合、temp nodeを一つずつ右に寄せる
	if (key <= tnode->key[0])
	{
		for (int i = N - 1; i > 0; i--)
		{
			tnode->key[i] = tnode->key[i - 1];
			tnode->chi[i] = tnode->chi[i - 1];
		}
	}

	// insert new key
	if (key >= tnode->key[N - 2])
	{
		tnode->key[N - 1] = key;
		tnode->chi[N - 1] = (NODE *)data;
		tnode->nkey++;
	}
	else
	{
		tnode->key[0] = key;
		tnode->chi[0] = (NODE *)data;
		tnode->nkey++;
	}

	//新しいnodeの生成
	NODE *new_leaf;
	if (!(new_leaf = (NODE *)calloc(1, sizeof(NODE))))
		ERR;
	new_leaf->isLeaf = tnode->isLeaf;
	new_leaf->parent = leaf->parent;

	// clean up leaf
	leaf->nkey = 0;
	for (int i = 0; i < N; i++)
	{
		leaf->key[i] = 0;
		leaf->chi[i] = NULL;
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

	leaf->chi[N - 1] = new_leaf;

	free(tnode);

	// insert_in_parent
	insert_in_parent(leaf, new_leaf->key[0], new_leaf);
	return;
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

int main(int argc, char *argv[])
{
	struct timeval begin, end;

	init_root();

	printf("-----Insert-----\n");
	begin = cur_time();
	while (true)
	{
		insert(interactive(), NULL);
		print_tree(Root);
	}
	end = cur_time();

	return 0;
}
