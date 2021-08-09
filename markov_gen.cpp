#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <numeric>
#include <assert.h>
#include <fstream>
#include <random>
#include <vector>

#define DICT std::unordered_map<std::string, std::unordered_map<char, double>>

double sum_val(const std::unordered_map<char, double>& vm){
    // dereferenced iterator type for b
    auto addition = [](double a, const std::pair<char, double>& b){ return a + b.second; }; 
    return std::accumulate(vm.begin(), vm.end(), 0, addition);
}   

std::pair<char, int> sample_char(const std::unordered_map<char, double>& vm){
    std::vector<std::pair<char, int>> poss_char;

    // ex. t = .25, f = .75:
    // {t,f,f,f ... t,f,f,f}, 100 total entries
    for(auto it = vm.begin(); it != vm.end(); it++){ // weighted distribution ~100 entries
        for(int i=0; i<it->second*100; i++){
            poss_char.push_back(*it);
        }
    }

    int rand = std::rand() % poss_char.size(); // gen rand # from 0 to poss_char.size()-1
    return poss_char[rand];
}

DICT gen_table(std::string& data, uint8_t k){
    DICT table;

    for(int i=0; i<data.length()-k; i++){
        std::string snippet = data.substr(i,k);

        if(table.find(snippet) != table.end()){ // snippet is already in map
            table[snippet][data[i+k]]++;
        }

        else{ 
            std::unordered_map<char, double> value; // create map as value corresponding to key
            value[data[i+k]] = 1; // give char k+1 occurance 1 in value map
            table[snippet] = value;
        }
        
    }

    for(auto it : table){ // normalize into markov chain
        double tot_occ = sum_val(table[it.first]); // sum values of the value map for each key
        for(auto it2 : table[it.first]){
            table[it.first][it2.first] /= tot_occ; // normalize # of occurances
        }
    }

    return table;
}

// determine the most probable next character given a starting sample
char find_next_char(const std::string& sample, const DICT& table, uint8_t k){
    uint8_t len = sample.length();
    assert(len >= k); // check that we have enough starting chars to match

    std::string snippet = sample.substr(len-k, k);
    std::cout << "NEW WORD: " << snippet << std::endl;
    if(table.find(snippet) == table.end()) { return ' '; } // return a space if pattern not recognized

    // use const accessor .at() instead of [], which is not a const function
    std::pair<char, int> next = sample_char(table.at(snippet));
    return next.first;
}

std::string gen_text(const std::string& sample, const DICT& table, uint8_t k, int num_char){
    std::string text = sample;
    uint8_t len = sample.length();

    for(int i = len-k; i<num_char; i++){ // generate characters until we have num_char characters
        std::cout << "NEXT SUBSTR: " << text.substr(i,k+1) << std::endl; 
        text += find_next_char(text.substr(i,k+1), table, k);
    }

    return text;
}

int main(){
    std::fstream data_file;
    data_file.open("data4.txt", std::ios::in); // open data1.txt for read
    assert(data_file); // make sure file has been succesfully opened

    std::string data = std::string();
    std::string line;
    while(getline(data_file, line)){
        data += line;
    }

    uint8_t k = 4;
    auto result = gen_table(data, k);

    // for(auto it : result){
    //     for(auto it2 : result[it.first]){
    //         std::cout << "STR: " << it.first << " NEXT CHAR: " << it2.first << " PROB: " << it2.second << "\n";
    //     }
    // }

    std::string text = gen_text("Namjoon", result, k, 2000);
    std::cout << "\n" << text;

    data_file.close();

}