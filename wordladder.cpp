/*CS106B Word Ladder.
e.g. code cade cate date data
Given start and end words, find a ladder in between.*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <iterator>
#include <queue>
#include <stack>

using namespace std;

string wordStart;
string wordFinish;
unordered_set<string> dict;
unsigned int neighbourI;
unsigned int neighbourJ;

void getDataFromConsole(){
    cout<<"Welcome to CS106B Word Ladder.\n"
          "Please give me two English words, and I will change the first \n"
          "into the second by changing one letter at a time."<<endl;
    string fileName="";
    ifstream fileStream;
    while (fileName ==""){
        cout<<"Dictionary file path?"<<endl;
        getline(cin,fileName);
        replace(fileName.begin(),fileName.end(),'\\','/');
        fileStream.open(fileName);
        if(fileStream.fail()){
            fileName="";
        }else{
            string current_word;
            while(!fileStream.fail()){
                getline(fileStream,current_word);
                dict.insert(current_word);
            }
            fileStream.close();
        }
    }

    wordStart=" ";
    while(dict.find(wordStart)==dict.end()){
        cout<<"Word #1 (or Enter to quit):"<<endl;
        getline(cin,wordStart);
        if(wordStart==""){
            exit(0);
        }else{
            transform(wordStart.begin(),wordStart.end(),wordStart.begin(),::tolower);
        }
    }
    wordFinish=" ";
    while(dict.find(wordFinish)==dict.end() || wordFinish.length()!=wordStart.length()){
        cout<<"Word #2 (or Enter to quit):"<<endl;
        getline(cin,wordFinish);
        if(wordFinish==""){
            exit(0);
        }else{
            transform(wordFinish.begin(),wordFinish.end(),wordFinish.begin(),::tolower);
        }
    }
}

void initialiseNeighbourIndices(){
    neighbourJ=0;
    neighbourI=0;
}

string findANeighbour(string currentWord){

    string neighbour=currentWord;

    if(neighbourI>=currentWord.length()){
        neighbour="-1";
        return neighbour;
    }

    while(neighbourI<currentWord.length()){

        string replaceLetter(1,char(neighbourJ+97));
        if(neighbourJ==0){
            neighbour=currentWord;
        }
        neighbour.replace(neighbourI,1,replaceLetter);

        if(neighbourJ<25){
            neighbourJ++;
        }else {
            neighbourI++;
            neighbourJ=0;
        }

        if (dict.find(neighbour)!=dict.end()){
            return neighbour;
        }else if (neighbourI==currentWord.length()-1 && neighbourJ==25){
            initialiseNeighbourIndices();
            neighbour="-1";
            return neighbour;
        }
    }
}


void printResult(stack<string> & currentStack){
    string currentWord;
    cout<<"A ladder between these words has been found: "<<endl;
    while(currentStack.size() >0){
        currentWord=currentStack.top();
        currentStack.pop();
        if(currentStack.size()==0){
            cout<<currentWord<<endl;
            cout<<endl;
        }else{
            cout<<currentWord<<" ";
        }
    }

}

void findLadder(){
    queue<stack<string>> wordsQue;
    stack<string> currentStack;
    stack<string> newStack;
    unordered_set<string> ladderSet;
    ladderSet.insert(wordStart);

    currentStack.push(wordStart);
    wordsQue.push(currentStack);
    string currentWord;
    string neighbour;
    bool found=0;

    while(wordsQue.size()>0){
        currentStack=wordsQue.front();
        wordsQue.pop();
        currentWord=currentStack.top();
        initialiseNeighbourIndices();
        while(found==0){
            do{
                neighbour=findANeighbour(currentWord);
                if(neighbour=="-1"){
                    break;
                }
            }while(ladderSet.find(neighbour)!=ladderSet.end());

            if (neighbour=="-1"){
                break;
            }else{
                if (neighbour==wordFinish){
                    found=1;

                    currentStack.push(neighbour);
                    printResult(currentStack);
                    return;
                }
                newStack=currentStack;
                newStack.push(neighbour);
                wordsQue.push(newStack);
                ladderSet.insert(neighbour);
            }
        }
    }

    if(wordsQue.size()==0){
        cout<<"There is no ladder between these words!"<<endl;
        return;
    }
}

int main()
{
    getDataFromConsole();
    findLadder();
    return 0;
}
