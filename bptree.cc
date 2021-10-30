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

void
print_tree_core(NODE *n)
{
	printf("["); 
	for (int i = 0; i < n->nkey; i++) {
		if (!n->isLeaf) print_tree_core(n->chi[i]); 
		printf("%d", n->key[i]); 
		if (i != n->nkey-1 && n->isLeaf) putchar(' ');
	}
	if (!n->isLeaf) print_tree_core(n->chi[n->nkey]);
	printf("]");
}

void
print_tree(NODE *node)
{
	print_tree_core(node);
	printf("\n"); fflush(stdout);
}

NODE *
find_leaf(NODE *node, int key)
{
	int kid;

	if (node->isLeaf) return node;
	for (kid = 0; kid < node->nkey; kid++) {
		if (key < node->key[kid]) break;
	}

	return find_leaf(node->chi[kid], key);
}

NODE *
insert_in_leaf(NODE *leaf, int key, DATA *data)
{
	int i;
	if (key < leaf->key[0]) {
		for (i = leaf->nkey; i > 0; i--) {
			leaf->chi[i] = leaf->chi[i-1] ;
			leaf->key[i] = leaf->key[i-1] ;
		} 
		leaf->key[0] = key;
		leaf->chi[0] = (NODE *)data;
	}
	else {
		for (i = 0; i < leaf->nkey; i++) {
			if (key < leaf->key[i]) break;
		}
		for (int j = leaf->nkey; j > i; j--) {		
			leaf->chi[j] = leaf->chi[j-1] ;
			leaf->key[j] = leaf->key[j-1] ;
		} 
		/* CodeQuiz*/
		while (leaf->key[i] != 0){
			i++;				
		}
		leaf->key[i] = key;
		leaf->chi[i] = (NODE*) data;
	}
    

	leaf->nkey++;

	return leaf;
}

NODE *
alloc_leaf(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = true;
	node->parent = parent;
	node->nkey = 0;

	return node;
}


NODE* insert_in_parent(NODE *left, int key, NODE *right){
	if(left == Root){
		//Create a new root candidate R
		NODE *newRoot;
		newRoot = alloc_leaf(NULL);
		//set R to root and return
		Root = newRoot;
		return(newRoot);
	}
}

void
insert(int key, DATA *data)
{
	NODE *leaf;

	if (Root == NULL) {
		leaf = alloc_leaf(NULL);
		Root = leaf;
	}else {
		leaf = find_leaf(Root, key);
	}

	if (leaf->nkey < (N-1)) {
		insert_in_leaf(leaf, key, data);
	}else {
		// split
		//copy L to T
		TEMP *tnode;
		for(int i=0; i < N-1 ; i++){
			tnode ->key[i] = leaf ->key[i];
			tnode->chi[i] = leaf ->chi[i];	
		}
		tnode->isLeaf =leaf ->isLeaf;
		tnode ->nkey =leaf ->nkey;

		//insert new key
		tnode -> key[N-1] = key;
		tnode -> chi[N-1] = (NODE *) data;
		tnode -> nkey ++;

		NODE *leafr;
		leafr =alloc_leaf(NULL);
		leafr -> isLeaf =tnode->isLeaf;

		// Set Pn of L’ to Pn of L
		//leafr -> chi[N-1] = leaf -> chi[N-1];

		//Set Pn of L to L’
		leaf ->chi[N-1] = leafr;
		
		//clean up L
		leaf ->nkey =0;
		for(int i=0; i<N; i++){
			leaf -> key[i] = 0;
			leaf -> chi[i] = NULL;
		}
		
		//copy from T to L;
		for(int i=0; i<N/2; i++){
			leaf ->key[i]= tnode ->key[i];
			leaf ->chi[i]= tnode ->chi[i];
		}


		//copy from T to L':
		for(int i=N/2; i<N; i++){
			leaf ->key[i-2/N] = tnode ->key[i];
			leaf ->chi[i-2/N] = tnode ->chi[i];
		}

		//printf("%d", leaf -> key[3]);

		//insert_in_parent
		NODE *newroot;
		newroot = insert_in_parent(leaf, leafr -> key[0], leafr);

		//set Key
		newroot ->isLeaf =false;
		newroot -> chi[0] = leaf;
		newroot -> chi[1] = leafr;
		newroot -> key[0] = key;
		newroot -> nkey = 1;

		leaf -> parent =newroot;
		leafr -> parent = newroot;

		return;	
	}
}

void
init_root(void)
{
	Root = NULL;
}

int 
interactive()
{
  int key;

  std::cout << "Key: ";
  std::cin >> key;

  return key;
}

int
main(int argc, char *argv[])
{
  struct timeval begin, end;

	init_root();

	printf("-----Insert-----\n");
	begin = cur_time();
  while (true) {
		insert(interactive(), NULL);
    print_tree(Root);
  }
	end = cur_time();

	return 0;
}
