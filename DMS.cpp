/*
 * File:   DMS.cpp
 * Author: Shiva Shankar Ganesan
 */

#include "DMS.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>
#include <regex>
#include <locale>
#include <iomanip>

PersonContact::Gender PersonContact::convertStringToGender(std::string str){
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    if(str=="MALE") return PersonContact::MALE;
    else return PersonContact::FEMALE;
}

void DMS::populateDirectory(std::string fileName){
    
    bool isFilePersonType=DMS::isFilePersonContact(fileName);
    std::vector<std::vector<Contact *> > finalVector;
   
    std::ifstream inFile(fileName);
    std::string eachLine;
    if(inFile.fail()){
        std::cout<<"Your file didn't work";
    }else{
        int i=0;
            while(!inFile.eof() && std::getline(inFile,eachLine)){
               // std::cout<<eachLine<<std::endl;
                std::vector<std::string> vect=DMS::parsingFunction(eachLine);
                if(i!=0 && isFilePersonType){
                    //std::cout<<"========================"<<std::endl;
                    //std::cout<<"Person: " <<i<<std::endl;
                    DMS::populatePersonPhoneContact(vect,phoneVector);
                    DMS::populatePersonEmailContact(vect,emailVector);
                    DMS::populatePersonAddressContact(vect,addressVector);
                    //std::cout<<"========================"<<std::endl;
                }else if(i!=0 && !isFilePersonType){
                    //std::cout<<"========================"<<std::endl;
                    //std::cout<<"Business: " <<i<<std::endl;
                    DMS::populateBusinessPhoneContact(vect,phoneVector);
                    DMS::populateBusinessWebContact(vect,emailVector);
                    DMS::populateBusinessAddressContact(vect,addressVector);
                    //std::cout<<"========================"<<std::endl;
                }
                i++;
            }
        //std::cout<<i;
    }
    
}

bool DMS::isFilePersonContact(std::string fileName){

    bool isFilePerson=false;
    std::string people="people";
    std::string sLine;
    std::ifstream inFile(fileName);
    std::vector<std::string> inputs;
    int i=0;
    if(inFile.good())
    {
        std::getline(inFile,sLine);
    }
    
    inputs=DMS::parsingFunction(sLine);
    if(inputs.at(1).compare(people) == 0) {
        isFilePerson=true;
    }
    inFile.close();
    return isFilePerson;
}

bool DMS::isPhoneNumber(std::string number) {
    // Ex: 1-651-284-3871
    if (number.size() != 14) {
        return false;
    }

    std::string AllowedChars = "0123456789-";
    for (int i=0; i<number.size(); i++) {
        if ((i==1 || i == 5 || i == 9) && number.at(i) != '-') {
            return false;
        }
        if (AllowedChars.find(number.at(i), 0) == std::string::npos) {
            // The char is not in allowedChars
            return false;
        }
    }

    return true;
}

 void DMS::populatePhoneNumbers(const std::vector<std::string>& vect, std::vector<std::string>& phoneNumbers) {
   for (int i=2; i<vect.size() && phoneNumbers.size() !=  2; i++) {
      if (isPhoneNumber(vect.at(i))) {
          phoneNumbers.push_back(vect.at(i));
      }
   }
 }
 void DMS::populatePersonPhoneContact(const std::vector<std::string>& vect, std::vector<Contact *>& phoneVector){
   std::string name=vect.at(0); 
   std::string gender=vect.at(1);
   std::vector<std::string> phoneNumbers;
   populatePhoneNumbers(vect, phoneNumbers);

   if (phoneNumbers.size() != 0) {
       Contact* personPhoneContact = new PersonPhoneContact(name,Contact::PERSON,PersonContact::convertStringToGender(gender),phoneNumbers);
       // Debugging
       //personPhoneContact->Display();
       //((PersonContact *) personPhoneContact)->Display();
       //((PersonPhoneContact *) personPhoneContact)->Display();
       phoneVector.push_back(personPhoneContact);
   } else {
       phoneVector.push_back(NULL);
   }
}

bool DMS::isEmailAddress(std::string input) {
    size_t pos = 0;
    std::vector<std::string> tokens;
    while ((pos = input.find("@")) != std::string::npos) {
        std::string token = input.substr(0, pos);
        tokens.push_back(token);
        input.erase(0, pos + 1);
    }

    tokens.push_back(input);
    if (tokens.size() != 2) {
        return false;
    }

    std::string AllowedSpecialChars = "_.;";

    std::string first_part = tokens.at(0);
    for(int i=0; i<first_part.size(); i++) {
        if (!std::isalnum(first_part[i]) && AllowedSpecialChars.find(first_part[i],0) == std::string::npos) {
            return false;
        }
    }

    std::string second_part = tokens.at(1);
    // The second part must contain at least one dot
    if (second_part.find(".") == std::string::npos) {
        return false;
    }

    for(int i=0; i<second_part.size(); i++) {
        if (!std::isalnum(second_part[i]) && AllowedSpecialChars.find(second_part[i],0) == std::string::npos) {
            return false;
        }
    }

    return true;
}

void DMS::populateEmailAddresses(const std::vector<std::string>& vect, std::vector<std::string>& emailAddresses) {
   for (int i=2; i<vect.size() && emailAddresses.size() !=  3; i++) {
       if (isEmailAddress(vect.at(i))) {
           emailAddresses.push_back(vect.at(i));
       }
   }
}

 void DMS::populatePersonEmailContact(const std::vector<std::string>& vect, std::vector<Contact *>& emailVector){
   std::string name=vect.at(0); 
   std::string gender=vect.at(1);
   std::vector<std::string> emailAddresses;
   populateEmailAddresses(vect, emailAddresses);

   if (emailAddresses.size() != 0) {
       Contact* personEmailContact= new PersonEmailContact(name,Contact::PERSON,PersonContact::convertStringToGender(gender),emailAddresses);
       // Debugging
       //((PersonEmailContact *) personEmailContact)->Display();
       emailVector.push_back(personEmailContact);
   } else {
       emailVector.push_back(NULL);
   }
 }

 void DMS::populateAddress(int index, const std::vector<std::string>& vect, std::string &street, std::string &city, std::string &state, std::string &pinCode) {

    if (index != 0 && (vect.size() - index) >= 4) {
        std::string tempStreet = "";
        for (int i=index; i<= vect.size() - 4; i++) {
            tempStreet = tempStreet + vect.at(i);
            if (i != vect.size() - 4) {
                tempStreet = tempStreet + ",";
            }
        }
        street=tempStreet;
        city=vect.at(vect.size() - 3);
        state=getStateInTwoLetters(vect.at(vect.size() - 2));
        pinCode=vect.at(vect.size() - 1);
    }
 }
 
 void DMS::populatePersonAddressContact(const std::vector<std::string>& vect, std::vector<Contact *>& addressVector){
    std::string name=vect.at(0); 
    std::string gender=vect.at(1);

    std::string street="";
    std::string city="";
    std::string state="";
    std::string pinCode="";

    int index = 0;
    for (int i=2; i<vect.size(); i++) {
        if (!isPhoneNumber(vect.at(i)) && !isEmailAddress(vect.at(i))) {
            index = i;
            break;
        }
    }

    populateAddress(index, vect, street, city, state, pinCode);
    if (street != "" && city != "" && state != "" && pinCode != "") {
        Contact* personAddressContact= new PersonAddressContact(name,Contact::PERSON,PersonContact::convertStringToGender(gender),street,city,state,pinCode);
        // Debugging
        //((PersonAddressContact *) personAddressContact)->Display();
        addressVector.push_back(personAddressContact);
    } else {
        addressVector.push_back(NULL);
    }
 }
 
 void DMS::populateBusinessPhoneContact(const std::vector<std::string>& vect, std::vector<Contact *>& phoneVector){
     std::string name=vect.at(0);
     std::string businessType=capitalizeFirstLetter(vect.at(1));
     std::vector<std::string> phoneNumbers;
     populatePhoneNumbers(vect, phoneNumbers);

     if (phoneNumbers.size() != 0) {
        Contact* businessPhoneContact = new BusinessPhoneContact(name,Contact::BUSINESS,businessType,phoneNumbers);
        businessPhoneVector.push_back(businessPhoneContact);
       // Debugging
       //businessPhoneContact->Display();
       //((BusinessContact *) businessPhoneContact)->Display();
       //((BusinessPhoneContact *) businessPhoneContact)->Display();
     } else {
        businessPhoneVector.push_back(NULL);
     }
 }

 bool DMS::isWebsite(std::string input) {
    size_t pos = 0;
    std::vector<std::string> tokens;
    while ((pos = input.find(".")) != std::string::npos) {
        std::string token = input.substr(0, pos);
        tokens.push_back(token);
        input.erase(0, pos + 1);
    }

    tokens.push_back(input);
    if (tokens.size() <= 1) {
        return false;
    }

    // Reference: https://en.wikipedia.org/wiki/Percent-encoding#Types_of_URI_characters
    std::string AllowedSpecialChars = "!*'();:&=+$,/?#[]%";

    for(int i=0; i<tokens.size(); i++) {
        std::string thisToken = tokens.at(i);
        for (int j=0; j<thisToken.size(); j++) {
            if (!std::isalnum(thisToken[j]) &&
                AllowedSpecialChars.find(thisToken[j], 0) == std::string::npos) {
                return false;
            }
        }
    }

    return true;
}

void DMS::populateWebsites(const std::vector<std::string>& vect, std::vector<std::string>& websiteAddresses) {
   for (int i=2; i<vect.size(); i++) {
       if (isWebsite(vect.at(i))) {
           websiteAddresses.push_back(vect.at(i));
       }
   }
}
 
 void DMS::populateBusinessWebContact(const std::vector<std::string>& vect, std::vector<Contact *>& emailVector){
      std::string name=vect.at(0);
      std::string businessType=capitalizeFirstLetter(vect.at(1));
      std::vector<std::string> websiteAddresses;
      std::vector<std::string> emailAddresses;
      populateEmailAddresses(vect, emailAddresses);
      populateWebsites(vect, websiteAddresses);
      if (websiteAddresses.size() !=0 || emailAddresses.size() != 0) {
          Contact* businessWebContact = new BusinessWebContact(name,Contact::BUSINESS,businessType,emailAddresses,websiteAddresses);
          businessEmailVector.push_back(businessWebContact);
          // Debugging
          //((BusinessWebContact *) businessWebContact)->Display();
      } else {
          businessEmailVector.push_back(NULL);
      }
      
 }
 
 void DMS::populateBusinessAddressContact(const std::vector<std::string>& vect, std::vector<Contact *>& addressVector){
     std::string name=vect.at(0);
     std::string businessType=capitalizeFirstLetter(vect.at(1));
     std::string street="";
     std::string city="";
     std::string state="";
     std::string pinCode="";

     int index = 0;
     for (int i=2; i<vect.size(); i++) {
         if (!isPhoneNumber(vect.at(i)) && !isEmailAddress(vect.at(i)) && !isWebsite(vect.at(i))) {
             index = i;
             break;
         }
     }

     populateAddress(index, vect, street, city, state, pinCode);
     if (street != "" && city != "" && state != "" && pinCode != "") {
        Contact* businessAddressContact = new BusinessAddressContact(name,Contact::BUSINESS,businessType,street,city,state,pinCode);
        businessAddressVector.push_back(businessAddressContact);
        // Debugging
        //((BusinessAddressContact *) businessAddressContact)->Display();
     } else {
        businessAddressVector.push_back(NULL);
     }
 }

 std::string DMS::capitalizeFirstLetter(std::string input) {
    size_t pos = 0;
    std::transform(input.begin(), input.end(),input.begin(), ::toupper);
    std::transform(input.begin(), input.end(),input.begin(), ::tolower);
    std::vector<std::string> tokens;
    while ((pos = input.find(" ")) != std::string::npos) {
        std::string token = input.substr(0, pos);
        tokens.push_back(token);
        input.erase(0, pos + 1);
    }

    tokens.push_back(input);
    std::string finalString = "";
    for (int i=0; i<tokens.size(); i++) {
       // Transform the first letter alone
       std::transform(tokens.at(i).begin(), tokens.at(i).begin()+1, tokens.at(i).begin(), ::toupper);
       finalString += tokens.at(i);
       if (i+1 != tokens.size()) {
          finalString += " ";
       }
    }
    return finalString;
 }

std::string DMS::trim(const std::string& str)
{
    const std::string whitespace = " \t";
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


std::vector<std::string> DMS::parsingFunction(std::string line){
        std::vector<std::string> inputs;
        std::istringstream ss (line);
            std::string temp;
            while(std::getline(ss,temp,',')){
                inputs.push_back(trim(temp));
            }
            return inputs;
}

void DMS::displayPerson(std::string name) {
    //std::cout<<"Person name typed"<<name;
   std::vector<Contact *>::iterator it;
   bool nameDisplayed = false;
   for(it=emailVector.begin();it!=emailVector.end(); ++it){
       if (*it != NULL) {
           if((((PersonEmailContact *)*it)->getName())==name){
               ((Contact *)*it)->Display(oss);
               ((PersonContact *)*it)->Display(oss);
               nameDisplayed = true;
               ((PersonEmailContact *)*it)->Display(oss);
           }
       }
   }
  
    for(it=phoneVector.begin();it!=phoneVector.end(); ++it){
       if (*it != NULL) {
           if((((PersonPhoneContact *)*it)->getName())==name){
              if (!nameDisplayed) {
                  ((Contact *)*it)->Display(oss);
                  ((PersonContact *)*it)->Display(oss);
                  nameDisplayed = true;
              }
              ((PersonPhoneContact *)*it)->Display(oss);
           }
       }
    }
    for(it=addressVector.begin();it!=addressVector.end(); it++){
       if (*it != NULL) {
           if((((PersonAddressContact *)*it)->getName())==name){
              if (!nameDisplayed) {
                  ((Contact *)*it)->Display(oss);
                  ((PersonContact *)*it)->Display(oss);
              }
              ((PersonAddressContact *)*it)->Display(oss);
           }
       }
    }
    (*oss)<<std::endl;
}

void DMS::displayBusiness(std::string name) {
   std::vector<Contact *>::iterator it;
   bool nameDisplayed = false;
   for(it=businessEmailVector.begin();it!=businessEmailVector.end(); ++it){
       if (*it != NULL) {
           if((((BusinessWebContact *)*it)->getName())==name){
               ((Contact *)*it)->Display(oss);
               ((BusinessContact *)*it)->Display(oss);
               nameDisplayed = true;
               ((BusinessWebContact *)*it)->Display(oss);
           }
       }
   }
  
    for(it=businessPhoneVector.begin();it!=businessPhoneVector.end(); ++it){
       if (*it != NULL) {
           if((((BusinessPhoneContact *)*it)->getName())==name){
              if (!nameDisplayed) {
                  ((Contact *)*it)->Display(oss);
                  ((BusinessContact *)*it)->Display(oss);
                  nameDisplayed = true;
              }
              ((BusinessPhoneContact *)*it)->Display(oss);
           }
       }
    }
    for(it=businessAddressVector.begin();it!=businessAddressVector.end(); it++){
       if (*it != NULL) {
           if((((BusinessAddressContact *)*it)->getName())==name){
              if (!nameDisplayed) {
                  ((Contact *)*it)->Display(oss);
                  ((BusinessContact *)*it)->Display(oss);
              }
              ((BusinessAddressContact *)*it)->Display(oss);
           }
       }
    }
    (*oss)<<std::endl;
}

/*Displays the details of the person/organisation. 
 *  For person- Displays name,gender,email,phone number, Address. So I have to access all the three object. Address, Phone, Email object.
 *  I have to iterate through the vector and check if the name matches. 
 */

void DMS::display(std::string name,bool isPersonType){
    if (isPersonType) {
        displayPerson(name);
    } else {
        displayBusiness(name);
    }
}

  /* Order by state
     * use only address vector 
     * Use hashmap to keep track number of john in each state
     */

void DMS::findPersonByName(std::string givenName){
    std::vector<Contact *>::iterator it;
    int number=1;
    std::map<std::string, int> stateAndCout;
    
    
     for(it=addressVector.begin();it!=addressVector.end(); it++){
         if (*it == NULL) {
             continue;
         }
         std::string contactObjectName=((PersonAddressContact *)*it)->getName();
         if(DMS::checkIfNameIsSame(givenName,contactObjectName)){
             std::string state=((PersonAddressContact *)*it)->state;
             DMS::addToMap(stateAndCout,number,state);
         }
     }
    
    //Display the map
    (*oss)<<"------------------"<<std::endl;
    (*oss)<<"State     | Number"<<std::endl;
    (*oss)<<"------------------"<<std::endl;
    for(auto elem : stateAndCout){
        (*oss) << elem.first << "        | " <<elem.second<<std::endl;
    }
    (*oss)<<"------------------"<<std::endl;
   
}

/* Ordered by gender
     * Use only email vector
     * Don't do exact match
     * Matching a substring can be done in common function
*/
void DMS::findPersonByEmailDomain(std::string emailDomain){
    std::vector<Contact *>::iterator it;
    int number=1;
    std::map<std::string, int> genderAndCount;
    for(it=emailVector.begin();it!=emailVector.end(); it++){
        if (*it == NULL) {
            continue;
        }
        std::vector<std::string> contactObjectEmails=((PersonEmailContact *)*it)->emailAddresses;
        for (int i=0; i<contactObjectEmails.size(); i++) {
            std::string contactObjectEmail = contactObjectEmails.at(i);
            if(contactObjectEmail.find(emailDomain)!=std::string::npos){
                PersonContact::Gender enumGender=((PersonEmailContact *)*it)->gen;
                std::string gender;
                if(enumGender==0){
                    gender="Male";
                } else{
                    gender="Female";
                }
                DMS::addToMap(genderAndCount,number,gender);
                // Even if there are multiple emails for the same person with same domain, we have to count only once
                break;
            }
        }
    }
    
    //Display the map
    (*oss)<<"----------------"<<std::endl;
    (*oss)<<"Gender  | Number"<<std::endl;
    (*oss)<<"----------------"<<std::endl;
    for(auto elem : genderAndCount){
        (*oss) << std::setw(7) << elem.first << " | " <<elem.second<<std::endl;
    }
    (*oss)<<"----------------"<<std::endl;
}

/*
 * Ordered by Category
 * Use only businessPhoneVector
 */
void DMS::findBusinessByPhonePrefix(std::string phonePrefix) {
    std::vector<Contact *>::iterator it;
    int number=1;
    std::map<std::string, int> categoryAndCount;
    
    
     for(it=businessPhoneVector.begin();it!=businessPhoneVector.end(); it++){
         if (*it == NULL) {
             continue;
         }
         std::vector<std::string> contactObjectPhones=((BusinessPhoneContact *)*it)->phoneNumbers;
         for (int i=0; i<contactObjectPhones.size(); i++) {
            std::string contactObjectPhone = contactObjectPhones.at(i);
            if(contactObjectPhone.substr(2, 3) == phonePrefix){
                std::string category=((BusinessPhoneContact *)*it)->businessType;

                DMS::addToMap(categoryAndCount,number,category);
                // Even if there are multiple phones with same prefix, we have to count only once
                break;
            }
         }
     }
    
    //Display the map
    (*oss)<<"------------------------"<<std::endl;
    (*oss)<<"Category        | Number"<<std::endl;
    (*oss)<<"------------------------"<<std::endl;
    for(auto elem : categoryAndCount){
        (*oss) << std::setw(15) << elem.first << " | " <<elem.second<<std::endl;
    }
    (*oss)<<"------------------------"<<std::endl;
}

/*
 * Ordered by Category
 * Use only businessEmailVector
 */
void DMS::findBusinessByWebDomain(std::string webDomain) {
    std::vector<Contact *>::iterator it;
    int number=1;
    std::map<std::string, int> categoryAndCount;
    
    
     for(it=businessEmailVector.begin();it!=businessEmailVector.end(); it++){
         if (*it == NULL) {
             continue;
         }
         bool foundWebDomain = false;
         std::vector<std::string> contactObjectEmails=((BusinessWebContact *)*it)->emailAddresses;
         for (int i=0; i<contactObjectEmails.size(); i++) {
            std::string contactObjectEmail = contactObjectEmails.at(i);
            if(contactObjectEmail.find(webDomain)!=std::string::npos){
                std::string category=((BusinessWebContact *)*it)->businessType;

                DMS::addToMap(categoryAndCount,number,category);
                foundWebDomain = true;
                // Even if there are multiple emails with same prefix, we have to count only once
                break;
            }
         }
         if (foundWebDomain) {
            // Domain already found in email address
            continue;
         }
         std::vector<std::string> contactObjectWebsites=((BusinessWebContact *)*it)->websiteAddresses;
         for (int i=0; i<contactObjectWebsites.size(); i++) {
            std::string contactObjectWebsite = contactObjectWebsites.at(i);
            if(contactObjectWebsite.find(webDomain)!=std::string::npos){
                std::string category=((BusinessWebContact *)*it)->businessType;

                DMS::addToMap(categoryAndCount,number,category);
                foundWebDomain = true;
                // Even if there are multiple websites with same prefix, we have to count only once
                break;
            }
         }
     }
    
    //Display the map
    (*oss)<<"---------------------------"<<std::endl;
    (*oss)<<"Category           | Number"<<std::endl;
    (*oss)<<"---------------------------"<<std::endl;
    for(auto elem : categoryAndCount){
        (*oss) << std::setw(18) << elem.first << " | " <<elem.second<<std::endl;
    }
    (*oss)<<"---------------------------"<<std::endl;
}

//Check if already the key exist in the map. If already exists increase the counter
// std::map maintains the keys in sorted order
void DMS::addToMap(std::map<std::string, int>& map, int& number,std::string key){
    if(map.count(key) == 0){ //If key already doesnt exists already 
        map.insert(std::make_pair(key, number));
    }else{
        std::map<std::string, int>::iterator it = map.find(key); 
        if (it != map.end())
        it->second++; 
    }
}

/* Dont do exact matach. Use substring 
     * Convert the search string and contact value to uppercase*/

bool DMS::checkIfNameIsSame(std::string name,std::string contactObjectName){
    bool nameMatches=false;
    std::string givenName=name;
    std::string contactName=contactObjectName;
    std::transform(givenName.begin(), givenName.end(),givenName.begin(), ::toupper);
    std::transform(contactName.begin(), contactName.end(),contactName.begin(), ::toupper);
    if (contactName.find(givenName) != std::string::npos) {
        nameMatches=true;
    }
    return nameMatches;
}

void Contact::Display(std::ostringstream *oss){
    (*oss)<<"Name:         "<<name<<std::endl;
}

void PersonContact::Display(std::ostringstream *oss){
    if (PersonContact::gen == MALE) {
        (*oss)<<"Gender:       Male"<<std::endl;
    } else {
        (*oss)<<"Gender:       Female"<<std::endl;
    }
}

void PersonAddressContact::Display(std::ostringstream *oss){
    (*oss)<<"Address:"<<std::endl;
    (*oss)<<"      Street:"<<street<<std::endl;
    (*oss)<<"      District:"<<city<<std::endl;
    (*oss)<<"      State:"<<state<<std::endl;
    (*oss)<<"      Zip:"<<zipCode<<std::endl;
}

void PersonPhoneContact::Display(std::ostringstream *oss){
    (*oss)<<"Phone number: ";
    for (int i=0; i < phoneNumbers.size(); i++) {
        (*oss)<<phoneNumbers.at(i)<< " ";
    }
    (*oss)<<std::endl;
}

void PersonEmailContact::Display(std::ostringstream *oss){
    (*oss)<<"Email:        ";
    for (int i=0; i < emailAddresses.size(); i++) {
        (*oss)<<emailAddresses.at(i)<< " ";
    }
    (*oss)<<std::endl;
}

void BusinessContact::Display(std::ostringstream *oss){
    (*oss)<<"Category:     "<<businessType<<std::endl;
}

void BusinessAddressContact::Display(std::ostringstream *oss){
    (*oss)<<"Address:"<<std::endl;
    (*oss)<<"      Street:"<<street<<std::endl;
    (*oss)<<"      District:"<<city<<std::endl;
    (*oss)<<"      State:"<<state<<std::endl;
    (*oss)<<"      Zip:"<<zipCode<<std::endl;
}

void BusinessPhoneContact::Display(std::ostringstream *oss){
    (*oss)<<"Phone number: ";
    for (int i=0; i < phoneNumbers.size(); i++) {
        (*oss)<<phoneNumbers.at(i)<< " ";
    }
    (*oss)<<std::endl;
}

void BusinessWebContact::Display(std::ostringstream *oss){
    (*oss)<<"Email:        ";
    for (int i=0; i < emailAddresses.size(); i++) {
        (*oss)<<emailAddresses.at(i)<< " ";
    }
    (*oss)<<std::endl;
    (*oss)<<"Website:      ";
    for (int i=0; i < websiteAddresses.size(); i++) {
        (*oss)<<websiteAddresses.at(i)<< " ";
    }
    (*oss)<<std::endl;
}

std::string Contact::getName(){
    return name;
}

std::string DMS::getStateInTwoLetters(std::string state) {
    if (state.size() == 2) {
        std::transform(state.begin(), state.end(), state.begin(), ::toupper);
        return state; 
    }

    std::transform(state.begin(), state.end(), state.begin(), ::tolower);
    // Reference: https://pe.usps.com/text/pub28/28apb.htm
    if (state == "alabama") {
        return "AL";
    } else if (state == "alaska") {
        return "AK";
    } else if (state == "american samoa") {
        return "AS";
    } else if (state == "arizona") {
        return "AZ";
    } else if (state == "arkansas") {
        return "AR";
    } else if (state == "california") {
        return "CA";
    } else if (state == "colorado") {
        return "CO";
    } else if (state == "connecticut") {
        return "CT";
    } else if (state == "delaware") {
        return "DE";
    } else if (state == "district of columbia") {
        return "DC";
    } else if (state == "federated States of micronesia") {
        return "FM";
    } else if (state == "florida") {
        return "FL";
    } else if (state == "georgia") {
        return "GA";
    } else if (state == "guam") {
        return "GU";
    } else if (state == "hawaii") {
        return "HI";
    } else if (state == "idaho") {
        return "ID";
    } else if (state == "illinois") {
        return "IL";
    } else if (state == "indiana") {
        return "IN";
    } else if (state == "iowa") {
        return "IA";
    } else if (state == "kansas") {
        return "KS";
    } else if (state == "kentucky") {
        return "KY";
    } else if (state == "louisiana") {
        return "LA";
    } else if (state == "maine") {
        return "ME";
    } else if (state == "marshall islands") {
        return "MH";
    } else if (state == "maryland") {
        return "MD";
    } else if (state == "massachusetts") {
        return "MA";
    } else if (state == "michigan") {
        return "MI";
    } else if (state == "minnesota") {
        return "MN";
    } else if (state == "mississippi") {
        return "MS";
    } else if (state == "missouri") {
        return "MO";
    } else if (state == "montana") {
        return "MT";
    } else if (state == "nebraska") {
        return "NE";
    } else if (state == "nevada") {
        return "NV";
    } else if (state == "new hampshire") {
        return "NH";
    } else if (state == "new jersey") {
        return "NJ";
    } else if (state == "new mexico") {
        return "NM";
    } else if (state == "new york") {
        return "NY";
    } else if (state == "north carolina") {
        return "NC";
    } else if (state == "north dakota") {
        return "ND";
    } else if (state == "northern mariana islands") {
        return "MP";
    } else if (state == "ohio") {
        return "OH";
    } else if (state == "oklahoma") {
        return "OK";
    } else if (state == "oregon") {
        return "OR";
    } else if (state == "palau") {
        return "PW";
    } else if (state == "pennsylvania") {
        return "PA";
    } else if (state == "puerto rico") {
        return "PR";
    } else if (state == "rhode island") {
        return "RI";
    } else if (state == "south carolina") {
        return "SC";
    } else if (state == "south dakota") {
        return "SD";
    } else if (state == "tennessee") {
        return "TN";
    } else if (state == "texas") {
        return "TX";
    } else if (state == "utah") {
        return "UT";
    } else if (state == "vermont") {
        return "VT";
    } else if (state == "virgin islands") {
        return "VI";
    } else if (state == "virginia") {
        return "VA";
    } else if (state == "washington") {
        return "WA";
    } else if (state == "west virginia") {
        return "WV";
    } else if (state == "wisconsin") {
        return "WI";
    } else if (state == "wyoming") {
        return "WY";
    } else {
        return "--";
    }
 }

const std::vector<Contact *> & DMS::getPersonPhoneVector() {
    return phoneVector;
}

const std::vector<Contact *> & DMS::getBusinessPhoneVector() {
    return businessPhoneVector;
}

