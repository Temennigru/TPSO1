#ifndef DCCTHREAD_H_INCLUDED
#define DCCTHREAD_H_INCLUDED
#include <iostream>
#include <ucontext.h>
#include <list>

#define STACK_SIZE_PRINCIPAL 2048


using namespace std;


typedef struct thread
{
	const char *nome;
	ucontext_t thread;
} dccthread_t;

dccthread_t * dccthread_create(const char *, void (*)(int), int);
dccthread_t * dccthread_self(void);
const char * dccthread_name(dccthread_t *);
void dccthread_yield(void);

dccthread_t *gerente;
dccthread_t *principal;
list<dccthread_t> threads_esperando;
list<dccthread_t> threads_executando;

void dccthread_init(void (*func)(int), int param)
{
	char teste;
	ucontext_t auxiliar;

	printf("Inicializando Thread #Gerente\n");
	gerente = (dccthread_t*) malloc(sizeof(dccthread_t));
	getcontext(&gerente->thread);
	gerente->nome = "gerente";
	gerente->thread.uc_link = NULL;
	gerente->thread.uc_stack.ss_sp = (char*) malloc(STACK_SIZE_PRINCIPAL);
	if(gerente->thread.uc_stack.ss_sp == NULL)
	{
		printf("Não foi possível alocar pilha para a Thread #Gerente\n");
		exit(2);
	}
	gerente->thread.uc_stack.ss_size = STACK_SIZE_PRINCIPAL;
	gerente->thread.uc_stack.ss_flags = 0;


	printf("Inicializando Thread #Principal\n");
	principal = (dccthread_t*) malloc(sizeof(dccthread_t));
	getcontext(&principal->thread);
	principal->nome = "principal";
	principal->thread.uc_stack.ss_sp = (char*) malloc(STACK_SIZE_PRINCIPAL);
	if(principal->thread.uc_stack.ss_sp == NULL)
	{
		printf("Não foi possível alocar pilha para a Thread #Principal\n");
		exit(2);
	}
	principal->thread.uc_stack.ss_size = STACK_SIZE_PRINCIPAL;
	principal->thread.uc_stack.ss_flags = 0;
	threads_esperando.push_front(*principal);
	getcontext(&gerente->thread); // 605018
	principal->thread.uc_link = &gerente->thread; // Devolve o controle à thread gerente quando a thread principal encerrar a execução
	makecontext(&principal->thread, (void (*)(void))func, 1, param); // Faz com que a thread principal execute determinadaa função

	setcontext(&principal->thread);

	list<dccthread_t>::iterator it = threads_esperando.begin();

	getcontext(&gerente->thread);
	auxiliar = gerente->thread;
	cout << "passei"<<endl;
//	colocar aqui dentro do loop infinito o codigo que ira selecionar dentre as threads que estao esperando para executar
	if(it == threads_esperando.end())
	{
		it = threads_esperando.begin();
	}
	if(it == threads_esperando.begin())
	{
		it++;
	}

	threads_executando.push_back(*it); // coloca thread na lista de execução
	it = threads_esperando.erase(it) ;// deleta thread da lista de threads esperando e aponta para o pŕoximo elemento
	getcontext(&gerente->thread);
	setcontext(&threads_executando.begin()->thread); // executa a thread que está em primeiro lugar na lista de threads em execução;

	threads_esperando.push_back(*threads_executando.begin()); // volta com a thread executada para a lista de threads em espera
	threads_executando.pop_front(); // retira o elemento da thread executada


	//it++;
	cout << "Digite \"e\" para encerrar: ";
	cin >> teste;
	if(teste == 'e')
	{
		exit(EXIT_SUCCESS);
	}
	gerente->thread = auxiliar;
	setcontext(&gerente->thread);
}

dccthread_t * dccthread_create(const char *name, void (*func)(int), int param) // ta quase pronto
{
	dccthread_t *nova_thread = (dccthread_t*) malloc(sizeof(dccthread_t));

	getcontext(&nova_thread->thread);
	nova_thread->nome = name;
	nova_thread->thread.uc_stack.ss_sp = (char*) malloc(STACK_SIZE_PRINCIPAL);
	if(nova_thread->thread.uc_stack.ss_sp == NULL)
	{
		printf("Não foi possível alocar pilha para a Thread #Principal\n");
		exit(2);
	}
	nova_thread->thread.uc_stack.ss_size = STACK_SIZE_PRINCIPAL;
	nova_thread->thread.uc_stack.ss_flags = 0;
	nova_thread->thread.uc_link = &gerente->thread; // 605018
	makecontext(&nova_thread->thread, (void (*)(void))func, 1, param);
	threads_esperando.push_back(*nova_thread);

	cout << endl << "Imprimindo a Lista de threads esperando: " << endl;
	cout << "Tamanho da lista de threads esperando: " << threads_esperando.size() << endl;
	for(list<dccthread_t>::iterator it=threads_esperando.begin(); it != threads_esperando.end(); it++)
	{
		cout << "nome: " << it->nome << endl;
	}
//	int i;
//	cin >> i;
	return nova_thread;
}

void dccthread_yield(void) //
{
	dccthread_t executando;
	if(threads_executando.empty())
	{
		cout << "Lista de threads em execução vazia." << endl << "Retornando o controle para a Thread #Gerente" << endl;
		setcontext(&gerente->thread);
	}
	else
	{
		// extrair o primeiro elemento da lista de threads em execção
		cout << "Thread #" << executando.nome << " estava em execução." << endl;
		cout << "Colocando a Thread na lista de threads em espera" << endl;
		threads_esperando.push_back(threads_executando.front());
		cout << "Retirando a Thread da lista de threads em execução" << endl;
		threads_executando.pop_front();
		list<dccthread_t>::iterator it = threads_esperando.begin();
		it++;
		//getcontext(&gerente->thread);
		swapcontext(&it->thread, &gerente->thread);
	}
}

dccthread_t * dccthread_self(void)
{
	if(threads_executando.empty())
	{
		return NULL;
	}
	else
	{
		return &threads_executando.front();
	}
}

const char * dccthread_name(dccthread_t * thread)
{
	return thread->nome;
}

#endif // DCCTHREAD_H_INCLUDED
