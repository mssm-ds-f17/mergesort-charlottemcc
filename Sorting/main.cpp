#include <iostream>
#include <functional>
#include <vector>
#include "gtest/gtest.h"

using namespace std;

class Thing {
public:
    int id;
    int thingNum;
    Thing(int id = 0);
};

Thing::Thing(int id) {
    this->id = id;
}

bool isSorted(const vector<int>& values) { //const makes sure that the vector is not changed
    if(values.size() > 0) {
        for(unsigned int i = 0; i < values.size() - 1; i++) {
            if(values[i] > values[i+1]) {
                return false;
            }
        }
    }

    return true;
}

int random(int maxValue) {
    return rand() % maxValue + 1;
}

//ids will range from 1 to maxid
//thingNum will be asending order
vector<Thing> makeRandomThings(int count, int maxId) {
    vector<Thing> things;

        for (int i = 0; i < count; i++) {
            things.push_back(Thing{random(maxId)});
        }

        for (int i = 0; i < count; i++) {
            things[i].thingNum = i+1;
        }

        return things;
}

void printVector(vector<Thing> values) {
    for(unsigned int i = 0; i < values.size(); i++) {
        cout << values[i].thingNum << ", " << values[i].id << endl;
    }
}

//assuming already sorted
bool isStable(vector<Thing> values) {
    for(unsigned int i = 0; i < values.size() - 1; i++) {
        if(values[i].id == values[i+1].id) {
            if(values[i].thingNum > values[i+1].thingNum) {
                return false;
            }
        }
    }

    return true;
}

//return true if thing a is less than thing b
bool compareThingsById(const Thing& a, const Thing& b) {
    return a.id < b.id;
}

bool isSorted(const vector<Thing>& values, function<bool(const Thing& a, const Thing& b)> comp) {
    if(values.size() > 0) {
        for(unsigned int i = 0; i < values.size() - 1; i++) {
            if(comp(values[i+1], values[i])) {
                return false;
            }
        }
    }

    return true;
}

//assumes that vector a and b are each sorted
vector<Thing> merge(vector<Thing> a, vector<Thing> b, function<bool(const Thing& a, const Thing& b)> comp) {
    vector<Thing> retval;

    unsigned int aI = 0;
    unsigned int bI = 0;

    while (aI < a.size() || bI < b.size()) {
        if(a.size() > 0 && b.size() > 0) {
            if(aI < a.size() && bI < b.size()) {
                if(comp(a[aI], b[bI])) {
                    retval.push_back(a[aI]);
                    aI++;
                }
                else {
                    retval.push_back(b[bI]);
                    bI++;
                }
            }
            else if(aI < a.size()) {
                retval.push_back(a[aI]);
                aI++;
            }
            else if(bI < b.size()) {
                retval.push_back(b[bI]);
                bI++;
            }
        }
    }

    return retval;
}

vector<Thing> mergeSort(vector<Thing> values, function<bool(const Thing& a, const Thing& b)> comp) {
    vector<Thing> retval = values;

    if(retval.size() == 2) {
        if(!comp(retval[0], retval[1])) {
            Thing larger = retval[0];
            retval[0] = retval[1];
            retval[1] = larger;
        }

        return retval;
    }
    else if(retval.size() > 1) {
        //split into two halves
        vector<Thing> half1;
        vector<Thing> half2;

        for(unsigned int i = 0; i < retval.size(); i++) {
            if(i <= retval.size()/2 - 1) {
                half1.push_back(retval[i]);
            }
            else {
                half2.push_back(retval[i]);
            }
        }

        //sort each half
        half1 = mergeSort(half1, comp);
        half2 = mergeSort(half2, comp);

        //merge the two halves back into the original retval vector
        retval = merge(half1, half2, comp);
    }

    return retval;
}

void insertValue(const Thing& value, vector<Thing>& things, function<bool(const Thing& a, const Thing& b)> comp) {

    unsigned int insertionSpot = 0;

    for(; insertionSpot < things.size(); insertionSpot++) {
        if(comp(value, things[insertionSpot])) {
            break;
        }
    }

    things.resize(things.size() + 1); //create a space at the end

    for(unsigned int i = things.size()-1; i > insertionSpot + 1; i--) {
        things[i] = things[i-1];
    }
}

//assumes that things is sorted (precondition)
//postcondition things is still sorted and 'value' is in the vector
void insertionSort(vector<Thing>& values, function<bool(const Thing& a, const Thing& b)> comp) {
    //create an empty output vector
    //loop through the first vector
    //insert into output vector in the right spot

    vector<Thing> output;

    for(Thing& t: values) {
        insertValue(t, output, comp);
    }

    values = output;
}

TEST(SortThing, sortNone) {
    vector<Thing> v;
    insertionSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortTest, isEmptyVectorSortedInt) {
    vector<int> v;
    ASSERT_TRUE(isSorted(v));
}

TEST(SortTest, isSortedVectorSorted) {
    vector<Thing> v {1,3,5,5,9,100};
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortTest, isUnsortedVectorSorted) {
    vector<Thing> v {1,3,5,1,9,100};
    ASSERT_FALSE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortOneInsertion) {
    vector<Thing> v { 5 };
    insertionSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortTwoAInsertion) {
    vector<Thing> v { 3, 4 };
    insertionSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortTwoBInsertion) {
    vector<Thing> v { 4, 3 };
    insertionSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortTwoCInsertion) {
    vector<Thing> v { 4, 4 };
    insertionSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortThreeInsertion) {
    vector<Thing> v{ 3, 1, 2 };
    insertionSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortTest, isSingleValueVectorSorted) {
    vector<Thing> v { 1 };
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, testMerge) {
    vector<Thing> a {3, 5, 7, 9};
    vector<Thing> b {1, 6, 10, 11};
    ASSERT_TRUE(isSorted(merge(a, b, compareThingsById), compareThingsById));
}

TEST(SortThing, testMergeSortEmpty) {
    vector<Thing> v;
    v = mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, testMergeSortOne) {
    vector<Thing> v {5};
    v = mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, testMergeSortEven) {
    vector<Thing> v {0, 6, 2, 10, 9, 13, 1, 4};
    v = mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, testMergeSortOdd) {
    vector<Thing> v {3, 5, 1, 44, 6, 2, 11};
    v = mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, testMergeSortMultiples) {
    vector<Thing> v {2, 2, 4, 5, 2, 4, 2};
    v = mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, testStableMerge) {
    vector<Thing> v = makeRandomThings(4, 10);
    v = mergeSort(v, compareThingsById);
    ASSERT_TRUE(isStable(v));
}

int main(int argc, char *argv[])
{
   ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
