/*
 * File:   LoadBalancer.cpp
 * Author: Shiva Shankar Ganesan
 */

#include "LoadBalancer.h"
#include <sstream>

Query::Query(int query_num) {
    this->query_num = query_num;
}

void Query::setQueryNum(int query_num) {
	this->query_num = query_num;
}

int Query::getQueryNum() {
	return query_num;
}

void Query::executeQuery(DMS &dms) {
    if (query_num == 1) {
        *(dms.oss)<<"Executing Query 1: "<<std::endl;
        dms.findPersonByName("John");
    } else if (query_num == 2) {
        *(dms.oss)<<"Executing Query 2: "<<std::endl;
        dms.findPersonByEmailDomain(".edu");
    } else if (query_num == 3) {
        *(dms.oss)<<"Executing Query 3: "<<std::endl;
        dms.findBusinessByPhonePrefix("203");
    } else if (query_num == 4) {
        *(dms.oss)<<"Executing Query 4: "<<std::endl;
        dms.findBusinessByWebDomain(".com");
    }else if (query_num == 5) {
        *(dms.oss)<<"Executing Query 5: "<<std::endl;
        dms.display("Sed LLP", false);
    } else if (query_num == 6) {
        *(dms.oss)<<"Executing Query 6: "<<std::endl;
        dms.display("Freya Mcdaniel", true);
    }
}

Partition::Partition(int server_num, Query *query) {
	this->query = query;
	this->server_num = server_num;
	this->nextp = NULL;
}

void Partition::setQuery(Query *query) {
	this->query = query;
}

Query* Partition::getQuery() {
	return query;
}

void Partition::setServerNum(int server_num) {
	this->server_num = server_num;
}

int Partition::getServerNum() {
	return server_num;
}

void Partition::setNextPartition(Partition *nextp) {
	this->nextp = nextp;
}

Partition* Partition::getNextPartition() {
	return nextp;
}

TimingWheel::TimingWheel(std::queue<Query*> *queryQueue) {
	this->current_slot = 0;
	for (int i=0; i<MAX_DELAY; i++) {
		slot[i] = NULL;
	}
	this->queryQueue = queryQueue;
    this->noOfPartitions = 0;
}

void TimingWheel::insert(int processing_time, int server_num, Query *query) {
	Partition *partition = new Partition(server_num, query);

	int slotToUse = (current_slot + processing_time) % MAX_DELAY;
	if (slot[slotToUse] == NULL) {
		slot[slotToUse] = partition;
	} else {
		Partition *curPartition = slot[slotToUse];
		while (curPartition->getNextPartition() != NULL) {
			curPartition = curPartition->getNextPartition();
		} 
		curPartition->setNextPartition(partition);
	}
    this->noOfPartitions++;
    if (serverStats.count(server_num) == 0) {
	serverStats.insert(std::make_pair(server_num, 0));
    }
    if (queryStats.count(query->getQueryNum()) == 0) {
	queryStats.insert(std::make_pair(query->getQueryNum(), 0));
    }
}

void TimingWheel::clear_curr_slot() {
    Partition *curPartition = slot[current_slot];
    if (curPartition == NULL) {
    	return;
    }

    while (curPartition != NULL) {
    	Partition *tempPartition = curPartition->getNextPartition();
        delete curPartition->getQuery();
    	delete curPartition;
    	curPartition = tempPartition;
        this->noOfPartitions--;
    }
    slot[current_slot] = NULL;
}

void TimingWheel::schedule(DMS &dms) {
    Partition *curPartition = slot[current_slot];
    if (curPartition == NULL) {
    	return;
    }

    while (curPartition != NULL) {
    	// Execute the query
    	curPartition->getQuery()->executeQuery(dms);
    	// Update the server and query stats
    	std::map<int, int>::iterator it1 = serverStats.find(curPartition->getServerNum());
    	it1->second++;
    	std::map<int, int>::iterator it2 = queryStats.find(curPartition->getQuery()->getQueryNum());
    	it2->second++;

    	// Get next element from the query queue and assign this server to next slot based on proessing time
        if (!queryQueue->empty()) {
            insert(processing_times[curPartition->getQuery()->getQueryNum() - 1], curPartition->getServerNum(), queryQueue->front());
            queryQueue->pop();
        }
        curPartition = curPartition->getNextPartition();
    }

}

Partition** TimingWheel::get_slot_info() {
	return slot;
}

int TimingWheel::get_curr_slot() {
	return current_slot;
}

void TimingWheel::inc_curr_slot() {
	current_slot = (current_slot + 1) % MAX_DELAY;
}

const std::map<int, int> & TimingWheel::getServerStats() const {
	return serverStats;
}

const std::map<int, int> & TimingWheel::getQueryStats() const {
	return queryStats;
}
