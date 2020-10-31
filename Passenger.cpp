/*
 *Title: Passenger.cpp
 *Author: Hamza Qureshi
 *Email: hamz1@umbc.edu
 *Description: Member function implementation for the Passenger.h file for project 3 CMSC 202
 */

#include "Passenger.h"
//Overloaded constructor 
Passenger::Passenger(string name, int age, string start, string end)
{
  m_fullName = name;
  m_age = age;
  m_startLocation = start;
  m_finalDestination = end;
}

//returns passenger name
string Passenger::GetName()
{
  return m_fullName;
}

//returns the starting location of the passeneger
string Passenger::GetStartLocation()
{
  return m_startLocation;
}

//returns the final location of the passeneger
string Passenger::GetFinalDestination()
{
  return m_finalDestination;
}
