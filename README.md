# Attendance-Program
A simple program created to easily take attendance at Chocolate Milk Club at Virginia Tech meetings 

Written with a qt gui that utilizes the included Attendance data structure to track new and old members upon card swipe. 

Hardware: MSR90 USB Swipe Magnetic Credit Card Reader 
https://www.amazon.com/gp/product/B01DUB4GVO/ref=ppx_yo_dt_b_asin_title_o09_s00?ie=UTF8&psc=1

Requires QT installed on your machine to run properly.

To operate with any ID card: 
  Edit the parsing of 'idNum' found in the swipe() function within cmmGui.h
  The current input requires that the input lead with ';' followed by the members 9-digit ID and ends with '=1234'
  
A demo history.txt and member.txt file are included for reference
