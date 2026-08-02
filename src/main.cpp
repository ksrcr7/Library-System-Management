#include "Database.h"
#include <iostream>

int main(){
    try
    {
        Database DB("database/library.db");
        std::cerr<<"Database opened successfully."<<std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

};