/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LoadBalancer.h
 * Author: Shiva Shankar Ganesan
 */

#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "DMS.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>

#define MAX_DELAY 10
extern int processing_times[6];

class Query {

private:
    int query_num;

public:
    Query(int query_num);
    ~Query() {}
    void setQueryNum(int query_num);
    int getQueryNum();
    void executeQuery(DMS &dms);
};

class Partition{
    
private:
    Query *query;
    int server_num;
    Partition *nextp;

public:
    Partition(int server_num, Query *query);
    void setQuery(Query *query);
    Query* getQuery();
    void setServerNum(int server_num);
    int getServerNum();
    void setNextPartition(Partition *nextp);
    Partition* getNextPartition();
};  

class TimingWheel {

private:
    std::queue<Query*> *queryQueue;
    Partition *slot[MAX_DELAY];
    int current_slot;
    // Key: Server num, val: numberOfQueries
    std::map<int, int> serverStats;
    // Key: Query num, val: numberOfTimesExecuted
    std::map<int, int> queryStats;

public:
    int noOfPartitions;
    TimingWheel(std::queue<Query*> *queryQueue);
    void insert(int processing_time, int server_num, Query *query);
    void schedule(DMS& dms);
    void clear_curr_slot();
    Partition** get_slot_info();
    int get_curr_slot();
    void inc_curr_slot();
    const std::map<int, int> & getServerStats() const;
    const std::map<int, int> & getQueryStats() const;
};


#endif /* LOAD_BALANCER_H */
