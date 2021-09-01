#include <iostream>
#include <string>
#include <fstream>
#include <forward_list>

using namespace std;

//created August 2020
//MUST USE SAVE FUNCTION IN ORDER TO UPDATE THE ACTUAL FILE IN MEMORY

class FileEditor
{
private:
	string address = ""; // address of file in directory
	forward_list<string> file; // list that contains each line of the file as an element
	forward_list<string>::iterator active; // iterator for above file list
	int size = 0; // number of lines in file
public:

	//constructor

	//associates the object with a file address and saves the file's text in local memory
	FileEditor(string add)
	{
		address = add;

	//obtaining file

		//file input stream
		ifstream fin(address);

		//variables
		char cstr[1000];

		//retrieving lines
		while (!fin.eof()) //checking if it's "not the end of the file"
		{
			fin.getline(cstr, 1000); //obtaining line
			string s(cstr);
			file.push_front(s);
			size++;
		}
		file.reverse();

		//closing stream
		fin.close();

		if (size == 1 && file.front().compare("") == 0)
		{
			file.clear();
			size = 0;
		}

		active = file.begin();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//iterator

	//resets iterator to the beginning
	void resetIt()
	{
		active = file.begin();
	}

	//returns an iterator of the list
	forward_list<string>::iterator getIterator()
	{
		return active;
	}

	//iterates the iterator
	void iter()
	{
		active++;
	}

	//gets string at iterator position
	string getIt()
	{
		return *active;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//input (reading)

	//returns string list of the entire file
	forward_list<string> getFile()
	{
		return file;
	}

	//returns given line string
	string getLine(int line)
	{
		if (line == 0)
			return file.front();
		else
		{
			forward_list<string>::iterator it = file.begin();
			for (int i = 0; i < line; i++)
				it++;
			return *it;
		}
	}

	//returns how many lines are in the file
	int lines()
	{
		return size;
	}

	//returns number of nth line equal to string s or returns -1
	int find(string s, int n)
	{
		int flags = 0;
		forward_list<string>::iterator it = file.begin();
		for (int i = 0; i < size; i++)
		{
			if (s.compare(*it) == 0)
				flags++;
			if (flags == n)
				return i;
			it++;
		}
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//output (writing)

	//sets the list to another list
	void set(forward_list<string> l)
	{
		file.assign(l.begin(), l.end());
		size = 0;
		for (forward_list<string>::iterator it = file.begin(); it != file.end(); it++)
			size++;
	}

	//reverses order of elements
	void reverse()
	{
		file.reverse();
	}

	//adds a line at a specified position
	void add(string s, int line)
	{
		if (line == 0)
			file.push_front(s);
		else
		{
			forward_list<string>::iterator it = file.begin();
			for (int i = 0; i < line - 1; i++)
				it++;
			file.insert_after(it, s);
		}
		size++;
	}

	//replace line at given position
	void replace(string s, int line)
	{
		if (line == 0)
		{
			file.pop_front();
			file.push_front(s);
		}
		else
		{
			forward_list<string>::iterator it = file.begin();
			for (int i = 0; i < line - 1; i++)
				it++;
			file.erase_after(it);
			file.insert_after(it, s);
		}
	}

	//remove line at given position
	void remove(int line)
	{
		if (line == 0)
			file.pop_front();
		else
		{
			forward_list<string>::iterator it = file.begin();
			for (int i = 0; i < line - 1; i++)
				it++;
			file.erase_after(it);
		}
		size--;
	}

	//clears all text in the file
	void clear()
	{
		file.clear();
		size = 0;
	}

	//writes string to the actual file in memory
	void save()
	{
		ofstream fout(address);
		forward_list<string>::iterator it = file.begin();

		for (int i = 0; i < size; i++)
		{
			fout << *it;
			if(i + 1 < size)
				fout << endl;
			it++;
		}

		fout.close();
	}
};