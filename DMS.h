/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DMS.h
 * Author: Shiva Shankar Ganesan
 */

#ifndef DMS_H
#define DMS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Contact {
public:
    std::string name;
    enum Type{PERSON,BUSINESS};
    Contact::Type t;
    void Display(std::ostringstream *oss);
    Contact(std::string name,Type personOrBusiness){
        this->name=name;
        t=personOrBusiness;
        
    };
    
    std::string getName();
       
};

class PersonContact: public Contact {
public:
    enum Gender{MALE,FEMALE};
    PersonContact::Gender gen;
    void Display(std::ostringstream *oss);
    PersonContact(std::string name,Contact::Type contactType,Gender maleOrFemale):Contact(name,contactType){
        gen=maleOrFemale;
    };
    
    PersonContact::Gender getGender();
    
    static Gender convertStringToGender(std::string str);
    
          
};

class PersonAddressContact: public PersonContact {
public:
    std::string street;
    std::string city;
    std::string state;
    std::string zipCode;
    void Display(std::ostringstream *oss);
    PersonAddressContact(std::string name,Type contactType,Gender maleOrFemale,std::string street,
    std::string city,std::string state,std::string zipCode):PersonContact(name,contactType,maleOrFemale){
        this->street=street;
        this->city=city;
        this->state=state;
        this->zipCode=zipCode;
    };
};

class PersonPhoneContact: public PersonContact {
public:
    std::vector<std::string> phoneNumbers;
    void Display(std::ostringstream *oss);
    PersonPhoneContact(std::string name,Type contactType,Gender maleOrFemale,std::vector<std::string> phoneNumbers)
            :PersonContact(name,contactType,maleOrFemale){
        for (int i=0; i<phoneNumbers.size();i++) {
            this->phoneNumbers.push_back(phoneNumbers.at(i));
        }
    };

};

class PersonEmailContact: public PersonContact {
public:
    std::vector<std::string> emailAddresses;
    void Display(std::ostringstream *oss);
    PersonEmailContact(std::string name,Type contactType,Gender maleOrFemale,std::vector<std::string> emailAddresses):PersonContact(name,contactType,maleOrFemale){
        for (int i=0; i<emailAddresses.size();i++) {
            this->emailAddresses.push_back(emailAddresses.at(i));
        }
    };
};

class BusinessContact: public Contact  {
public:
    std::string businessType;
    BusinessContact(std::string businessName,Contact::Type contactType,std::string businessType):Contact(businessName,contactType){
        this->businessType=businessType;
    };
    void Display(std::ostringstream *oss);
};

class BusinessAddressContact: public BusinessContact {
public:
    std::string street;
    std::string city;
    std::string state;
    std::string zipCode;
    void Display(std::ostringstream *oss);
    BusinessAddressContact(std::string businessName,Type contactType,std::string businessType,std::string street,
    std::string city,std::string state,std::string zipCode):
    BusinessContact(businessName,contactType,businessType){
        this->street=street;
        this->city=city;
        this->state=state;
        this->zipCode=zipCode;
        
    };
  
};

class BusinessPhoneContact: public BusinessContact {
public:
    std::vector<std::string> phoneNumbers;
    void Display(std::ostringstream *oss);
    BusinessPhoneContact(std::string businessName,Type contactType,std::string businessType,std::vector<std::string> phoneNumbers)
        :BusinessContact(businessName,contactType,businessType){
        for (int i=0; i<phoneNumbers.size();i++) {
            this->phoneNumbers.push_back(phoneNumbers.at(i));
        }
    };

};

class BusinessWebContact: public BusinessContact {
public:
    std::vector<std::string> emailAddresses; 
    std::vector<std::string> websiteAddresses;
    void Display(std::ostringstream *oss);
    BusinessWebContact(std::string businessName,Type contactType,std::string businessType,std::vector<std::string> emailAddresses,std::vector<std::string> websiteAddresses):
    BusinessContact(businessName,contactType,businessType){
        for (int i=0; i<emailAddresses.size();i++) {
            this->emailAddresses.push_back(emailAddresses.at(i));
        }
        for (int i=0; i<websiteAddresses.size();i++) {
            this->websiteAddresses.push_back(websiteAddresses.at(i));
        }
    };
};

class DMS{
    
private:
    std::vector<Contact *> phoneVector;
    std::vector<Contact *> emailVector;
    std::vector<Contact *> addressVector;

    std::vector<Contact *> businessPhoneVector;
    std::vector<Contact *> businessEmailVector;
    std::vector<Contact *> businessAddressVector;

    std::vector<std::string> parsingFunction(std::string line);
    bool isFilePersonContact(std::string fileName);
    void populatePhoneNumbers(const std::vector<std::string>& vect, std::vector<std::string>& phoneNumbers);
    void populateEmailAddresses(const std::vector<std::string>& vect, std::vector<std::string>& emailAddresses);
    void populateWebsites(const std::vector<std::string>& vect, std::vector<std::string>& websiteAddresses);
    void populateAddress(int index, const std::vector<std::string>& vect, std::string &street, std::string &city, std::string &state, std::string &pincode);
    bool isPhoneNumber(std::string number);
    bool isEmailAddress(std::string input);
    bool isWebsite(std::string input);
    std::string getStateInTwoLetters(std::string state);
    bool checkIfNameIsSame(std::string givenName,std::string contactObjectName);
    std::string trim(const std::string& str);
    std::string capitalizeFirstLetter(std::string input);
    void addToMap(std::map<std::string, int> &stateAndCount, int& count,std::string state);
    void displayPerson(std::string name);
    void displayBusiness(std::string name);

    void populatePersonPhoneContact(const std::vector<std::string>  &vect,std::vector<Contact *>& phoneVector);
    void populatePersonAddressContact(const std::vector<std::string>& vect, std::vector<Contact *>& phoneVector);
    void populatePersonEmailContact(const std::vector<std::string>& vect, std::vector<Contact *>& phoneVector);

    void populateBusinessPhoneContact(const std::vector<std::string>& vect, std::vector<Contact *>& phoneVector);
    void populateBusinessAddressContact(const std::vector<std::string>& vect, std::vector<Contact *>& addressVector);
    void populateBusinessWebContact(const std::vector<std::string>& vect, std::vector<Contact *>& emailVector);
public:
    std::ostringstream *oss;
    DMS(std::ostringstream *ossp) {oss = ossp;}
    void populateDirectory(std::string singleLine);

    void display(std::string name,bool isPersonType);
    void findPersonByName(std::string name); 
    void findPersonByEmailDomain(std::string emailDomain);
    void findBusinessByPhonePrefix(std::string phonePrefix);
    void findBusinessByWebDomain(std::string webDomain);

    const std::vector<Contact *> & getPersonPhoneVector();
    const std::vector<Contact *> & getBusinessPhoneVector();
};  



#endif /* DMS_H */

