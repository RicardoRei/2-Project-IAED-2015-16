#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

int main()
{
	char command;
	hashtable = (Link*) malloc((sizeof(Link))*TABLESIZE);

	String msg;
	diferentHastags = 0;
	totalHashtags = 0;
	TreeNode maximus;

	while ((command = getchar()) != 'x') {  /* le o comando */
        switch (command) 
        {
        case 'a':
        	msg = readMSG(); /*le a mensagem*/
        	split(msg);	/*faz o tratamento dos dados da mensagem (parsing)*/
            break;
        case 's':
        	printf("%d %d\n", diferentHastags, totalHashtags); /* imprime apenas o valor das variaveis globais que nao sendo actualizadas */
        	getchar();
            break;
        case 'm':
        	if (raiz != NULL) /*se a raiz da arvore estiver a null nao existe nenhum hastag portanto o comando nao faz nada*/
        	{				/*caso contrario imprime o hashtage que apareceu mais vezes*/
        		maximus = left(raiz);
        		printf("%s %d\n", maximus->hashtag->tag, maximus->hashtag->counter);
        	}getchar();       
            break;
        case 'l':			/*Imprime a arvore In-Order*/
        	traverse(raiz);
        	getchar();
            break;
 		default:
            printf("ERRO: Comando desconhecido\n");
        }
    }

	return EXIT_SUCCESS;
}

/*@brief:
*	funcao de split sugerida pelo prof.
*/
void split(String line)
{
	int i, j, k, positionOnTable;
	char buffer[MAXLINESIZE+1];
	HASHTAG new = NULL;
	HASHTAG aux;

	for(i = 0, k = 0; line[i] != '\0'; i++, k++) 
	{
		buffer[k] = tolower(line[i]); 
		for(j = 0; j < NUMSEP; j++) 
		{
			if(line[i] == separators[j]) 
			{ 
				if(k != 0) 
				{
					buffer[k] = '\0';
					/*caso o buffer guarde uma palavra comecada # que seja maior que o tamanho minimo de um #*/
					if ((buffer[0] == '#') && (strlen(buffer) >= 2))
					{
						/*primeiro vamos fazer a funcao de hash para obter a posicao para aquela palavra no vector (hashtable)*/
						positionOnTable = hash(buffer, TABLESIZE) % TABLESIZE;
						/*depois procuramos pela existencia dessa palavra na lista ligada daquela posicao*/
						aux = search(hashtable[positionOnTable], buffer);
						if (aux != NULL) /*caso ja exista essa palavra*/
						{
							totalHashtags++; /*incrementar o contador global de hashtags*/
							raiz = delete(raiz, buffer, aux->counter); /*apagar da arvore */
							aux->counter++;	/*incrementar o novo valor para de seguida colocarmos na arvore na nova posicao*/
							new = aux;
						}
						else /*caso ainda nao exista um hashtag com aquela palavra*/
						{
							diferentHastags++; /*incrementamos na variavel global dos hashtags diferentes*/
							totalHashtags++; /*incrementamos a variavel global do total de hashtags*/
							new = createHashtag(buffer, 1);	/*criamos o novo hashtag */
							hashtable[positionOnTable] = insertBegin(new, hashtable[positionOnTable]); /*inserimos na hashtable*/
							/*depois insere-se na arvore*/
						}
						raiz = insertTree(raiz, new);
					}
				}
				k = -1; 
			}
		} 
	}
}

/*@brief:
*	funcao de hash de strings copiada dos slides.
*/
int hash(char *v, int M)
{
 	int h = 0, a = 127;
 	for (; *v != '\0'; v++)
 		h = (a*h + *v) % M;
 	return h;
} 

/*@brief:
*	a funcao insertBegin vai inserir um elemento no inicio de uma lista ligada, fazendo-a funcionar como uma pilha (FIFO).
*/
Link insertBegin(HASHTAG i, Link head)
{
	Link new = createNode(i);
	if (head == NULL)
		head = new;
	else
	{
		new->next = head;
		head = new;
	}
	return head;
}

/*@brief:
*	funcao que aloca memoria para um no da lista ligada e o coloca a apontar para o hashtag passado no argumento.
*/
Link createNode(HASHTAG i)
{
	Link new = (Link) malloc(sizeof(struct node));
	new->item = i;
	new->next = NULL;
	return new;
}

/*@brief:
*	funcao que percorre todos os nos de uma lista ligada a procura do no que esteja a guardar o Hashtag com o nome passado no argumento i.
*/
HASHTAG search(Link head, String i)
{

	Link current = head;
	while (current != NULL)
	{
		if (strcmp(current->item->tag, i) == 0)
		{
			return current->item;
		}
		current = current->next;
	}
	return NULL;
}


/*@brief:
*	Funcao que apaga o TreeNode da arvore com a tag e o contador passados nos argumentos.
*	Baseada na funcao delete dos slides com diferenca apenas nos criterios de escolha de ramos usada para encontrar o ramo que se pretende apagar.
*/
TreeNode delete(TreeNode head, String tag, int count)
{
	TreeNode aux;
	if (head == NULL)
		return head;

	/*criterio mais importante (numero do contador */
	else if (count > head->hashtag->counter)	
		head->l = delete(head->l, tag, count);

	else if (count < head->hashtag->counter)
		head->r = delete(head->r, tag, count);

	/*em caso de empate no primeiro criterio verifica a ordem alfabetica do hashtag */
	else if (strcmp(tag, head->hashtag->tag) < 0)
		head->l = delete(head->l, tag, count);

	else if (strcmp(tag, head->hashtag->tag) > 0)
		head->r = delete(head->r, tag, count);

	/*se nao entrou no ifs anteriores significa que ja encontramos so precisamos de ver se o no e interno ou nao...*/
	else
	{
		/*caso seja um no interno*/
		if (head->l != NULL && head->r != NULL)
		{
			aux = min(head->l);
			head->hashtag = aux->hashtag;
			head->l = delete(head->l, aux->hashtag->tag, aux->hashtag->counter);
		}
		/*caso nao seja um no interno*/
		else
		{
			aux = head;
			if (head->l == NULL && head->r == NULL)
				head = NULL;
			else if (head->l == NULL)
				head = head->r;
			else 
				head = head->l;
			free(aux);
		}
	}
	return head;
}

/*@brief:
*	imprime a informacao relevante daquele TreeNode.
*/
void visit(TreeNode h)
{
	printf("%s %d\n", h->hashtag->tag, h->hashtag->counter);
}

/*@brief:
*	Funcao que percorre a arvore e da esquerda para a direita vai imprimindo todos os TreeNodes.
*	In-Order transverse dos slides.
*/
void traverse(TreeNode h)
{
	if (h == NULL)
	 	return;

	traverse(h->l);
	visit(h);
	traverse(h->r);
}

/*@brief:
*	vai buscar o ramo mais direita do TreeNode passado como argumento (h). Oposto da funcao left...
*	Baseado no codigo dos slides.  
*/
TreeNode min(TreeNode h)
{
	while(h != NULL && h->r != NULL)
		h=h->r;
	return h;
}

/*@brief:
*	funcao que vai buscar o treeNode mais a esquerda, ou seja o que aponta para o Hashtag com mais referencias, ou com ordem alfabetica
*	menor dentro dos que foram mais vezes referenciados. 
*	Baseado no codigo dos slides.
*/
TreeNode left(TreeNode h)
{
	while(h != NULL && h->l != NULL)
		h=h->l;
	return h;
}

/*@brief:
*	insere o hashtag na arvore.
*	recebe: a raiz da arvore e o hashtag a ser inserido.
*	Baseado no codigo dos slides.
*/
TreeNode insertTree(TreeNode h, HASHTAG novo)
{	
	if (h == NULL)
	{
		h = (TreeNode) malloc(sizeof(struct treeNode));
		h->l = NULL;
		h->r = NULL;
		h->hashtag = novo;
		return h;
	}

	if (novo->counter > h->hashtag->counter)
		h->l = insertTree(h->l, novo);

	else if (novo->counter < h->hashtag->counter)
		h->r = insertTree(h->r, novo);

	else if (strcmp(novo->tag, h->hashtag->tag) < 0)
		h->l = insertTree(h->l, novo);

	else
		h->r = insertTree(h->r, novo);

	return h;
}

/*@brief:
*	cria a estrutura hashtage com a tag e o contador passados.
*/
HASHTAG createHashtag(String tag, int counter)
{
	HASHTAG hashtag = (HASHTAG) malloc(sizeof(struct h));
	hashtag->tag = (String) malloc(sizeof(char)*(strlen(tag)+1)); /* +1 para o \0*/
	strcpy(hashtag->tag, tag);
	hashtag->counter = counter;
	return hashtag;
}

/*@brief:
*	esta funcao le do standard input as mensagens.
*	return: devolve a string com a mensagem para dps ser processada na funcao split.
*/
String readMSG()
{
	char msg[MAXLINESIZE], c;
	int i = 0;
	String txt;

	getchar();
	while (((c = getchar()) != '\n') && (i < MAXLINESIZE)) 
	{
		msg[i] = c;
		i++; 
	}
	msg[i++] = ' ';	/* acrescenta um espaco para garantir que a funcao split guarda o # do fim da msg*/
	msg[i] = '\0';  
	txt = (String) malloc(sizeof(char)*(strlen(msg)+2));  /* aloca memoria para o tamanho apenas daquela msg +2 (contando com o \0 e o espaco extra*/

	strcpy(txt, msg);
	return txt;
}


