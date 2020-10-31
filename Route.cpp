/*
 *Title: Route.cpp
 *Author: Hamza Qureshi
 *Email: hamz1@umbc.edu
 *Description: Member function implementation for the Route.h file for project 3 CMSC 202
 */

#include "Route.h"
Route::Route()
{
  m_head = nullptr;
  m_tail = nullptr;
  m_totalStops = 0;
}

Route::~Route()
{
  Stop *curr = m_head;
  
  while(curr != nullptr)
    {
      m_head = curr;
      curr = curr->GetNextStop();
      delete m_head;
    }

  m_head = nullptr;
  m_tail = nullptr;
  m_totalStops = 0;
}

//iterates through file and makes passengers if start location matches 
void Route::LoadRoute(string file)
{
  string line; 
  int count = 0;
  ifstream myFile;
  //this counts the amount of stops in the file to make coreect size of arrays 
  myFile.open(file);
  if(myFile.is_open())
    {
      while(getline(myFile, line))
	{
	  count += 1;
	}
    }
  myFile.close();
  //makes appropriate size arrays
  string name[count];
  string numbers[count];
  int number[count];

  myFile.open(file);
  if(myFile.is_open())
    {
      for(int i = 0; i < count; i++)
	{
	  getline(myFile, name[i], ',');
	  getline(myFile, numbers[i]);
	  number[i] = stoi(numbers[i]);
	  AddStop(name[i], number[i]);
	}
    }
  myFile.close();
  //always will initially set current stop to first node 
  m_currentStop = m_head; 
}

//goes to the end of linked list and adds stop
void Route::AddStop(string name, int number)
{
  Stop *newStop = new Stop(name, number);
  
  if(m_head == nullptr)
    {
      m_head = newStop;
      m_tail = newStop;
    }
  else
    {
      m_tail->SetNextStop(newStop);
      m_tail = newStop;
    }
  m_totalStops += 1;
}

//returns the current and next stops
void Route::PrintRouteDetails()
{
  Stop* next = m_currentStop;
  
  cout << "Current Stop: " << m_currentStop->GetName() << " (" << m_currentStop->GetNumber() << ")";
  cout << endl;
  if(m_currentStop->GetNextStop() != nullptr)
    {
      next = m_currentStop->GetNextStop();
      
      cout << "Next Stop: " << next->GetName() << " (" << next->GetNumber() << ")";
      cout << endl;
    }
}

//returns current stop
Stop* Route::GetCurrentStop()
{
  return m_currentStop;
}

//sets the current stop as the train moves
void Route::SetCurrentStop(Stop* temp)
{
  temp = m_currentStop->GetNextStop();
  m_currentStop = temp;
}

//reverses the entire route linked list
void Route::ReverseRoute()
{
  if(m_currentStop->GetNextStop() == nullptr)
    {
      Stop* curr=m_head;
      Stop* prev=nullptr;
      Stop* next=nullptr;
      //goes through each node and points it to the previous node 
      while(curr!=nullptr)
	{
	  next = curr->GetNextStop();
	  curr->SetNextStop(prev);
	  prev = curr;
	  curr = next;
	}

      m_head = prev;
      m_currentStop = m_head;
    }
  //validates train is at the last stop
  else
    {
      cout << "Can only reverse at end of Route!" << endl;
    }
}
