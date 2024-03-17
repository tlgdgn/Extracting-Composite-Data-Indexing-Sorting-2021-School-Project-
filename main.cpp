#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include <chrono>
#include <ctime>
using namespace std;
template<class T1, class T2>
class list {

private:
    struct Node {
        Node(T1 i, T2 j) : first(i), second(j) {}
        Node() : first(), second() {}
        ~Node() { delete next; }

        T1 first;
        T2 second;
        Node* next = nullptr;
    }; 

    void push(const T1& first, const T2& second) {
        if (root == nullptr) {
            root = new Node(first, second);
            last = root;
        }
        else {
            last->next = new Node(first, second);
            last = last->next;
        }
    }

public:
    using NodePtr = Node*;
    
    NodePtr root;
    NodePtr last;

    list(T1 i, T2 j) {
        root = new Node(i, j);
        last = root;
    }
    list() {
        root = nullptr;
        last = root;
    }
    ~list() {
        delete root;
    }

    bool exists(const T1 &data) {
        for (NodePtr i = root; i != nullptr; i = i->next) {
            if (i->first == data) return true;
        }
        return false;
    }

    T2& operator[] (const T1& first) {
        for (NodePtr i = root; i != nullptr; i = i->next) {
            if (i->first == first) return i->second;
        }
        this->push(first, T2());
        return last->second;
    }

    bool empty() {
        return root == nullptr;
    }

    void print() {
        for (NodePtr i = root; i != nullptr; i = i->next) {
            std::cout << i->first << ' ' << i->second << std::endl;
        }
    }
};


//I hope it works
size_t pow(size_t base, size_t expo) {
    size_t output = 1;
    for (int i = 0; i < expo; i++) {
        output *= base;
    }
    return output;
}

class HashTable {
    static const int PRIME_CONST = 31;
    static const int ARR_SIZE = 150000;
    list<string, int> *arr = new list<string, int>[ARR_SIZE];

public:
    ~HashTable() {
        delete arr;
    }

    void put(string key, int value) {
        arr[hashFunc(key)][key] += value;
    }

    static int hashFunc(string key) {
        size_t sum = 0;
        for (size_t i = 0; i < key.length(); i++) {
            sum += (key[i] * (int)pow(PRIME_CONST, i)) % ARR_SIZE;
        }
        return sum % ARR_SIZE;
    }

    void printAll() {
        for (size_t i = 0; i < ARR_SIZE; i++) {
            if (!arr[i].empty()) {
                arr[i].print();
            }
        }
    }

    void topten() {
        list<string, int> topwords;
        size_t temp_big = 0;
        size_t last_big = 0;
        string temp_key;
        string last_key;
        for(int i = 0; i < 10; i++) {
            temp_big = 0;
            for (size_t j = 0; j < ARR_SIZE; j++) {
                for(auto k = arr[j].root; k != nullptr; k = k->next){
                    if(k->second > temp_big) {
                        if(i == 0) {
                            temp_key = k->first;
                            temp_big = k->second;
                        } 
                        else if( !topwords.exists(k->first) && last_big >= k->second && k->second > temp_big ) {
                            temp_key = k->first;
                            temp_big = k->second;
                        }
                    }
                }
            }
            last_key = temp_key;
            last_big = temp_big;
            topwords[last_key] = last_big;
        }
        topwords.print();
    }
};
//End of I hope it works

int arrayCurrentSize = 0;

bool checkIfStopWord(const string (&arr)[580], int p, int r, const string &wordle) {
    if (p <= r) {
        int mid = (p + r) / 2;
        if (arr[mid] == wordle)
            return true;
        if (arr[mid] > wordle)
            return checkIfStopWord(arr, p, mid - 1, wordle);
        if (arr[mid] < wordle)
            return checkIfStopWord(arr, mid + 1, r, wordle);
    }
    return false;
}

//string wordList[1000000] = { "" };
//int wordCount[1000000] = { 0 };
HashTable stoppWords;
string stopWords[580] = {""};

int main()
{
    clock_t time;
    time = clock();
    size_t size = 100000;
    int* pInt = new int[size];
    for (size_t i = 0; i < size; i++) {
        pInt[i] = rand();
    }

    //stop words
    ifstream StopWordsFile;
    string lineS;
    StopWordsFile.open("stopwords.txt");
    if (StopWordsFile.is_open()) {
        for (int i = 0; !StopWordsFile.eof(); i++) {
            getline(StopWordsFile, lineS);
            stopWords[i] = lineS;
        }
    }
    else {
        cout << "Error reading the StopWords" << endl;
    }
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    string search = "\"unigramCount\"";
    string line;

    ifstream Myfile;
    Myfile.open("PublicationsDataSet.txt");
    if (Myfile.is_open()) {
        cout << "Initialized" << endl;
        while (!Myfile.eof()) {
            getline(Myfile, line, '"');
            getline(Myfile, line, '"');
            if (line == "unigramCount") {
                do {
                    string number = "0";
                    string word = "";
                    string temp = "";

                    //Word getter codes
                    getline(Myfile, temp, '"');
                    if (Myfile.eof()) {
                        break;
                    }
                    while (Myfile.peek() != ':') {
                        getline(Myfile, temp, '"');
                        word.append(temp);
                    }

                    //ToLower + Remove special characters
                    for (int i = 0; i < word.length(); i++) {
                        word[i] = tolower(word[i]);
                        if (!((word[i] >= 'a' && word[i] <= 'z') || word[i] == '\'')) {
                            word.erase(i);
                            i--;
                        }
                    }

                    if (checkIfStopWord(stopWords, 0, 579, word) == true) {
                        continue;
                    }
                    

                    //Number getter codes
                    Myfile.ignore(1); //getline(Myfile, number, ':'); (Alternative)


                    while ('0' <= Myfile.peek() && Myfile.peek() <= '9') {
                        number.push_back(Myfile.get());
                    }


                    if (word == "creator" || Myfile.eof()) {//This will be changed at the end
                        break;
                    }

                    //cout << word << endl;
                    //cout << number << endl;

                    //Array Search Check
                    int number_converted = 0;

                    number_converted = stoi(number);
                    stoppWords.put(word, number_converted);


                    /*
                    bool isFound = false;
                    for (int i = 0; i < arrayCurrentSize; i++) {
                        if (wordList[i] == word) {
                            isFound = true;
                            wordCount[i] += number_converted;
                            break;
                        }
                    }
                    if (isFound == false) {
                        wordList[arrayCurrentSize] = word;
                        wordCount[arrayCurrentSize] = number_converted;
                        arrayCurrentSize++;
                    }
                    */

                } while (!Myfile.eof());
            }
        }
        Myfile.close();

    }
    else {
        cout << "Error";
    }

    //cout << "Top 10";
    //Find top 10
    /*
    string temp_biggest_word = "";
    int temp_biggest = 0;
    int temp_previous = 1000000; //This should be increased lated

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i <= arrayCurrentSize; i++) {
            if (wordCount[i] > temp_biggest && wordCount[i] < temp_previous) {
                temp_biggest = wordCount[i];
                temp_biggest_word = wordList[i];
            }
        }
        temp_previous = temp_biggest;
        cout << temp_biggest_word << " : " << temp_biggest << endl;
        temp_biggest = 0;
    }
    */

    //stoppWords.printAll();
    stoppWords.topten();

    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Elapsed Time = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << " ms" << std::endl;
    time = clock() - time;
    cout << "Elapsed Time: " << time / CLOCKS_PER_SEC << " seconds" << endl;
    return 0;
}