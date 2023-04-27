#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <string>
#include <map>
#include <math.h> 
using namespace std;
using namespace std::chrono;



//keywords can be added to a node to record data of a given word 
struct KeyWord
{
    string word = "";

    double count = 0;
    double views = 0;
    double likes = 0;
    double dislikes = 0;
    double comments = 0;

    KeyWord(string i) : word(i) {}
    KeyWord() : count(1) {}
};

struct Node 
{
    
    map<char, Node*> children;
    bool endWord = false;
    KeyWord* k;

};


//removes special characters from title fields
string truncate(string title)
{
    for (int i = 0; i < title.length(); i++)
    {
        if (title[i] != '0' && title[i] != '1' && title[i] != '2' && title[i] != '3' && title[i] != '4' && title[i] != '5' && title[i] != '6' && title[i] != '7' && title[i] != '8' && title[i] != '9' && title[i] != '-' && title[i] != ' ' && title[i] < 'A' || title[i] > 'Z' && title[i] < 'a' || title[i] > 'z')
        {
            title.erase(i, 1);
            i--;
        }
    }
    return title;
}

//initializes empty node
Node* createNode() 
{
    Node* temp = new Node;
    temp->endWord = false;
    return temp;
}

//inserts the given word into the Trie and also adds a KeyWord object to that node
void insert(Node* root, string word)
{
    Node* temp = root;

    for (char c : word)
    {
        if (temp->children.find(c) == temp->children.end())
        {
            (temp->children)[c] = createNode();
        }

        temp = temp->children[c];
    }
    temp->endWord = true;
    temp->k = new KeyWord(word);
}

//searches for a given word and returns that node, otherwise, returns root
Node* search(Node* root, string word)
{
    Node* temp = root;

    for (char c : word)
    {
        if (temp->children.find(c) == temp->children.end())
        {
            return root;
        }

        temp = temp->children[c];
    }

    if (temp->endWord == true)
        return temp;
    else
        return root;

}

int main()
{
    ifstream file;

    cout << "Project 3 - YouTube Trending Words Finder" << endl << endl;

    //user chooses dataset by entering number

    cout << "Choose dataset:" << endl;
    cout << "   1. United States Trending Videos" << endl;
    cout << "   2. United Kingdom Trending Videos" << endl;
    cout << "   3. Canada Trending Videos" << endl;
    cout << "   4. All of the above" << endl;

    string choice;
    cin >> choice;
    bool all = false;

    if (choice == "1") file.open("USvideos.csv");
    if (choice == "2") file.open("GBvideos.csv");
    if (choice == "3") file.open("CAvideos.csv");
    if (choice == "4")
    {
        file.open("USvideos.csv");
        all = true;
    }
    
    //user chooses which data structure to sort words into
   
    cout << "Choose data structure:" << endl;
    cout << "   1. Ordered Map" << endl;
    cout << "   2. Prefix Tree (Trie)" << endl;

    cin >> choice;

    cout << "Processing videos... please wait!" << endl << endl;

    string line;
    string title;
    stringstream ss;

    double v = 0;
    double l = 0;
    double d = 0;
    double c = 0;

    getline(file, line);
    
    cout << fixed;
    cout << setprecision(0);
    int wordCount = 0;

    //reads in file and creates a map of words

    if (choice == "1")
    {
        auto start = high_resolution_clock::now();

        map<string, KeyWord*> words;

        while (getline(file, line))
        {
          
            //iterates through the spreadsheet and records the titles, views, likes, dislikes, and comments of every video
            ss << line;
            getline(ss, line, ',');
            getline(ss, line, ',');
            getline(ss, title, '"');
            getline(ss, title, '"');
            title = truncate(title);
            getline(ss, line, '"');
            getline(ss, line, '"');
            getline(ss, line, ',');
            getline(ss, line, ',');
            getline(ss, line, ',');
            getline(ss, line, ',');
            getline(ss, line, ',');

            v = stod(line);
            getline(ss, line, ',');
            l = stod(line);
            getline(ss, line, ',');
            d = stod(line);
            getline(ss, line, ',');
            c = stod(line);


            ss.str("");
            ss.clear();
            ss << title;
            string w;

            while (getline(ss, w, ' '))
            {
                wordCount++;
                KeyWord* kw = new KeyWord(w);
                
                if (words.find(w) == words.end())
                {
                    words[w] = kw;

                }

                words[w]->count++;
                words[w]->likes += l;
                words[w]->dislikes += d;
                words[w]->comments += c;
                words[w]->views += v;


            }


            ss.str("");
            ss.clear();


        }



        bool program = true;

        //searches for the requested keyword and gives its stats

        cout << wordCount << " keywords were processed in total." << endl;

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<seconds>(stop - start);
        cout << "Time taken to process words in an Ordered Map:" << " ";
        cout << duration.count() << " seconds" << endl << endl;

        while (program)
        {
            

            cout << "Enter keyword to display data: ";
            string word;
            cin >> word;
            auto start2 = high_resolution_clock::now();
          

            cout << endl;
            if (words.find(word) == words.end())
            {
                cout << "Keyword not found!" << endl;
            }
            else
            {
                cout << "   Times Used: " << words[word]->count << endl;
                cout << "   Average Views: " << words[word]->views / words[word]->count << endl;
                cout << "   Average Likes: " << words[word]->likes / words[word]->count << endl;
                cout << "   Average Dislikes: " << words[word]->dislikes / words[word]->count << endl;
                cout << "   Average Comments: " << words[word]->comments / words[word]->count << endl;

            }

            cout << endl << "Time taken to search:" << " ";
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<microseconds>(stop2 - start2);
            cout << duration2.count() << " microseconds" << endl << endl;
            cout << endl;



        }






    }


    //reads in file and creates a prefix tree of words

    if (choice == "2")
    {
        auto start = high_resolution_clock::now();

        Node* root = createNode();


        while (getline(file, line))
        {
           

            //iterates through the spreadsheet and records the titles, views, likes, dislikes, and comments of every video
            ss << line;
            getline(ss, line, ',');
            getline(ss, line, ',');
            getline(ss, title, '"');
            getline(ss, title, '"');
            title = truncate(title);
            getline(ss, line, '"');
            getline(ss, line, '"');
            getline(ss, line, ',');
            getline(ss, line, ',');
            getline(ss, line, ',');
            getline(ss, line, ',');
            getline(ss, line, ',');
      
            v = stod(line);
            getline(ss, line, ',');
            l = stod(line);
            getline(ss, line, ',');
            d = stod(line);
            getline(ss, line, ',');
            c = stod(line);
       
            
           
            ss.str("");
            ss.clear();
            ss << title;
            string w;

            while (getline(ss, w, ' '))
            {
                wordCount++;
                Node* test = search(root, w);
                if (test == root)
                {
                    insert(root, w);
                }
               

                test = search(root, w);
                test->k->count++;
                test->k->likes += l;
                test->k->dislikes += d;
                test->k->comments += c;
                test->k->views += v;

                
            }
            

            ss.str("");
            ss.clear();
            

        }


        
        bool program = true;

        //searches for the requested keyword and gives its stats

        cout << wordCount << " keywords were processed in total." << endl;

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<seconds>(stop - start);
        cout << "Time taken to process words in a Prefix Tree: ";
        cout << duration.count() << " seconds" << endl << endl;

        while (program)
        {
           
                cout << "Enter keyword to display data: ";
                string word;
                cin >> word;


                    auto start2 = high_resolution_clock::now();
                    Node* n = search(root, word);
                    cout << endl;
                    
                    if (n == root)
                        {

                        cout << "Keyword not found!" << endl;
                        }
                    else
                    {
                        cout << "   Times Used: " << n->k->count << endl;
                        cout << "   Average Views: " << n->k->views / n->k->count << endl;
                        cout << "   Average Likes: " << n->k->likes / n->k->count << endl;
                        cout << "   Average Dislikes: " << n->k->dislikes / n->k->count << endl;
                        cout << "   Average Comments: " << n->k->comments / n->k->count << endl;

                    }
                    cout << endl << "Time taken to search: ";
                    auto stop2 = high_resolution_clock::now();
                    auto duration2 = duration_cast<microseconds>(stop2 - start2);
                    cout << duration2.count() << " microseconds" << endl << endl;

                    cout << endl;

                

         }



     }


 }


