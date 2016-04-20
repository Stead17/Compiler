  //
//  gramm_analyzer.cpp
//  LexicalAnalyzer
//
//  Created by Ivan on 4/13/16.
//  Copyright © 2016 Ivan Tkachenko. All rights reserved.
//

#include "gramm_analyzer.hpp"

void gram_analyze(std::vector<std::vector<std::string>>& vect) {
    std::ofstream file;
    file.open("/Users/ivan/Developer/uniCode/ASM_2/LexAnalyzer/LexicalAnalyzer/LexicalAnalyzer/gram.txt");
    
    std::vector<std::string> user_id;
    std::vector<int> offset;
    int i = 0;
    
    while(i < vect.size()) {
        bool segment_check_flag = segment_check(vect, user_id, offset, i);
        std::vector<std::string> tmp_str = vect.at(i);
        for (int j = 0; j < tmp_str.size(); j++) {
            printf("%s ", tmp_str[j].c_str());
        }
        printf("\n");
        std::string str = tmp_str[0];
        if (segment_check_flag) {
            for (int j = i; j < offset.size(); ++j) {
                std::vector<std::string> tmp = vect[j];
                file << "000" << std::hex << offset[j] << "\t";
                copy(tmp.begin(), tmp.end(), std::ostream_iterator<std::string>(file, "\t"));
                file << "\n";
                
            }
            // change this!!
            if (i < 8)
                i += offset.size();
            else
                i++;
        } else if(str.compare("ASSUME") == 0) {
            file << "\t";
            copy(tmp_str.begin(), tmp_str.end(), std::ostream_iterator<std::string>(file, "\t"));
            file << "\n";
            if ((i + 1) <= vect.size())
                i++;
        } else if(label_check(vect[i])){
            file << "000" << std::hex << offset[offset.size() - 1] << "\t";
            copy(tmp_str.begin(), tmp_str.end(), std::ostream_iterator<std::string>(file, "\t"));
            file << "\n";
            if ((i + 1) <= vect.size())
                i++;
        } else {
            command_run(vect[i], offset, i);
            file << "00" << std::hex << offset[offset.size() - 1] << "\t";
            copy(tmp_str.begin(), tmp_str.end(), std::ostream_iterator<std::string>(file, "\t"));
            file << "\n";
            if ((i + 1) <= vect.size())
                i++;
        }
//        offset.clear();
    }
    
    file.close();
}

// Maybe add index int&
bool segment_check(std::vector<std::vector<std::string>>& vector, std::vector<std::string>& usr_id, std::vector<int>& offset, int index) {
    if (valid_segment_name(vector[index])) {
        offset.push_back(0);
        offset.push_back(0);
        if ((index + 1) < vector.size())
            index++;
        
        while (mnem_check(vector[index], usr_id, offset, index)) {
            if ((index + 1) < vector.size())
                index++;
        }
        
    } else {
        return false;
    }
    
    return true;
}

bool valid_segment_name(std::vector<std::string> sentence) {
    if (sentence.size() == 2 && id_check(sentence[0])) {
        if (!sentence[1].compare("SEGMENT")) {
            return true;
        }
    }
    
    return false;
}

bool id_check(std::string str) {
    if (!str.empty() && str.length() <= max_lexems_length) {
        for (int i = 0; i < str.length(); ++i)
            if ((!isalpha(str[i]) || !isdigit(str[i])) && islower(str[i]))
                return false;
        return true;
    } else {
        return false;
    }
}

bool mnem_check(std::vector<std::string>& vector, std::vector<std::string>& usr_id, std::vector<int>& offset, int indx) {
    std::string type_array[] = {"DB", "DD", "DW"};
    int last_elem_size = offset[offset.size() - 1];
    if (vector.size() >= 3) {
        usr_id.push_back(vector[0]);
        if (vector[1] == type_array[0]) {
            if (!vector[2].compare("'") && !vector[vector.size() - 1].compare("'")) {
                offset.push_back((int)(last_elem_size + vector[3].size()));
            } else {
                offset.push_back((int)(last_elem_size + 1));
            }
            return true;
        } else if(vector[1] == type_array[1]) {
            offset.push_back((last_elem_size + 4));
            return true;
        } else if(vector[1] == type_array[2]) {
            offset.push_back((last_elem_size + 2));
            return true;
        } else {
            return false;
        }
    }
    
    return false;
}

// Check if the row is label
bool label_check(std::vector<std::string> label) {
    if (!label[1].compare(":") && id_check(label[0]))
        return true;
    return false;
}

void command_run(std::vector<std::string> instuct, std::vector<int>& offset, int indx) {
    std::string instruct_name = instuct[0];
    int last_elem_size = offset[offset.size() - 1];
    if (!instruct_name.compare("MOV"))
        mov_command(instuct, offset, indx);
    else if(!instruct_name.compare("ADD"))
        add_command(instuct, offset, indx);
    else if (!instruct_name.compare("AND"))
        and_command(instuct, offset, indx);
    else if(!instruct_name.compare("OR"))
        or_command(instuct, offset, indx);
    else if (!instruct_name.compare("STOS"))
        stos_command(instuct, offset, indx);
    else if (!instruct_name.compare("WAIT"))
        offset.push_back(last_elem_size + 1);
    else if(!instruct_name.compare("XCHG"))
        xchg_command(instuct, offset, indx);
    else if(!instruct_name.compare("RETN"))
        offset.push_back(last_elem_size + 1);
    
}

void mov_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    auto it = lexems.find(instr[1]);
    int last_elem_size = offset[offset.size() - 1];
    if (it != lexems.end()) {
        std::string tmp = it->second;
        auto found = tmp.find("REGISTR 16-BIT");
        if (found != std::string::npos) {
            offset.push_back(last_elem_size + 4);
        } else {
            offset.push_back(last_elem_size + 2);
        }
    }
}

void add_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    auto it = lexems.find(instr[1]);
    int last_elem_size = offset[offset.size() - 1];
    if (it != lexems.end()) {
        std::string tmp = it->second;
        auto found = tmp.find("REGISTR 16-BIT");
        if (found != std::string::npos)
            // For 16 bit registre
            offset.push_back(last_elem_size + 3);
        else
            // For 8 bit registre
            offset.push_back(last_elem_size + 2);
    }
}

void and_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    // And is 2 byte in offset
    // For db user_id to offset 7
    // For dw user_id to offset 8
    // For db user_id[] to offset 6
    // For dw user_id[] to offset 7
    // For db ds: to offset 4
    // For dw ds: to offset 4
    int last_elem_size = offset[offset.size() - 1];
    auto it = lexems.find(instr[0]);
    for (int i = 1; i < lexems.size(); ++i) {
        it = lexems.find(instr[i]);
    }
    if (it != lexems.end()) {
        std::string tmp = it->second;
        auto found_one = tmp.find("REGISTR 16-BIT");
        auto found_two = tmp.find("REGISTR 8-BIT");
        if (found_one != std::string::npos) {
            
        } else if(found_two != std::string::npos) {
            
        }
    }
}

void or_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    
}

void stos_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    
}

void xchg_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    
}

void jbe_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    
}
