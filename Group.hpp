//
//  Group.hpp
//  Group-Generator
//
//  Created by Aidan Rappaport on 1/7/23.
//

#ifndef Group_hpp
#define Group_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

struct Person {
    bool has_ride;
    string name;
    string phone;
    vector<Day> available;

    
    Person(bool has_ride_, string name_, string phone_, string days);
};

class Group {
private:
    vector<Person> drivers;
    vector<Person> people;
    unordered_map<Day, vector<vector<Person>>> groups;
    
public:
    Group();
    
    void read(string filename);
    
    void generate();
    
    void print();
};

#endif /* Group_hpp */
