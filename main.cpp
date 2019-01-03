/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Shiva Shankar Ganesan
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>
#include "DMS.h"
#include "LoadBalancer.h"

 // Processing times for different queries (between 3 to 10 time units)
int processing_times[6] = {4, 5, 5, 8, 2, 2};

std::ostringstream oss;

void get_user_input(int &file_count, std::string **file_names, int &no_of_servers, int &no_of_queries) {
    std::cout<<"Enter the number of input files: "<<std::endl;
    std::cin>>file_count;

    *file_names = new std::string[file_count];

    std::cout<<"Enter the file names: "<<std::endl;
    for(int i = 0; i < file_count; i++){
        std::cin>>(*file_names)[i];
    }

    std::cout<<"Enter the number of servers: "<<std::endl;
    std::cin>>no_of_servers;

    std::cout<<"Enter the number of queries: "<<std::endl;
    std::cin>>no_of_queries;
}

void generate_query_queue(std::queue<Query*> &queryQueue, int no_of_queries) {
    srand((int)time(0));
    // Generate random queries from 1 to 6
    oss<<"Queries to be executed: ";
    for (int i=0; i < no_of_queries; i++) {
        int query_num = (rand() % 6)+1;
        Query *thisQuery = new Query(query_num);
        queryQueue.push(thisQuery);
        oss<<query_num<<" ";
    }
    oss<<std::endl;
}

void print_status(TimingWheel &TW) {
     oss<<std::endl;
     oss<<"Current Slot: "<<TW.get_curr_slot()<<std::endl;
     oss<<"Slot Schedule:"<<std::endl;
     oss<<"--------------"<<std::endl;
     Partition** slot = TW.get_slot_info();
     for(int i=0; i < MAX_DELAY; i++) {
        oss<<"Slot "<<i<<": ";
        Partition* cur_partition = slot[i];
        while(cur_partition != NULL) {
            oss<<"{Query: "<<cur_partition->getQuery()->getQueryNum();
            oss<<", Server: "<<cur_partition->getServerNum()<<"}";
            if (cur_partition->getNextPartition() != NULL) {
                oss<<", ";
            }
            cur_partition = cur_partition->getNextPartition();
        }
        oss<<std::endl;
     }
     oss<<std::endl;
}

void print_final_statistics(DMS &dms, TimingWheel &TW, int no_of_queries, int total_simulation_time) {
    //std::map<int, int> &serverStats = TW.getServerStats();
    //std::map<int, int> &queryStats = TW.getQueryStats();
    oss<<"================"<<std::endl;
    oss<<"Final Statistics"<<std::endl;
    oss<<"================"<<std::endl;
    oss<<"Number of Person Contacts: "<<dms.getPersonPhoneVector().size()<<std::endl;
    oss<<"Number of Business Contacts: "<<dms.getBusinessPhoneVector().size()<<std::endl;
    oss<<"Total Number of Queries: "<<no_of_queries<<std::endl;
    oss<<std::endl;
    oss<<"Server Stats (Server Num: Number of Queries Executed)"<<std::endl;
    oss<<"------------"<<std::endl;
    for (auto elem : TW.getServerStats()) {
        oss<<elem.first<<": "<<elem.second<<std::endl;
    }
    oss<<std::endl;
    oss<<"Query Stats (Query Num: Number of Times Executed)"<<std::endl;
    oss<<"-----------"<<std::endl;
    for (auto elem : TW.getQueryStats()) {
        oss<<elem.first<<": "<<elem.second<<std::endl;
    }
    oss<<std::endl;
    oss<<"Total Simulation Time = "<<total_simulation_time<<std::endl;
}

void part_two() {
    DMS dms(&oss);
    int file_count;
    char response;
    std::cout<<"Enter the number of input files: "<<std::endl;
    std::cin>>file_count;
    
    std::string *file_names = new std::string[file_count];
    
    std::cout<<"Enter the file names: "<<std::endl;
    for(int i = 0; i < file_count; i++){
        std::cin>>file_names[i];
        try {
            dms.populateDirectory(file_names[i]);
        } catch(std::exception e) {
            std::cout<<"Unable to read from file! "<<file_names[i];
        }
    }
    std::cin.ignore();

    std::string choice = "";
    std::string name="";
    std::string personName;
    std::string organizationName;
    
    
   
    while (choice != "exit") {
        std::cout<<"Choose one of the following options or enter 'exit':"<<std::endl;
        std::cout<<"1. Find the number of person in the directory ordered by state"<<std::endl;
        std::cout<<"2. Find the number of people in the directory whose email domain is '.edu' ordered by the gender."<<std::endl;
        std::cout<<"3. Find the number of organizations in the directory whose phone number start with the area code '203' ordered by the organization category.."<<std::endl;
        std::cout<<"4. Find the number of organizations in the directory with a '.com' email or a '.com' website ordered by the organization category."<<std::endl;
        std::cout<<"5. Display the details of an organization"<<std::endl;
        std::cout<<"6. Display the details of a person"<<std::endl;
        std::cout<<"Enter Your Choice: ";
        std::cin>>choice;
        if (choice == "1") {
            std::cin.ignore();
            std::cout<<"Enter the Name of the Person";
            std::getline(std::cin, name);
            dms.findPersonByName(name);
        } else if (choice == "2") {
            dms.findPersonByEmailDomain(".edu");
        } else if (choice == "3") {
            dms.findBusinessByPhonePrefix("203");
        } else if (choice == "4") {
            dms.findBusinessByWebDomain(".com");
        }else if (choice == "5") {
            std::cin.ignore();
            std::cout<<"Enter the organization name";
            std::getline(std::cin, organizationName);
            dms.display(organizationName, false);
        } else if (choice == "6") {
            std::cin.ignore();
            std::cout<<"Enter the name"<<std::endl;
            std::getline(std::cin, personName);
            dms.display(personName, true);
        }
        std::cout<<oss.str()<<std::endl;
        oss.str("");
    }
}

void part_four() {
    std::ofstream ofile;
    ofile.open ("output.txt", std::ofstream::out);
    DMS dms(&oss);
    int file_count = 0, no_of_servers = 0, no_of_queries = 0;
    char response;
    
    std::string *file_names;

    get_user_input(file_count, &file_names, no_of_servers, no_of_queries);

    for(int i = 0; i < file_count; i++){
        try {
            dms.populateDirectory(file_names[i]);
        } catch(std::exception e) {
            std::cout<<"Unable to read from file! "<<file_names[i];
        }
    }

    std::queue<Query*> queryQueue;
    generate_query_queue(queryQueue, no_of_queries);

    TimingWheel TW(&queryQueue); 

    int serversUsed = std::min(no_of_servers, no_of_queries);
    for (int i=0; i < serversUsed; i++) {
        TW.insert(processing_times[queryQueue.front()->getQueryNum() - 1], i, queryQueue.front());
        queryQueue.pop();
    }

    print_status(TW);
    int total_simulation_time = 0;

    while (!queryQueue.empty() || TW.noOfPartitions != 0) {   
        TW.schedule(dms);
        TW.clear_curr_slot();
        TW.inc_curr_slot();
        print_status(TW);
        total_simulation_time++;
    }
    print_final_statistics(dms, TW, no_of_queries, total_simulation_time);
    std::cout<<oss.str()<<std::endl;
    ofile<<oss.str();
    ofile.close();
}

int main() {
 part_two();
 //part_four();
}
