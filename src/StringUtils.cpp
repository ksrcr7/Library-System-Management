#include <algorithm>
#include <cctype>

std::string trim(const std::string& str){
    auto first = std::find_if_not(str.begin(),str.end(),
                            [](unsigned char c){
                                return std::isspace(c);
                            });
    
    auto last = std::find_if_not(str.rbegin(),str.rend(),
                                [](unsigned char c){
                                    return std::isspace(c);
                                }).base();
                                
    if(first >= last){
        return "";
    }    
    
    return std::string(first,last);
}
