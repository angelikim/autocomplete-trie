#include <iostream>
#include <string>
#include <fstream>



#include "library.h"



using namespace std;

int main(int argc , char ** argv)
{
	string buffer,line, text;
	Trie trie;
 	
	fstream fs;
	
	if(argc == 1)                     //no arguments , open default dictionary
	{
		string str;
		str = getenv("HOME");
		fs.open(str+"/.dict",ios::in );
		//fs.close();
	}
	else if(argc== 3)                     // follow path to dictionary
	{	
		fs.open(argv[2], ios::in );
		
	}
	else
	{
		cout<< " Wrong parameters , please try again " <<endl;
		return 1;	
	}
	
	if(!fs.is_open())
	{
		cout<<"Unaible to open file"<<endl;
		return 1;
	}
	

	
	 while ( getline (fs,line) )          //initialize trie
    {
      trie.add(line.c_str());
    }
    
    fs.close();
    
	 int ch;
     
	cout << "Press Ctr-d for exit ..."<<endl;
    while ((ch = getchar_silent()) != EOF)
    {
         if (isalpha(ch) )    //print letters
        {
            buffer =buffer + (char) ch;
            text=text + (char) ch;
            cout <<(char) ch;
            
        }
		else if ( ispunct(ch) || ch == ' ' || ch == '\n')       //end of word, add word (if exists) to trie
		{
			if(!buffer.empty())
			{
				trie.add(buffer.c_str());
				buffer.clear();
			}
			text=text + (char) ch;
			cout <<(char) ch;
		}
		else if ( isdigit(ch) )                            //print numbers 
		{
			buffer.clear();									//flush buffer - no numbers can be added to trie
			text=text + (char) ch;
			cout <<(char) ch;
		}
		else if (ch == '\t' && (!buffer.empty()) )           //if any (well-formed) prefix is written 
        {													//and user presses TAB
        	string text1;
        	int changed=0;
			text1=trie.print_most_frequent(buffer.c_str(),changed);         //display options and choose desired suffix
			if(isupper(buffer.back()))
				for( unsigned int i=0 ; i < text1.size() ; i ++ )
					text1[i]=toupper(text1[i]);
			buffer=buffer+text1;
			if(changed==0)                                             //user chose suffix
			{
				trie.add(buffer.c_str());                             
				text=text+text1;
				cout<<text;	
				buffer.clear();						//fluch buffer for new word
			}
			else
			{														//user continued typing instead of choosing suffix
				text=text+text1;									
				cout<<text;											
			}
											
        }
        else if (ch == BACKSPACE)
        {
        	printf("\b \b");
  			if(!text.empty())                     //deleting last character from word
  				text.resize(text.size()-1);
            if(!buffer.empty())
                buffer.resize(buffer.size()-1);
        }
		else if (ch == VEOF)
		{
			
			if(argc == 1)
			{
				string str;
	 			str = getenv("HOME");
				fs.open(str+"/.dict",ios::out |ios :: trunc );
			}
			else if(argc== 3)
			{	
				fs.open(argv[2],ios::out |ios :: trunc );
			}
			
			if(!fs.is_open())
			{
				cout<<"Unaible to open file, trie will not be saved "<<endl;
				return 1;
			}
			cout<<endl<<"Saving changes....."<<endl;
			trie.print(trie.get_root() , buffer,fs);           //save trie to file
			fs.close();
     		cout<<"Bye ;)"<<endl;
     		return 0;
     		
		}
      
        
    }
	
 return 1;
}
