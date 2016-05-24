
#define MAXLINESIZE 140
#define NUMSEP 11
/*tamanho da hashtable (numero primo para evitar colisoes)*/
#define TABLESIZE 5003

static const char separators[] = {' ','\t',',',';','.','?','!','"','\n',':','\0'};

int diferentHastags;
int totalHashtags;

/*definicao de um tipo que representa um conjunto de caracteres ou seja uma palavra.*/
typedef char* String;

/*estrutura hashtag que guarda um texto (campo tag) e um contador de vezes que foi usado (campo counter)*/
typedef struct h
{
	String tag;
	int counter;
}*HASHTAG;

/*estrutura que representa um no numa arvore com um ponteiro para uma estrutura do tipo hashtag (campo hashtage) 
  e dois ponteiros para uma estrutura do mesmo tipo que vao representar o no da direita e da esquerda (campos l e r)*/
typedef struct treeNode
{
	HASHTAG hashtag;
 	struct treeNode *l;
 	struct treeNode *r;
}*TreeNode;

/*estrutura no que vai ter um ponteiro para um hashtag (campo item) e um ponteiro para uma estrutura do mesmo tipo (campo next)*/
typedef struct node
{
	HASHTAG item;
	struct node *next;
}*Link;

/*funcoes especificas do projecto*/
String readMSG();
void split(String line);
HASHTAG createHashtag(String tag, int counter);
void deleteHashTag(HASHTAG h);

/*funcoes para trabalhar com a arvore*/
void traverse(TreeNode h);
void visit(TreeNode h);
TreeNode min(TreeNode h);
TreeNode left(TreeNode h);
TreeNode delete(TreeNode head, String tag, int count);
TreeNode insertTree(TreeNode h, HASHTAG novo);

/*funcoes para a hashtable*/
int hash(char *v, int M);
/*funcoes para manipular listas na hashtable*/
HASHTAG search(Link head, String i);
Link insertBegin(HASHTAG i, Link head);
void show(Link head);
Link createNode(HASHTAG i);


/*VARIAVEIS GLOBAIS PARA A RAIZ DA ARVORE E PARA A HASHTABLE*/
TreeNode raiz = NULL;
Link* hashtable = NULL;