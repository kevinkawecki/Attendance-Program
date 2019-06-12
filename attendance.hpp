#include <vector>
#include <map>

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

enum parse {id, colon1, firstName, lastName, colon2, days};

class Attendance
{
public:

  //read in members.txt and assign personalAttendance, nameToNumber
  void initNameAndNum();

  //return name based on number
  std::string getName(std::string idNum);

  //check if name associated with number
  bool inTheSystem(std::string idNum);

  //associate name with number
  void addNameNum(std::pair<std::string, std::string> in);

  //increment historical attendance
  void incrementDay(std::string idNum);

  //get the number of days person attended
  int getDays(std::string idNum);

  //keep track of who has attended today
  bool addToToday(std::string idNum);

  //set todays daysAttended
  bool setDate(std::string);

  //return date
  std::string getDate();

  //map email and number together
  void addEmail(std::pair<std::string, std::string> in);

  //get email from number
  std::string getEmail(std::string id);

  //create a vector of the highest attendance numbers to lowest attendance numbers
//  std::vector<std::string> attendanceRanking();

//private
  std::string                        todaysDate;
  std::map<std::string, int>         personalAttendance;
  std::map<std::string, std::string> nameToNumber;
  std::map<std::string, std::string> emailToNumber;
  std::vector<std::string>           todaysAttendees;
};

//#include "attendance.cpp"
