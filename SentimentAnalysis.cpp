/*
	ACM Research Fall 2021 Sentiment Analysis Coding Challenge
	By: Mark Friant
*/

#include <iostream>
#include <string>
#include <fstream>
#include <forward_list>
#include <C:\Users\markl\Documents\Comp Sci\C++\All\Programs\Code Functions\FileEditor.cpp> // importing file editor class

using namespace std;

/*
	note - I refer to ".", "?", and "!" as "sentence enders"
	because they are at the end of sentences and distinguish one sentence from the next
*/

//////////////////////////////////////////////////////////////////////
//variables
//////////////////////////////////////////////////////////////////////

//address of input file in local directory
FileEditor file("C:\\Users\\markl\\Documents\\College\\SentimentAnalysis\\SentimentAnalysis\\input.txt");

//list of all the words in the file
//contains ".", "?", and "!" as separate elements for easier sentence analysis
forward_list<string> words;

//number of words in the file, including sentence enders
int wordCount = 0;

//key list (of scores), not including sentence enders
//each element is the score that corresponds with the word that has the same number
int wordScores[678];

//list of sentence scores
forward_list<int> sentenceScores;

//number of sentences in the file
int sentenceCount = 0;

//iterates through list "words"
forward_list<string>::iterator current;

//////////////////////////////////////////////////////////////////////
//functions
//////////////////////////////////////////////////////////////////////

//takes character input and outputs whether it is a lowercase letter from the alphabet
bool isLetter(char letter)
{
	if (letter >= 'a' && letter <= 'z')
		return true;
	return false;
}

//makes everything lowercase
//this makes it easier to distinguish words
void lowercase()
{
	file.resetIt();
	//switching uppercase to lowercase
	//going through each line
	for (int lineNum = 0; lineNum < file.lines(); lineNum++)
	{
		//going through each character in a line
		for (int charNum = 0; charNum < file.getIt().length(); charNum++)
		{
			if (file.getIt().at(charNum) >= 'A' && file.getIt().at(charNum) <= 'Z')
			{
				char lowercaseReplacement = file.getIt().at(charNum) + 32;

				string newLine =
					file.getIt().substr(0, charNum)
					+ lowercaseReplacement
					+ file.getIt().substr(charNum + 1, file.getIt().length() - charNum - 1);

				//replacing uppercase letter
				file.replace(newLine, lineNum);

				file.resetIt();
				for (int i = 0; i < lineNum; i++) file.iter();
			}
		}
		file.iter();
	}
}

/////////////////////////////////////////////////

//extracts words from the file and puts each one into the "words" list as its own element
//puts sentence enders into the list as well (also as their own separate elements)
void wordList()
{
	file.resetIt();
	//loop is going through each line in the file
	for (int lineNum = 0; lineNum < file.lines(); lineNum++)
	{
		string word = "";
		//going through each character in a line
		for (int charNum = 0; charNum < file.getIt().length(); charNum++)
		{
			//adding a character onto the word being worked on
			if (file.getIt().at(charNum) != ' ')
			{
				word = word + file.getIt().at(charNum);

				//last word of the line (no spaces after it)
				if (charNum == file.getIt().length() - 1)
				{
					words.push_front(word); // adding to list
					wordCount++;
					word = ""; // resetting the buffer word
				}
			}
			else
			{
				//adding word to list
				if (word != "")
				{
					words.push_front(word); // adding to list
					wordCount++;
					word = ""; // resetting buffer word
				}
			}
		}
		file.iter(); // iterating by one line
	}
	words.reverse(); // since the words are being added at the front
	
	////////////////////////////////////////
	//trimming non-letter characters

	current = words.begin();

	//going through words
	for (int wordNum = 0; wordNum < 678; wordNum++)
	{
		//making a separate buffer variable so it's easier to work with
		string currentWord = *current;
		//skips sentence enders and uses this variable to keep track of when it needs to skip
		bool sentenceEnder = false;

		//chopping extra characters off front
		while (!isLetter(currentWord.at(0)))
		{
			currentWord = currentWord.substr(1);
			*current = currentWord;
		}

		//chopping extra characters off back
		while ( !isLetter( currentWord.at(currentWord.length() - 1) ) )
		{
			//making the sentence ender a separate word for easier processing later
			if (currentWord.at(currentWord.length() - 1) == '.')
			{
				//adding separate word for sentence ender
				sentenceEnder = true;
				words.insert_after(current, ".");
				wordCount++;
			}
			//sentence ender
			else if (currentWord.at(currentWord.length() - 1) == '!')
			{
				//adding separate word for sentence ender
				sentenceEnder = true;
				words.insert_after(current, "!");
				wordCount++;
			}
			//sentence ender
			else if (currentWord.at(currentWord.length() - 1) == '?')
			{
				//adding separate word for sentence ender
				sentenceEnder = true;
				words.insert_after(current, "?");
				wordCount++;
			}
			//chopping character
			currentWord = currentWord.substr(0, currentWord.length() - 1);
			*current = currentWord;
		}

		//moving on to next word
		current++;
		//skipping sentence ender
		if (sentenceEnder)
		{
			current++;
			sentenceEnder = false;
		}

	}
}

//creates a list of all the scores that correspond with each word in the "words" list
//magnitude is 0 to 3 (neutral, slight, moderate, and high)
//negative or positive sign corresponds with negative or positive emotion
//score of 100 signifies a rule for doubling effect of next word
void keyList()
{
	//organized by line in file
	//does not include sentence enders
	int key[678] =
	{
		-1, 0, 0, -1, -1, 100, 0, -1, 0, 0, 0, 0, // 1
		0, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 1,
		0, 0, 0, 0, 0, 0, 0, -3, -1, 0, 0, 0, -1,
		0, -2, -2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, // 5
		0, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
		0, 0, 0, -1, 2, 0, 0, 0, -2, 0, 1, 0, 0, -2,
		0, 0, 0, 1, 0, 0, 0, 0, -2, -3,
		-1, 0, -1, 0, 0, 0, 0, -2, 0,
		-1, 0, -1, // 10
		0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 1,
		-2, 0, -1, 0, -1, 0, 0, 0, -1, 0, 2, 2, 0, 0, 0,
		0, -1, 0, 0, 0, 0, 0, -2, 0, 0, 1, 0, 0,
		1, 0, 0, -1, 0, 0, 0, 1, 0, 0, 1, -2, 0,
		0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 1, 0, // 15
		2, 0, 2, 0, -2, 0, 100, -2, 0, 0, -3,
		-1, -2, 0, 0, 0, 0, 0, -3, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 1, 0, -1, 0, 0, 0, -3,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 2, 0, // 20
		0, -1, 0, -2, 0, 0, 0, 0, 1, 0, -1, 0, 0, 0,
		0, 1, 0, 100, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 0,

		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, // 25
		0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
		0, 100, 2, 0, 0, 2, 1, 0, 2, 0, 2, 0, -1,
		0, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 1, 1,
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, -1, 0, 0, 100, 1, 0, 0, 0, 0, // 30
		0, 0, 2, 0, 0, 0, 1, 0, 100, 1, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 2, 3, 0, 0, 0, 0, 1, 0,
		1, 1, 0, 1, 0, 0, 0, -1, 0, 0, -1,
		0, 0, 0, 1, 0, 0, -2, 1, 0, 1, 1, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 35
		0, 0, 0, 1, 0, 0, 0, 1, 0, 2, 0, 0,
		1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 1, 0,
		2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 1, 0, 1,
		0, 0, 0, 100, 1, 0, -1, 0, 0, 0, -1, 0, 0, -1,
		0, 0, 0, 0, 0, 1, 0, -1, 1, // 40
		0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 2, 0,
		1, 0, 0, 0, 0, 1, -1, 2, 0, 1, 0, 2, 0, 2,
		0, 0, 1, 0, 0, 0, 0, 2, 0, 1, 0, 0, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0,
		0, 1, 0, 1, 0, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, // 45
		0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -2, 0, 0, 0, -1, 0, 0,
		0, 2, 0, -2, 1, 1, 0, -1, 0, 0, 0, 0, 0, 0,
		0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, -1, 0, 0, 0,
		0, 0, 0, 100, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0,
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 1, // 50
		0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		2, 0, 0, 0, 100, -2, 0, -1, 0, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 0, 0, 0
	};

	//printing words
	current = words.begin();
	for (int wordNum = 0; wordNum < wordCount; wordNum++)
	{
		//cout << "Word: " + to_string(wordNum + 1) + ": " + *current << endl;
		current++;
	}

	//printing out word scores
	for (int wordNum = 0; wordNum < 678; wordNum++)
	{
		wordScores[wordNum] = key[wordNum];
		//cout << "Word " + to_string(wordNum + 1) + " Score: " + to_string(wordScores[wordNum]) << endl;
	}
}

//combining word scores into sentence scores
//exclamation points double the magnitude of a sentence's score
void sentenceScoreList()
{
	current = words.begin();
	//keeps track of position in the words list
	int wordNum = 0;
	//buffer for sentence score
	int currentSentenceScore = 0;
	//used for keeping track of special rule for adverbs doubling next word
	bool adverb = false;
	//how many sentences there are thus far
	sentenceCount = 0;
	
	//going through each word in the list
	while (wordNum < wordCount)
	{
		//checking if the element is a sentence ender
		//if so, it will stop this sentence and start a new one
		if (*current == "." || *current == "?")
		{
			sentenceScores.push_front(currentSentenceScore); // adding the score to list
			currentSentenceScore = 0; // resetting score
			sentenceCount++;
		}
		//same as above block except for doubling score
		else if (*current == "!")
		{
			currentSentenceScore *= 2; // doubling score of sentence since exclamation point adds emotion
			sentenceScores.push_front(currentSentenceScore);
			currentSentenceScore = 0;
			sentenceCount++;
		}
		//special adverb rule
		else if (wordScores[wordNum - sentenceCount] == 100)
		{
			adverb = true;
		}
		else
		{
			//doubling next word
			if (adverb)
			{
				currentSentenceScore += 2*wordScores[wordNum - sentenceCount];
				adverb = false; // resetting flag variable
			}
			//normal word, just adding its score to the sentence
			else
			{
				currentSentenceScore += wordScores[wordNum - sentenceCount];
			}
		}

		wordNum++;
		current++;
	}
	sentenceScores.reverse();
}

//////////////////////////////////////////////////////////////////////
//main function
//////////////////////////////////////////////////////////////////////

int main()
{
	lowercase();
	wordList();
	keyList();
	sentenceScoreList();

	forward_list<int>::iterator sentenceIter = sentenceScores.begin();

	//score for whole text
	int totalScore = 0;
	//percentage of max possible score (-100% to +100%)
	//makes it possible to compare the text with others of different sizes
	double sentiment = 0;

	//going through each sentence
	for (int sentenceNum = 0; sentenceNum < sentenceCount; sentenceNum++)
	{
		totalScore += *sentenceIter; // adding sentence score to overall score
		cout << "Sentence " + to_string(sentenceNum + 1) + " Score: " + to_string(*sentenceIter) << endl; // printing sentence score
		sentenceIter++;
	}

	sentiment = ( (double)totalScore / (678.0 * 3.0) ) * 100.0; // calculating sentiment percentage

	//printing
	cout << "Total Score: " + to_string(totalScore) << endl;
	cout << "Sentiment: " + to_string(sentiment) + "%" << endl;

	return 0;
}