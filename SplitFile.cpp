#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

class myRow;
class packet;
myRow** hashedArray;
vector<packet *> packets;
vector<myRow *> rawFile;
int maxId;
int filesProduced;

class myRow{
    public: 
        bool isRoot;
        myRow();
    
        string type;
        string symbol; 
        string price;
        string quantity;
        string expirydate;
        string strikeprice;
        string amendtime;
        int id;
        int parentid;

        myRow *parent;
        vector<myRow *> children;

        string getString(); 
        int getSubtreeSize();
        string getSubtreeAsString();
};

myRow::myRow():isRoot(true), parent(NULL){}

string myRow::getString(){
    return type + ',' + symbol + ',' + price + ',' + quantity + ',' + 
                 expirydate + ',' + strikeprice + ',' + amendtime + ',' + to_string(id) 
                 + ',' + to_string(parentid);
}

int myRow::getSubtreeSize(){
    int size = 1; 
    for(int i=0; i<children.size(); i++){
        size += children[i]->getSubtreeSize();
    }
    return size;
}

string myRow::getSubtreeAsString(){
    string res = getString(); 
    for(int i=0; i<children.size(); i++){
        res = res + "\n" + "\n" + children[i]->getSubtreeAsString();
    }
    return res;
}

class packet{ //required to avoid calculation of size repeatedly
    public: 
        int size;
        myRow *root; 
        bool operator < (const packet &p) const{
            return (size < p.size);
        }
};

void getPackets(){
    for(int i=0; i<=maxId; i++){
        myRow *current = hashedArray[i];
        if(current != NULL and current->isRoot){
            packet *p = new packet();
            p->size = current->getSubtreeSize();
            p->root = current;
            packets.push_back(p);
        }
    }
}

void getHashedFile(){
    hashedArray = new myRow * [maxId+1];
    for(int i=0; i<=maxId; i++)
        hashedArray[i] = NULL;
    for(int i=0; i<rawFile.size(); i++){
        hashedArray[rawFile[i]->id] = rawFile[i];
    }
    for(int i=0; i<=maxId; i++){
        myRow *current = hashedArray[i];
        if(current != NULL){
            if(current->type.compare("P") == 0){
                current->isRoot = false;
                current->parent = hashedArray[current->parentid];
                hashedArray[current->parentid]->children.push_back(current);
            }
        }
    }
}

void readInputFile(char *fileName){
    ifstream file(fileName);
    string str; 

    maxId = INT_MIN;
    while (getline(file, str))
    {
        if(str.size() == 0)
            continue;

        myRow *row = new myRow;

        istringstream iss(str);

        getline(iss, row->type, ',');
        getline(iss, row->symbol, ',');
        getline(iss, row->price, ',');
        getline(iss, row->quantity, ',');
        getline(iss, row->expirydate, ',');
        getline(iss, row->strikeprice, ',');
        getline(iss, row->amendtime, ',');
        string s;
        getline(iss, s, ',');
        row->id = stoi(s);
        getline(iss, s, ',');
        row->parentid = stoi(s);

        rawFile.push_back(row);

        if(maxId < row->id)
            maxId = row->id;
    }
}

bool comparePtrToPacket(packet* a, packet* b) { 
    return (*a < *b); 
}

void dump(vector<packet *> *selected, int sizeLeft){
    if(sizeLeft <= 0){
        filesProduced+=1;
        string fileName = "output_" + to_string(filesProduced) + ".txt";
        ofstream out(fileName);
        for(int i=0; i<selected->size(); i++ ){
            out << (*selected)[i]->root->getSubtreeAsString() << endl << endl; 
        }
        out.close();
    }else{ 
        ofstream out;
        string fileName = "output_" + to_string(filesProduced) + ".txt";
        out.open(fileName, ios_base::app);
        for(int i=0; i<selected->size(); i++ ){
            out << (*selected)[i]->root->getSubtreeAsString() << endl << endl;
        }
        out.close();
    }
}

void getSubfiles(int X){
    while(packets.size() != 0){
        vector<packet *> selected;
        packet *current = packets.back();
        int sizeLeft = X-current->size;
        selected.push_back(current);
        packets.pop_back();
        while(packets.size()!=0 and sizeLeft > 0){
            bool sizeCrossed = false;
            int i;
            for(i=0; i<packets.size(); i++){
                if(sizeLeft - packets[i]->size <= 0){
                    sizeCrossed = true;
                    break;
                }
            }
            if(sizeCrossed){
                selected.push_back(packets[i]);
                sizeLeft = sizeLeft - packets[i]->size;
                packets.erase(packets.begin()+i);
            }else{
                selected.push_back(packets.back());
                sizeLeft = sizeLeft - packets.back()->size;
                packets.pop_back();
            }
        }
        dump(&selected, sizeLeft);
    }
}

int main(int argc, char **argv){
    filesProduced = 0;
    if(argc < 3){
        cout << "Usage: ./fileReduce <inputFileName> <X: Capacity of smaller file>";
        exit(1);
    }
    int X = atoi(argv[2]);
    readInputFile(argv[1]); 
    getHashedFile();
    getPackets();
    sort(packets.begin(), packets.end(), comparePtrToPacket);
    getSubfiles(X);

    return 0; 
}
