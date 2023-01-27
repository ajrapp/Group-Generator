//
//  Group.cpp
//  Group-Generator
//
//  Created by Aidan Rappaport on 1/7/23.
//

#include "Group.hpp"
#include <vector>

using namespace std;

Person::Person(bool has_ride_, string name_, string phone_, string days)
    : has_ride(has_ride_), name(name_), phone(phone_) {
        
        stringstream ss(days);
        while (ss.good()) {
            string day;
            getline(ss, day, ',');
            if (day[0] == ' ') {
                day[0] = day[1];
                day[1] = day[2];
            }
            
            if (day[0] == 'M') {
                available.push_back(Day::Monday);
            }
            else if (day[0] == 'T') {
                if (day[1] == 'u') {
                    available.push_back(Day::Tuesday);
                }
                else {
                    available.push_back(Day::Thursday);
                }
            }
            else if (day[0] == 'W') {
                available.push_back(Day::Wednesday);
            }
            else if (day[0] == 'F') {
                available.push_back(Day::Friday);
            }
            else if (day[0] == 'S') {
                if (day[1] == 'a') {
                    available.push_back(Day::Saturday);
                }
                else {
                    available.push_back(Day::Sunday);
                }
            }
        }
        
        //print to check
        /*
        cout << "Name: " << name << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Ride: " << has_ride << "\n";
        cout << "Days: ";
        for (size_t i = 0; i < available.size(); i++) {
            cout << available[i] << ", ";
        }
        cout << "\n\n";
        */
}

void Group::read(string filename) {
    ifstream file(filename);
    
    if(!file.is_open()) throw std::runtime_error("Could not open file");
    
    string junk;
    //get rid of header
    if (file.good()) {
        getline(file, junk);
    }
        
    string name;
    string phone;
    string days;
    string has_ride;
    while (!file.eof()) {
        getline(file, junk,  ',');
        getline(file, junk,  ',');
        getline(file, name,  ',');
        getline(file, phone, ',');
        if (file.peek() == '"') {
            getline(file, days, '"');
            getline(file, days, '"');
        }
        else {
            getline(file, days, ',');
        }
        getline(file, has_ride);
        if (has_ride[0] == ',') {
            has_ride[0] = has_ride[1];
        }
        bool ride = has_ride[0] == 'N' ? false : true;
        
        if (ride) {
            drivers.emplace_back(Person(ride, name, phone, days));
        }
        else {
            people.emplace_back(Person(ride, name, phone, days));
        }
    }
} //read()

void Group::generate() {
    
    for (size_t i = 0; i < people.size(); i++) {
        Person* curr_person = &people[i];
        bool placed = false;
        
        Day smallest = curr_person->available[0];
        for (size_t day = 0; day < curr_person->available.size(); day++) {
            Day curr_day = curr_person->available[day];
            
            auto it = groups.find(curr_day);
            if (it == groups.end()) {
                vector<Person> vec;
                vec.reserve(4);
                vec.push_back(*curr_person);
                groups[curr_day][0] = vec;
                placed = true;
                break;
            }//if
            
            //sets smallest group so far to add person into
            size_t last_curr_size = groups[curr_day][groups[curr_day].size() - 1].size();
            size_t last_smlst_size = groups[smallest][groups[smallest].size() - 1].size();
            if (curr_day != smallest)
                smallest = last_curr_size < last_smlst_size ? curr_day : smallest;
        } //for day
        
        if (placed) continue;
        //else, place in day with smallest so far
        else {
            if (groups[smallest][groups[smallest].size() - 1].size() == 4) {
                vector<Person> vec;
                vec.reserve(4);
                vec.push_back(*curr_person);
                groups[smallest].push_back(vec);
            }
            else {
                groups[smallest][groups[smallest].size() - 1].push_back(*curr_person);
            }
        }
        
    }
} //generate()
