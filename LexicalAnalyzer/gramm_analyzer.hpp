//
//  gramm_analyzer.hpp
//  LexicalAnalyzer
//
//  Created by Ivan on 4/13/16.
//  Copyright © 2016 Ivan Tkachenko. All rights reserved.
//

#ifndef gramm_analyzer_hpp
#define gramm_analyzer_hpp

#include "stdafx.h"
#include "common_data.h"

enum instructions {MOV, ADD, AND, OR, XCHG, JBE, WAIT, RETN, STOS};

struct label_type{
    std::string name;
    int offset_number;
};

struct user_id_type {
    std::string name;
    std::string type;
    std::string segment_name;
    int value_in_offset;
};

void gram_analyze(std::vector<std::vector<std::string>>&);

bool segment_check(std::vector<std::vector<std::string>>& vector, std::vector<user_id_type>& usr_id, std::vector<int>& offset, int index);
bool mnem_check(std::vector<std::string>& vector, std::vector<user_id_type>& usr_id, std::vector<int>& offset, int indx);
void command_run(std::vector<std::string> instuct, std::vector<int>& offset, int indx, std::vector<label_type> lab_cont);

bool id_check(std::string str);
bool valid_segment_name(std::vector<std::string> sentence, std::vector<user_id_type>& use_id);
bool label_check(std::vector<std::string> label, std::vector<int>& offset, std::vector<label_type>& lab_cont, std::vector<user_id_type>& use_id);


void mov_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void add_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void and_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void or_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void stos_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void xchg_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void jbe_command(std::vector<std::string> instr, std::vector<int>& offset, int indx, std::vector<label_type> lab_cont);

void information_output(std::vector<user_id_type> user_id, std::ofstream& file_to_write);


#endif /* gramm_analyzer_hpp */
