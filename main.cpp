// We have not yet produced the test driver main routine for you.
#include "provided.h"
#include "http.h"
//#include "HashTable.h"
#include <iostream>
using namespace std;


int main()
{
    string originalPageText =
    "<html>\n"
    "<head>\n"
    " <title>My Pretend Web Page</title>\n"
    "</head>\n"
    "<body>\n"
    "<h2>My Pretend Web Page<h1>\n"
    "<p>\n"
    "I wish I were creative enough to have something interesting \n"
    "to say here.  I'm not, though, so this is a boring page.\n"
    "</p>\n"
    "<p>\n"
    "Oh, well.\n"
    "</p>\n"
    "</body>\n"
    "</html>\n"
    ;
    HTTP().set("http://boring.com", originalPageText);
    string plan =
    "Lefty and Mugsy enter the bank by the back door.\n"
    "Shorty waits out front in the getaway car.\n"
    "Don't let the teller trip the silent alarm.\n"
    ;
    string newPageText;
    if ( ! WebSteg::hideMessageInPage("http://boring.com", plan, newPageText))
    {
        cout << "Error hiding!" << endl;
        return 1;
    }
    HTTP().set("http://boring.com", newPageText);
    string msg;
    if ( ! WebSteg::revealMessageInPage("http://boring.com", msg))
    {
        cout << "Error revealing!" << endl;
        return 1;
    }
    if (msg != plan)
    {
        cout << "Message not recovered properly:\n" << msg;
        return 1;
    }
    cout << "Recovered text is what was hidden:\n" << msg;
}

/*
#include "provided.h"
#include "http.h"
#include "substituteHashTable.h"
#include <iostream>
using namespace std;
*/

//int main(){
//    string text;
//    string str = "shivam is gay fuck shit bullshit niggers niggers niggers niggers niggers niggers niggers A call to Multiset<url>::insert will eventually call […Multiset<url>::find(const ItemType& value)…] which contains the line";
//    /*
//    for(int i = 0; i<200; i++){
//        str.push_back(static_cast<char>(rand()%256));
//    }
//     */
//    
//    if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", str, text))
//        cout << "Error hiding!" << endl;
//    string msg;
//    HTTP().set("http://cs.ucla.edu", text);
//    if ( ! WebSteg::revealMessageInPage("http://cs.ucla.edu", msg))
//        cout << "Error revealing!" << endl;
//    if(msg == str)
//        cout << "Code works yay!" << endl;
//    cout << msg << endl << endl;
//    cout << str;
//}

/*
int main(){
    string page;
    HTTP().get("http://cs.ucla.edu", page);
    string out;
    string andyIsGay ="shivam is gay fuck shit bullshit niggers niggers niggers niggers niggers niggers niggers A call to Multiset<url>::insert will eventually call […Multiset<url>::find(const ItemType& value)…] which contains the line";
    cout << "---- STEG HIDE ----" << endl;
    Steg::hide(page, andyIsGay,out);
    //cout << out << endl;
    
    
    cout << "---- STEG REVEAL ----" << endl;
    string msg = "";
    //cout << out << endl;
    Steg::reveal(out, msg);
    cout << (msg == andyIsGay) << endl;
    cout << msg.size() << endl;
    cout << msg << endl;
    cout << andyIsGay.size() << endl;
}
*/
/*

unsigned int computeHash(string s){
    return s.size();
}


int main(){
    HashTable<string, int> table(10,20);
    table.set("roh", 18);
    table.set("rak", 10, true);
    table.set("shi",199);
    table.touch("shi");
    string key;
    int val;
    table.discard(key, val);
    table.touch("shi");
    cout << val << endl;
    table.set("roh", 20);
    table.get("roh", val);
    table.get("rak", val);
    table.get("roh", val);
    cout << val << endl;
    table.touch("rak");
    table.touch("shi");
    table.discard(key, val);
    cout << key << endl;
    cout << val << endl;
}
*/


/*
 int main(){
 vector<unsigned short> v;
 v.push_back(1);
 v.push_back(5);
 string s = BinaryConverter::encode(v);
 BinaryConverter::decode(s, v);
 cout << "---- BINARY CONVERTER DECODE ----" << endl;
 for(int i = 0; i<v.size(); i++){
 cout << v[i] << endl;
 }
 
 cout << "---- COMPRESSOR COMPRESS ----" << endl;
 vector<unsigned short> vec;
 Compressor::compress("AAAAAAAAAB", vec);
 for(int i = 0; i<vec.size(); i++){
     cout << vec[i] << endl;
 }
 
 cout << "---- COMPRESSOR DECOMPRESS ----" << endl;
 string str;
 if(Compressor::decompress(vec, str))
 cout << str << endl;
 else
 cout << "DECOMPRESS ERROR" << endl;
 }
 */
