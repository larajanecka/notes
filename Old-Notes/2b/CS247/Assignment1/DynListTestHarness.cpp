#include <iostream>

using namespace std;


//************************************************************************
//  DynList ADT
//************************************************************************

class DynList {
public:
    DynList();                                  // default constructor
    ~DynList();                                 // destructor
    DynList(const DynList&);                    // copy constructor
    DynList& operator=( const DynList& );       // assignment
    bool operator==( const DynList& ) const;    // equality
    string& operator[]( int i );				// accessor 
    int size() const;                           // accessor 
private:
    class Node;
    Node *list_;
    Node *last_;
    int size_;
    void deleteAllNodes();
    void copyAllNodes(DynList&, const DynList&);
};


class DynList::Node {
    friend string& DynList::operator[] (int i);
private:
    string value;
    Node *next;
public:
    Node* getNext() const;
    string getValue() const;
};

// accessor -- operator[]
string& DynList::operator[] (int i) {
    // check whether array needs to grow
    if (0 <= i && i < size_) {
		
        // locate ith node and return contents
        Node *curr;
        int index;
        for (curr = list_, index = 0; index < i; curr = curr->next, index+=1);
        return curr->value;
    }
	
    // otherwise, need to grow the list. New elements have empty strings
    else {
        if (size_ == 0) {
            Node *node = new Node;
            node->value = "";
            node->next = 0;
            list_ = node;
            last_ = node;
            size_ = 1;
        }
        for (int index = size_; index <= i; index+=1) {
            Node *node = new Node;
            node->value = "";
            node->next = 0;
            last_->next = node;
            last_ = node;
            size_ += 1;
        }
		
        return last_->value;
    }
}

// accessor - size of list
int DynList::size() const {
    return size_;
}
//************************************************************************
//* YOUR IMPLENTATION HERE 
//***********************************************************************
DynList::DynList() : size_(0){}

DynList::DynList(const DynList& d) : size_(0) {
    this->copyAllNodes(*this, d);
}

DynList::~DynList() {
    this->deleteAllNodes();
}

DynList& DynList::operator=(const DynList& d) {
    this->deleteAllNodes();
    this->copyAllNodes(*this, d);
    return *this;
}

bool DynList::operator==(const DynList& d) const {
    if(size_ != d.size()) {
        return false;
    }
    int i = 0;
    for (DynList::Node *currThis = this->list_, *currD = d.list_ ; i < this->size(); currThis = currThis->getNext(), currD = currD->getNext(), ++i)
    {
        if(currThis->getValue() != currD->getValue()){
            return false;
        }
    }
    return true;
}

void DynList::deleteAllNodes() {
    DynList::Node* temp;
    int i = 0; 
    for(DynList::Node* curr = this->list_; i < this->size(); curr = curr->getNext(), ++i) {
        temp = curr;
        delete curr; //may fuck shit up
        curr = temp;
    }
    this->size_ = 0;
} 

void DynList::copyAllNodes(DynList& copier, const DynList& copied) {
    if (copier == copied){
        return;
    }
    int i = 0;
    for(DynList::Node* curr = copied.list_; i < copied.size(); curr = curr->getNext(), ++i) {
        copier[i] = curr->getValue();
    }
}


DynList::Node* DynList::Node::getNext() const {
    return next;
}

string DynList::Node::getValue() const { 
    return value;
} 







//************************************************************************
//  Helper variables and functions for test harness
//************************************************************************
//  test harness commands
enum Op {NONE, Con, Des, Size, Copy, Read, Mut, Assign, Eq};


// parse input command
Op convertOp(string opStr) {
    switch (opStr[0]) {
        case 'x': return Con;
        case 'd': return Des;
        case 's': return Size;
        case 'c': return Copy;
        case 'r': return Read;
        case 'm': return Mut;
        case 'a': return Assign;
        case 'e': return Eq;
        default: return NONE;
    }
}

// parse name of dynamic list that is to be operated on
int readName() {	
    int index = -1;
    cin >> index;
    if ( index >= 0 && index <= 9 ) return index;
    
    cout << "Invalid name of dynamic list!" << endl;
    
    // try to fix cin
    cin.clear();
    string junk;
    getline( cin, junk );
    
    return -1;
}


//*******************
// main()
//*******************


int main() {
    cout << "Test harness for DynList ADT:" << endl << endl;
	
	
    // create a collection of dynamic lists to manipulate
	DynList *lists[10] = {0};
	
    // get input command
    cout << "Command: ";
    string command;
    cin >> command;
	
    Op op = convertOp(command);
	
    while ( !cin.eof() ) {
        switch (op) {
            // default construction of a new dynamic list
            case Con: {
                int name = readName();
                if ( name != -1 ) {
                    delete lists[name];
                    lists[name] = new DynList;
                }
                break;
            }
				
            // destroy a dynamic list
            case Des: {
                int name = readName();
                if ( name != -1 ) {
                    if ( lists[name] == 0 ) {
                        cout << "List " << name << " is not yet defined." << endl;
                        break;
                    }
                    delete lists[name];
                    lists[name] = 0;
                }
                break;
            }
                
                // use operator[] to read/access element in a list
            case Read: {
                int name = readName();
                if ( name != -1 ) {
                    if ( lists[name] == 0 ) {
                        cout << "List " << name << " is not yet defined." << endl;
                        break;
                    }
                    int index;
                    cin >> index;
                    if ( index < 0 ) {
                        cout << "Invalid index." << endl;
                        break;
                    }
                    cout << "Value of the " << index << "th element of " << name << "th list  == \"" << ( *lists[name] )[index] << "\"" << endl;;
                }
                break;
            }
				
            // change the value of an element in a list, by assigning to the result of operator[]
            case Mut: {
                int name = readName();
                if ( name != -1 ) {
                    if (lists[name] == 0) {
                        cout << "List " << name << " is not yet defined." << endl;
                        break;
                    }
                    int index;
                    cin >> index;
                    if ( index < 0 ) {
                        cout << "Invalid index." << endl;
                        break;
                    }
                    string newValue;
                    cin >> newValue;
                    ( *lists[name] )[index] = newValue;
                    cout << "Value of the " << index << "th element of " << name << "th list == \"" << ( *lists[name] )[index] << "\"" << endl;;
                }
                break;				
            }
				
            // query the size of a list
            case Size: {
                int name = readName();
                if ( name != -1 ) {
                    if (lists[name] == 0) {
                        cout << "List " << name << " is not yet defined." << endl;
                        break;
                    }
                    cout << "Size of the " << name << "th list  = " << lists[name]->size() << endl;
                }
                break;					
            }
				
            // test whether two lists are equal copies of each other
            case Eq: {
                int name = readName();
                if ( name != -1 ) {
                    if ( lists[name] == 0 ) {
                        cout << "List " << name << " is not yet defined." << endl;
                        break;
                    }
                    
                    int name2 = readName();
                    if ( name2 != -1 ) {
                        if ( lists[name2] == 0 ) {
                            cout << "List " << name2 << " is not yet defined." << endl;
                            break;
                        }
                        
                        string eq = (*lists[name] == *lists[name2]) ? " " : " not ";
                        cout << "Lists " << name << " and " << name2 << " are" << eq << "equal." << endl;
                    }
                }
                break;
            }
                        
            // assign one dynamic list to another.  Print contents of both lists, to check results
            case Assign: {
                int name = readName();
                if ( name != -1 ) {
                    if ( lists[name] == 0 ) {
                        cout << "List " << name << " is not yet defined." << endl;
                        break;
                    }
					
                    int name2 = readName();
                    if ( name2 != -1 ) {
                        if (lists[name2] == 0) {
                            cout << "List " << name2 << " is not yet defined." << endl;
                            break;
                        }

                        *lists[name] = *lists[name2];
						
                        cout << "Size of " << name << "th list = " << lists[name]->size() << endl;
                        cout << "Value of " << name << "th list = [";
                        for (int i=0; i < (lists[name]->size()); i+=1) {
                            cout << (*lists[name])[i] << ",";
                        }
                        cout << "]" << endl;
                        cout << "Size of " << name2 << "th list = " << lists[name2]->size() << endl;
                        cout << "Value of " << name2 << "th list = [";
                        for (int i=0; i < (lists[name2]->size()); i+=1) {
                            cout << (*lists[name2])[i] << ",";
                        }
                        cout << "]" << endl;
                    }
                }
                break;
            }
			
            // use copy constructor to create a new list, initializing from an existing list
            // print the contents of both lists, to check results
            case Copy: {
                int name = readName();
                if ( name != -1 ) {
                    if (lists[name] == 0) {
                        cout << "List " << name << " is not yet defined." << endl;
                        break;
                    }
					
                    DynList copy(*lists[name]);
											
                    cout << "Size of " << name << "th list = " << lists[name]->size() << endl;
                    cout << "Value of " << name << "th list = [";
                    for (int i=0; i < (lists[name]->size()); i+=1) {
                        cout << (*lists[name])[i] << ",";
                    }
                    cout << "]" << endl;
					
                    cout << "Size of copy" << " = " << copy.size() << endl;
                    cout << "Value of copy" << " = [";
                    for (int i=0; i < (copy.size()); i+=1) {
                        cout << copy[i] << ",";
                    }
                    cout << "]" << endl;
                }
                break;
            }
            default: {
                cout << "Invalid command." << endl;
            }
        } // switch command
				
        cout << endl << "Command: ";
        cin >> command;
        op = convertOp(command);
				

    } // while cin OK
    
    for (int i = 0; i < 10; i++) {
        delete lists[i];
    }
}
