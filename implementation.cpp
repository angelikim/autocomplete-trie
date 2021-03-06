#include <iostream>
#include <cstdio>
#include <ctype.h>
#include <string>
#include <algorithm>



#include "library.h"



using namespace std;




#define LINESIZE 128
#define BACKSPACE 0x7f

int getchar_silent()
{
    int ch;
    struct termios oldt, newt;

    /* Retrieve old terminal settings */
    tcgetattr(STDIN_FILENO, &oldt);

    /* Disable canonical input mode, and input character echoing. */
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );

    /* Set new terminal settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    /* Read next character, and then switch to old terminal settings. */
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}


/////////////////////////////////////////////////////////////////////////////////////

Trie  ::  Trie ()   
	{                             
		_root=new node();
		_size=0;
		for(int i=0 ;i<ALPHA ; i++)
			_root->children[i]=NULL;
		_root->parent=NULL;
		_root->is_final=NULL;
		for(int i =0; i<N ; i ++ )
			_root->most_used[i]=NULL;
		
	}


Trie  ::  ~Trie()
 	{
		
		remove(_root);	
	
	}	
		


int  Trie  ::  size () 
	{ 
		return _size; 
	}




void Trie ::  remove (node * temp )
{
	if(temp == NULL)                  //remove  temp and all its children
		return;
	for(int i=0 ; i < ALPHA ; i++)
		if(temp->children[i]!= NULL )
			remove(temp->children[i]);
			
	if(temp->is_final!= NULL )
		delete temp->is_final;
	delete temp;
		
}


void  Trie  :: add (const char * c)
	{                                  //add word to trie
										//this is used whenever a word is typed ( new entry ,or not)
		node * temp=_root;				
		while(*c != '\0')
		{
			int counter = tolower(*c) -'a';
			if (temp->children[counter]==NULL )           //if child == NULL
			{												//it means that this prefix/word is typed for the first time 
				temp->children[counter]= new node ();		
				for(int i=0;i>ALPHA;i++)
					temp->children[counter]->children[i]=NULL;   //initializing
				temp->children[counter]->parent=temp;
				temp=temp->children[counter];                   //move pointer to current node
							
			}
			else
			{
				temp=temp->children[counter];				//this prefix/word is already made, so use this pointer
			}
			
			c++;
		}
	    if(temp->is_final==NULL)                     //that means it's a new entry
	    	{										 // so its default frequency is equal to 1
	    	temp->is_final= new fin;
	    	temp->is_final->frequency=1;
	    	}
	    else
	    {
	    	temp->is_final->frequency++;            //if not a new entry , icrease its frequency by one.
	    }
	    set_most_frequent(temp , temp);              //set new "top N  most used" now that one frequency is added/changed
	    
	 
		
	}


void Trie :: print( node * temp ,string & buf , fstream &out) 
{
	
	if(temp == NULL)          
		return;
	if(temp->is_final != NULL)
		out<<buf<<endl;	    //if end of word
	for(int i=0 ; i <ALPHA ; i++ )
	{	
		if(temp->children[i]!=NULL)
		{
			buf+=(char) i+97;     //97 : ASCII code for 'a'
			print(temp->children[i],buf, out); 
			buf.resize(buf.size() -1);        //pops the last character from string "buf" 
								  //so that the prefix is still available 
								  //to print if needed
			
		}
	}
	
	
}


void Trie :: set_most_frequent( node * temp , node * leaf)
{
	  	int tmp=0;
		if(temp->parent == NULL)
			return;
		set_most_frequent(temp->parent,leaf);
     
	    bool flag=false;
	    for(int i=0;i<N ; i++)             //if parent was just created or has less than N children
	    {		
	    	if(temp->most_used[i]==leaf)
	    		return;							//this word is added to the top-n most frequent 
	    	if(temp->most_used[i]==NULL)
	    	{	
	    		temp->most_used[i]=leaf;
	    		flag=true;
	    		break;
	    		
	   		}
	   		
		}
		if(!flag)                           //else if parent has a full set of most frequent words
		{														
			int min=temp->most_used[0]->is_final->frequency;
			for(int i=1 ; i <N ; i++)
			{
				if(  temp->most_used[i]->is_final->frequency  <  min )
				{
					min=temp->most_used[i]->is_final->frequency;				//choose the one with the minimum frequency.
					tmp=i;					
					
				}
			}
			if(  leaf->is_final->frequency  >  min  ) 
			{ 										// If it's less than the one the user wrote swap them		
				temp->most_used[tmp]=leaf;
			}
		}
	


}


string  Trie :: print_most_frequent( const char*  c , int & changed)        
{
	string buffer[N];
	int ch,counter=0;
	node * temp=get_root();	
	int index,size=0;
	cerr <<endl;
	while( isalpha(*c))
	{	
		index=tolower(*c)-'a';
		if(temp->children[index] != NULL)
			temp=temp->children[index];
		else
		{
			buffer[0].clear();
			buffer[0]= buffer[0] +"";
			return buffer[0];            // New prefix-no words exist		
		}
		c++;
		size++;
	}
	for(index = 0; index <N ; index ++)
		if( temp->most_used[index] != NULL )
		{
			buffer[index]=reverse_print(temp->most_used[index]);     //count most frequent
			counter++;
		}
	if(counter == 1 )  	//only one choice available ,return that
	{	
		buffer[counter-1].erase(0,size);
		return buffer[counter-1];
		
		
	}
	sort(begin(buffer),end(buffer));       //if more than one choice is available ,sort alphabetically
	for(index = 0; index <N ; index ++)
		if(!buffer[index].empty() )
		{	
		cerr <<"(" << index + 1 << ")"<<buffer[index]<<"	"; //print to stderr
		
		}
	cerr<<endl;	
	cerr<< " *select word by typing its counterpart number or keep typing"<<endl;
	ch =  getchar_silent(); 
	int error=0;
	while(true)
	{
	if(!isdigit(ch))            //if not digit return( user propably wants to continue writing)
	{	
		
		buffer[0].clear();
		buffer[0]= buffer[0] +(char) ch ;
		changed=1;
		return  buffer[0];
	}
	else if( ( (ch - 48) > counter ) || ( (ch-48) <= 0 ) )       //if digit but greater than counter( mentioned above ) or less than 1
	{	
		error ++;
		if(error==1)
			cerr<<"**please choose one of the numbers listed above or continue typing "<<endl;
		ch =  getchar_silent(); 
		
	}
	else								//if right input return suffix
	{	
		ch=ch-48;
		buffer[ch-1].erase(0,size);
		return buffer[ch-1];
	}
	//return (string)"";
	}
}




string   Trie :: reverse_print( node * temp )
{
	string buf="";                   //when given a node / leaf
	while(temp!= _root)				 //use buffer to save values and then print backwards
	{	
		for(int i=0 ; i<ALPHA ; i ++ )
		{	
			if(temp->parent->children[i]==temp)
			{
				buf+=(char) i+97;
				break;
			}
		}
		
		temp=temp->parent;
	}
	buf = string ( buf.rbegin(), buf.rend() );  //reversing string
	return buf;
}








