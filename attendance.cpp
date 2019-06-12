#include "attendance.hpp"

void Attendance::initNameAndNum()
{
  //open the file
  std::ifstream attendancefile;
  attendancefile.open("members.txt");

  std::string input;
  std::string number;
  std::string fname;
  std::string lname;
  int  daysAttended;

  parse state = id;
  if (attendancefile.is_open())
  {
    while (attendancefile >> input)
    {
      switch (state)
      {
        case id:
          number = input;
          //std::cout << number << std::endl;
          state = colon1;
        break;
        case colon1:
          state = firstName;
        break;
        case firstName:
          fname = input;
          state = lastName;
        break;
        case lastName:
          lname = input;
          state = colon2;
        break;
        case colon2:
          state = days;
        break;
        case days:
          daysAttended = std::stoi(input);
          //append map
          fname += ' ';
          fname += lname;
          //std::cout << fname << std::endl;
          nameToNumber.insert( std::pair<std::string, std::string>(number, fname));
          personalAttendance.insert( std::pair<std::string, int>(number, daysAttended));
          state = id;
        break;
      }
    }
  }
  else
    std::cout << "broken init\n";
}

std::string Attendance::getName(std::string idNum)
{
  if (inTheSystem(idNum))
    return nameToNumber.at(idNum);
  else
    return "ERR";
}

bool Attendance::inTheSystem(std::string idNum)
{
  std::map<std::string, std::string>::iterator it;
  it = nameToNumber.find(idNum);
  if (it != nameToNumber.end())
    return true;
  else
    return false;
}

void Attendance::addNameNum(std::pair<std::string, std::string> in)
{
  nameToNumber.insert(in);
}

void Attendance::incrementDay(std::string idNum)
{
  int curr;
  if (inTheSystem(idNum))
  {
    curr = personalAttendance[idNum];
    curr++;
    personalAttendance[idNum] = curr;
  }
}

int Attendance::getDays(std::string idNum)
{
  if (inTheSystem(idNum))
  {
    return personalAttendance.at(idNum);
  }
}

bool Attendance::addToToday(std::string idNum)
{
  std::vector<std::string>::iterator it;
  it = find(todaysAttendees.begin(), todaysAttendees.end(), idNum);
  if (it != todaysAttendees.end())
    return false;
  else
  {
    todaysAttendees.push_back(idNum);
    return true;
  }
}

bool Attendance::setDate(std::string in)
{
  for (size_t i = 0; i < in.size(); i++)
  {
    if (i < 2 || i > 2)
      if (!isdigit(in[i]))
        return false;
    else if (i == 2 && in[i] != ':')
        return false;
  }
  todaysDate = in;
  return true;
}

std::string Attendance::getDate()
{
  return todaysDate;
}

void Attendance::addEmail(std::pair<std::string, std::string> in)
{
	emailToNumber.insert(in);
}

std::string Attendance::getEmail(std::string id)
{
	if (inTheSystem(id))
		return emailToNumber.at(id);
	else
		return "ERR";
}

// std::vector<std::string> Attendance::attendanceRanking()
// {
//   // std::vector<int> days;
//   // //push all days to vector
//   // for(auto it = personalAttendance.cbegin();
//   //     it != personalAttendance.cend(); ++it)
//   // {
//   //   days.push_back(it->second);
//   // }
//   //
//   // //sort the vector
//   // std::sort(days.begin(), days.end(), std::greater<int>());
//
//   //vector contains attendance in decending order
//   //so now match that with its corresponding id
//   std::vector<std::string> thing;
//   return thing;
// }
