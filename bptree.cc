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
    	// future work

		//leafはroot nodeじゃなくなった
		/*leaf ->isLeaf =false;*/

		//copy L to T
		TEMP *tnode;
		for(int i=0; i < N ; i++){
			tnode ->key[i] = leaf ->key[i];
			tnode->chi[i] = leaf ->chi[i];
			tnode->isLeaf =leaf ->isLeaf;
			tnode ->nkey =leaf ->nkey;
		}

		//insert new key
		tnode -> key[3] = key;
		tnode -> chi[3] = (NODE *) data;

		NODE *leafl;
		leafl =alloc_leaf(NULL);
		leafl ->isLeaf =tnode->isLeaf;

		//ptr to T
		//leafl ->chi[3] = tnode ->chi[4];

		//Set Pn of L’ to Pn of L

		//Set Pn of L to L’


		

		//clean up L
		for(int i=0; i<3; i++){
			leaf -> key[i] = 0;
			leaf -> chi[i] = NULL;
			leaf ->nkey =0;
			leaf ->isLeaf =true;
		}

		leaf ->chi[4] = leafl;
		

		//copy from T to L;
		leaf ->key[0]= tnode ->key[0];
		leaf ->key[1]= tnode ->key[1];
		leaf ->chi[0]= tnode ->chi[0];
		leaf ->chi[1]= tnode ->chi[1];

		//copy from T to L':
		leafl ->key[0]= tnode ->key[2];
		leafl ->key[1]= tnode ->key[3];
		leafl ->chi[0]= tnode ->chi[2];
		leafl ->chi[1]= (NODE *)data;

		//create a new root candidate R
		NODE *r;
		r = alloc_leaf(NULL);
		r->isLeaf =false;

		//set key
		r ->key[0] = leafl ->key[0];
		r ->chi[0] = leaf ->chi[1];
		r ->chi[1] = leafl ->chi[1];

		leaf ->parent =r;
		leafl ->parent = r;


		Root = r;
		return;







		/*NODE *leafa, *leafb;
		leafa = alloc_leaf(leaf);
		leafb = alloc_leaf(leaf);

		insert_in_leaf(leafa,leaf ->key[0],(DATA *) leaf ->chi[0]);
		insert_in_leaf(leafa,leaf ->key[1],(DATA *) leaf ->chi[1]);
		leafa =find_leaf(Root,key);
		
		insert_in_leaf(leafb,leaf ->key[2],(DATA *) leaf ->chi[2]);

		leaf -> key[0] =leaf ->key[2];
		leaf -> key[1] =NULL;
		leaf -> key[2] =NULL;

		for(int i=0; i<4; i++){
			leaf ->chi[i] = NULL;
		}
		
		leafa -> chi[3] = leafb;

		leaf ->chi[0] =leafa;
		leaf ->chi[1] =leafb;*/




		


		/*leafa -> key[0] = leaf -> key[0];
		leafa -> key[1] = leaf -> key[1];

		leafb -> key[0] = leaf -> key[2];
		leafb -> key[1] = key;

		
*/


		
	
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
