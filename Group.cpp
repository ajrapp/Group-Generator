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

Group::Group() {
    next_open_idx[Day::Monday]    = 0;
    next_open_idx[Day::Tuesday]   = 0;
    next_open_idx[Day::Wednesday] = 0;
    next_open_idx[Day::Thursday]  = 0;
    next_open_idx[Day::Friday]    = 0;
    next_open_idx[Day::Saturday]  = 0;
    next_open_idx[Day::Sunday]    = 0;
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
    assign_drivers();
    assign_rest();
    consolidate();
}

void Group::assign_drivers() {
    //assign drivers
    for (size_t i = 0; i < drivers.size(); i++) {
        Person* curr_driver = &drivers[i];
        bool placed = false;
        
        Day smallest = curr_driver->available[0];
        for (size_t day = 0; day < curr_driver->available.size(); day++) {
            Day curr_day = curr_driver->available[day];
            
            auto it = groups.find(curr_day);
            if (it == groups.end()) {
                vector<Person> vec;
                vec.reserve(4);
                vec.push_back(*curr_driver);
                groups[curr_day].push_back(vec);
                placed = true;
                break;
            }
            
            //sets smallest day to add new group
            size_t curr_day_size = groups[curr_day].size();
            size_t smlst_day_size = groups[smallest].size();
            if (curr_day != smallest) {
                smallest = curr_day_size < smlst_day_size ? curr_day : smallest;
            }
        }
        
        if (placed) continue;
        //else create new group in smallest day
        else {
            vector<Person> vec;
            vec.reserve(4);
            vec.push_back(*curr_driver);
            groups[smallest].push_back(vec);
        }
    } //for
} //assign_drivers()
    
void Group::assign_rest() {
    //assign nondrivers
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
                groups[curr_day].push_back(vec);
                placed = true;
                break;
            }//if
            
            //sets smallest group so far to add person into
            size_t last_curr_size = groups[curr_day][next_open_idx[curr_day]].size();
            size_t last_smlst_size = groups[smallest][next_open_idx[smallest]].size();
            if (curr_day != smallest)
                smallest = last_curr_size < last_smlst_size ? curr_day : smallest;
        } //for day
        
        if (placed) continue;
        //else, place in day with smallest so far
        else {
            groups[smallest][next_open_idx[smallest]].push_back(*curr_person);
            if (groups[smallest][next_open_idx[smallest]].size() == 4) {
                next_open_idx[smallest]++;
                if (groups[smallest].size() <= next_open_idx[smallest]) {
                    vector<Person> vec;
                    vec.reserve(4);
                    groups[smallest].push_back(vec);
                }
            }
        }//else
    } //for
} //assign_rest()

void Group::consolidate() {
    for (int i = 0; i < 7; i++) {
        Day curr_day = Day::Monday;
        if (i == 0) {
            curr_day = Day::Monday;
        }
        else if (i == 1) {
            curr_day = Day::Tuesday;
        }
        else if (i == 2) {
            curr_day = Day::Wednesday;
        }
        else if (i == 3) {
            curr_day = Day::Thursday;
        }
        else if (i == 4) {
            curr_day = Day::Friday;
        }
        else if (i == 5) {
            curr_day = Day::Saturday;
        }
        else if (i == 6) {
            curr_day = Day::Sunday;
        }
        
        //if no people in day
        auto it = groups.find(curr_day);
        if (it == groups.end()) {
            continue;
        }
        
        //if only one person in day, assign to next smallest
        if (groups[curr_day].size() == 1 && groups[curr_day][0].size() == 1) {
            Day smallest_other_day = curr_day;
            bool new_day_found = false;
            Person curr_person = groups[curr_day][0][0];
            
            for (size_t i = 0; i < curr_person.available.size(); i++) {
                Day curr_new_day = curr_person.available[i];
                if (curr_new_day == curr_day) {
                    continue;
                }
                
                if (!new_day_found) {
                    if (groups[curr_new_day].size() > 0) {
                        smallest_other_day = curr_new_day;
                        new_day_found = true;
                    }
                }
                else {
                    smallest_other_day = groups[smallest_other_day].size() > groups[curr_new_day].size() ? curr_new_day : smallest_other_day;
                }
            }
            
            if (smallest_other_day != curr_day) {
                groups[smallest_other_day][next_open_idx[smallest_other_day]].push_back(curr_person);
                if (groups[smallest_other_day][next_open_idx[smallest_other_day]].size() == 4) {
                    next_open_idx[smallest_other_day]++;
                }
                groups[curr_day].clear();
            }
        }
        
        for (size_t i = groups[curr_day].size() - 1; i > 0; i--) {
            size_t size_first = groups[curr_day][i - 1].size();
            size_t size_second = groups[curr_day][i].size();
            
            if (size_second == 4 || size_second == 3) {
                break;
            }
            
            else if (size_second == 2) {
                if (size_first == 4) {
                    groups[curr_day][i].push_back(groups[curr_day][i-1][3]);
                    groups[curr_day][i-1].erase(groups[curr_day][i-1].begin() + 3);
                }
                else if (size_first == 3) {
                    continue;
                }
                else if (size_first == 2) {
                    groups[curr_day][i-1].push_back(groups[curr_day][i][0]);
                    groups[curr_day][i-1].push_back(groups[curr_day][i][1]);
                    groups[curr_day].erase(groups[curr_day].begin() + i - 1);
                }
            }
            
            else if (size_second == 1) {
                if (size_first == 4) {
                    groups[curr_day][i].push_back(groups[curr_day][i-1][3]);
                    groups[curr_day][i-1].erase(groups[curr_day][i-1].begin() + 3);
                }
                else {
                    groups[curr_day][i-1].push_back(groups[curr_day][i][0]);
                    groups[curr_day].erase(groups[curr_day].begin() + i - 1);
                }
            }
        }//for
        
    }//for
} //consolidate()

void Group::print() {
    
    for (int i = 0; i < 7; i++) {
        Day curr;
        if (i == 0) {
            curr = Day::Monday;
            cout << "Monday: \n";
        }
        else if (i == 1) {
            curr = Day::Tuesday;
            cout << "Tuesday: \n";
        }
        else if (i == 2) {
            curr = Day::Wednesday;
            cout << "Wednesday: \n";
        }
        else if (i == 3) {
            curr = Day::Thursday;
            cout << "Thursday: \n";
        }
        else if (i == 4) {
            curr = Day::Friday;
            cout << "Friday: \n";
        }
        else if (i == 5) {
            curr = Day::Saturday;
            cout << "Saturday: \n";
        }
        else if (i == 6) {
            curr = Day::Sunday;
            cout << "Sunday: \n";
        }
        
        auto it = groups.find(curr);
        if (it == groups.end()) {
            continue;
        }
        
        for (size_t i = 0; i < groups[curr].size(); i++) {
            cout << "Group " << i << ": \n";
            for (size_t j = 0; j < groups[curr][i].size(); j++) {
                cout << groups[curr][i][j].name << " " << groups[curr][i][j].has_ride << "\n";
            }
            cout << "\n";
        }
        cout << "\n";
    }//for
}
