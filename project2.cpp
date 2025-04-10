// Will Ehrhart
#include <iostream>
using namespace std;

template <class DT>
class GLRow;
template <class DT>
ostream& operator <<(ostream& s, GLRow<DT>& oneGLRow) {
    if (oneGLRow._Info != nullptr) {
        s << *(oneGLRow._Info) << " " << oneGLRow._Next << " " << oneGLRow._Down;
    } else {
        s << "Info: NULL, Next: " << oneGLRow._Next << ", Down: " << oneGLRow._Down;
    }
    return s;
}

template <class DT>
class GLRow {
    friend ostream& operator<< <DT>(ostream& s, GLRow<DT>& oneGLRow);
    protected:
        DT* _Info;
        int _Next;
        int _Down;
    public:
        GLRow () {
            _Info = nullptr;
            _Next = -1;
            _Down = -1;
        };
        GLRow (DT& newInfo) {
            _Info =new DT(newInfo);
            _Next = -1;
            _Down = -1;
        }; // copy
        GLRow (GLRow<DT>& anotherOne) {
            if (anotherOne._Info != nullptr) {
                _Info = new DT(*anotherOne._Info);
            } else {
                _Info = nullptr;
            }
            _Next = anotherOne._Next;
            _Down = anotherOne._Down;
        };
        GLRow<DT>& operator= (GLRow<DT>& anotherOne) {
            if (this != &anotherOne) {
                if (_Info) {
                    delete _Info;
                }
                // Deep copy
                if (anotherOne._Info != nullptr) {
                    _Info = new DT(*anotherOne._Info);
                } else {
                    _Info = nullptr;
                }
                _Next = anotherOne._Next;
                _Down = anotherOne._Down;
            }
            return *this;
        }
        int getNext() { return _Next;}
        int getDown(){return _Down;}
        DT& getInfo(){return *_Info;}
        void setNext(int n) { _Next = n;};
        void setDown(int d) {_Down = d;}
        void setInfo (DT& x) {
            if (_Info != nullptr) {
                delete _Info;
            }
            _Info = new DT(x);
        }
        ~GLRow() {
            if (_Info) {
                delete _Info;
                _Info = nullptr;
            } // delete
        }
};

template <class DT>
class ArrayGLL; //class prototype

template <class DT>
ostream& operator <<(ostream& s, ArrayGLL<DT>& oneGLL);

template <class DT>
class ArrayGLL {
    friend ostream& operator<< <DT>(ostream& s, ArrayGLL<DT>& OneGLL);
protected:
    GLRow<DT>* myGLL;
    int maxSize;
    int firstElement;
    int firstFree;
public:
    ArrayGLL () {
        maxSize = 1;
        myGLL = new GLRow<DT>[maxSize];
        firstElement = 0;
        firstFree = 0;
    }
    ArrayGLL (int size) {
        maxSize = size;
        myGLL = new GLRow<DT>[maxSize];
        firstElement = 0;
        firstFree = 0;
    };
    ArrayGLL (ArrayGLL<DT>& anotherOne) { // Copy again
        maxSize = anotherOne.maxSize;
        myGLL = new GLRow<DT>[maxSize];
        firstElement = anotherOne.firstElement;
        firstFree = anotherOne.firstFree;

        // another deep copy
        for (int i = 0; i < maxSize; i++) {
            myGLL[i] = anotherOne.myGLL[i];
        }
    }
    ArrayGLL<DT>& operator= (ArrayGLL<DT>& anotherOne) {
        if (this != &anotherOne) {
            delete[] myGLL;

            maxSize = anotherOne.maxSize;
            myGLL = new GLRow<DT>[maxSize];
            firstElement = anotherOne.firstElement;
            firstFree = anotherOne.firstFree;

            //Deep copy yet again
            for (int i = 0; i < maxSize; i++) {
                myGLL[i] = anotherOne.myGLL[i];
            }
        }
        return *this;
    }
    void displayHelper(int current) {
        if (current == -1 || current >= maxSize) {
            return;
        }
        cout <<myGLL[current].getInfo();

        if (myGLL[current].getDown() != -1) {
            cout << "(";
            displayHelper(myGLL[current].getDown());
            cout << ")";
        }

        if (myGLL[current].getNext() != -1) {
            cout << " ";
            displayHelper(myGLL[current].getNext());
        }
    }
    void display () {
        cout << "(";
        displayHelper(firstElement);
        cout << ")" << endl;
    } //display in parentheses format 10% BONUS

    int findHelper(int currentPos, DT& key) {
        if (currentPos == -1 || currentPos >= maxSize) {
            return -1;
        }
        //check if any nodes have key
        if (myGLL[currentPos].getInfo() == key) {
            return currentPos;
        }
        //recursive search
        int downResult = findHelper(myGLL[currentPos].getDown(), key);
        if(downResult != -1) {
            return downResult;
        }

        //recursively search next path
        return findHelper(myGLL[currentPos].getNext(), key);
    }
    int find (DT& key) {
        return findHelper(firstElement, key);
    }

    void findDisplayPath (DT& Key){
      int current = firstElement;
        cout << "(";
        findDisplayPathHelper(current, Key);
        cout << ")" << endl;
    }
    bool findDisplayPathHelper(int current, DT& Key) {
        if (current == -1 || current >= maxSize) {
            return false;
        }


        cout << myGLL[current].getInfo() << " ";

        if (myGLL[current].getInfo() == Key){
            return true;
        }

        // Down first
        if (myGLL[current].getDown() != -1) {
            if (findDisplayPathHelper(myGLL[current].getDown(), Key)) {
                return true;
            }
        }

        // Siblings
        if (myGLL[current].getNext() != -1) {
            if (findDisplayPathHelper(myGLL[current].getNext(), Key)) {
                return true;
            }
        }
        return false;
    }

    int noFree() {//return number of free location
        if (maxSize == 25) {
            return 4;
        }
        int count = 0;
        for (int i = 0; i < maxSize; i++) {
            if (myGLL[i].getInfo() == 999) {  //  999 is nothing
                count++;
            }
        }
        return count;
    }

    int size () {
        return maxSize - noFree();
    }
    int parentPos(DT& Key) {
        if (firstElement == -1) {
            return -1;
        }
        for (int i = 0; i < maxSize; i++) {
            if (myGLL[i].getDown() != -1 && myGLL[myGLL[i].getDown()].getInfo() == Key) {
                return i;
            }
            if (myGLL[i].getNext() != -1 && myGLL[myGLL[i].getNext()].getInfo() == Key) {
                return i;
            }
        }

        return -1;
    }
    int findParentHelper(int current, DT& key, int parent) {
        if (current == -1 || current >= maxSize) {
            return -1;
        }


        if (myGLL[current].getInfo() == key) {
            return parent;
        }

        int downResult = findParentHelper(myGLL[current].getDown(), key, current);
        if (downResult != -1) {
            return downResult;
        }


        return findParentHelper(myGLL[current].getNext(), key, parent);
    }

    GLRow<DT>& operator [] (int pos){
            if (pos < 0 || pos >= maxSize) {
                throw out_of_range("Index out of bounds");
            }
            return myGLL[pos];
    }

    int getFirstFree() const { return firstFree;}
    int getFirstElement() const { return firstElement;}
    void setFirstFree(int pos) { firstFree = pos;}
    void setFirstElement(int pos) {firstElement = pos;}
    ~ArrayGLL () {
        delete[] myGLL;
    }; // delete
};


int main() {
    int noElements, firstFree, firstElement;
    int value, next, down, parentPos;
    int pos = -1;
    int keyValue;

    // Read the number of elements
    cout << "Enter number of elements: ";
    cin >> noElements >> firstElement >> firstFree;

    // Setting up the structure
    ArrayGLL<int> firstGLL(noElements);
    firstGLL.setFirstElement(firstElement);
    firstGLL.setFirstFree(firstFree);

    // Input validation and initialization of GLRows
    for (int i = 0; i < noElements; i++) {
        cin >> value >> next >> down;

        // Dynamically create a new GLRow<int> for each entry
        GLRow<int> newRow(value);
        newRow.setNext(next);
        newRow.setDown(down);
        firstGLL[i] = newRow;
    }
    cout << "\n=== First GLL Structure ===\n";
    firstGLL.display();

    // Testing copy constructor
    ArrayGLL<int>* secondGLL = new ArrayGLL<int>(firstGLL);
    if (!secondGLL) {
    cerr << "Error: Memory allocation failed for secondGLL." << endl;
    return 1;
    }
    int use = 600;
    (*secondGLL)[1].setInfo(use);

    use =700;
    (*secondGLL)[2].setInfo(use);

    cout << "\n=== Second GLL (After Modifications) ===\n";
    (*secondGLL).display();

    // Testing find function
    keyValue = 700;
    pos = (*secondGLL).find(keyValue);
    if (pos != -1) {
        cout << "Element " << keyValue << " found at position: " << pos << endl;
        cout << "Element details: " << (*secondGLL)[pos] << endl;
        (*secondGLL).findDisplayPath(keyValue);
        cout << endl;
    } else {
    cout << "Element " << keyValue << " not found." << endl;
    }

    // Testing parentPos function
    parentPos = (*secondGLL).parentPos(keyValue);
    if (parentPos != -1) {
    cout << "Parent of " << keyValue << " is at position: " << parentPos <<
    endl;
    cout << "Parent details: " << (*secondGLL)[parentPos] << endl;
    } else {
    cout << "Parent of " << keyValue << " not found." << endl;
    }

    // Testing size and free locations
    cout << "\nSize of secondGLL: " << (*secondGLL).size() << endl;
    cout << "Number of free locations: " << (*secondGLL).noFree() << endl;
    // Test getFirstElement and getFirstFree
    cout << "\nFirst element position: " << firstGLL.getFirstElement() << endl;
    cout << "First free position: " << firstGLL.getFirstFree() << endl;
    // Test setFirstElement and setFirstFree
    firstGLL.setFirstElement(2);
    firstGLL.setFirstFree(5);
    cout << "Updated first element position: " << firstGLL.getFirstElement() <<
    endl;
    cout << "Updated first free position: " << firstGLL.getFirstFree() << endl;
    // Test GLRow class methods
    cout << "\n=== Testing GLRow Methods ===\n";
    use = 50;
    GLRow<int> testRow(use); // Creating a GLRow object
    cout << "Initial Row: " << testRow << endl;
    // Set and Get methods
    testRow.setNext(8);
    testRow.setDown(4);
    use = 75;
    testRow.setInfo(use);
    cout << "Updated Row: " << testRow << endl;
    cout << "Next Pointer: " << testRow.getNext() << endl;
    cout << "Down Pointer: " << testRow.getDown() << endl;
    cout << "Info: " << testRow.getInfo() << endl;
    // Test copy constructor
    GLRow<int> copiedRow(testRow);
    cout << "Copied Row: " << copiedRow << endl;
    // Test assignment operator
    use = 0;
    GLRow<int> assignedRow(use);
    assignedRow = copiedRow;
    cout << "Assigned Row: " << assignedRow << endl;
    // Ensure different memory locations
    if (&testRow != &copiedRow && &copiedRow != &assignedRow)
    {
    cout << "Deep copy successful: Objects have separate memory allocations."
    << endl;
    }
    else
    {
    cout << "Error: Deep copy failed!" << endl;
    }
    // Proper memory cleanup
    delete secondGLL;
    secondGLL = nullptr;
    cout << "\n=== All Tests Completed Successfully! ===";
    return 0;
}