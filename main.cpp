//
//  main.cpp
//  Group-Generator
//
//  Created by Aidan Rappaport on 1/7/23.
//

#include "Group.hpp"
#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    //ios_base::sync_with_stdio(false);
    //xcode_redirect(argc, argv);
    cin >> std::boolalpha;
    cout << std::boolalpha;

    string filename = argv[1];
    Group group;
    group.read(filename);
    
    return 0;
}
