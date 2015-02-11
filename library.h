#include <sys/ioctl.h>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <fstream>

#define ALPHA 26
#define N 5

#define LINESIZE 128
#define BACKSPACE 0x7f

int getchar_silent();


///////////////////////////////////////////////////////////////////////////////


struct  fin
		{                         //leaf
			int frequency ;
		};


class Trie
{
	struct node
	{
		 node * children[ALPHA];   
		 node * parent;
		 fin * is_final;  //leaf
		 node *most_used[N]; 
	};

	node* _root;
	int   _size;

public:

	Trie ();
	~Trie();

	int size () ;
	
	void add (const char* );		//add to trie

	void print( node *,std::string &, std :: fstream & ) ;
	
	void set_most_frequent(node *,node *);
	
	std::string print_most_frequent(const char * , int &);
	
	void remove(node * );
	
	std::string  reverse_print( node * temp);
	
	node * get_root(){return _root;};
};






	
