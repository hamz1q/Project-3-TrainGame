/*
 *Title: Route.cpp
 *Author: Hamza Qureshi
 *Email: hamz1@umbc.edu
 *Description: Member function implementation for the Train.h file for project 3 CMSC 202
 */

#include "Train.h"

// Name: Train() - Overloaded Constructor
// Desc - Creates a new train with one car and a size of one. A train is a linked list of cars
//        The train is passed a route which is a list of stops for the train
// Preconditions - None
// Postconditions - Creates a new train
Train::Train(Route* route)
{
  m_head = nullptr;
  m_route = route;
  m_totalCars = 0;
  AddCar();
}

// Name: ~Train() - Destructor
// Desc - Removes each car from the train and deallocates each passenger in each car.
//        Resets each of the linked list pointers
// Preconditions - None
// Postconditions - Removes all cars and passengers
Train::~Train()
{
  Car *curr = m_head;
  while(curr != nullptr)
    {
      for(unsigned int i = 0; i < curr->m_passengers.size(); i++)
	{
	  delete curr->m_passengers.at(i);
	}
      curr->m_passengers.clear();
      m_head = curr;
      curr = curr->m_next;
      delete m_head;
    }

  m_head = nullptr;
  m_route = nullptr;
  m_totalCars = 0;
}

// Name: AddCar
// Desc - Dynamically allocates a new car and inserts it at the end of the train.
//        Increments m_totalCars
// Preconditions - Train must exist
// Postconditions - New car node inserted at end of train
void Train::AddCar()
{
  m_totalCars += 1;
  Car* newCar = new Car(CAR_CAPACITY, m_totalCars);

  if(m_head == nullptr)
    {
      m_head = newCar;
    }
  else
    //goes to the end of the train and adds a car
    {
      Car* curr = m_head;
      
      while(curr->m_next != nullptr) 
	{
	  curr = curr->m_next;
	}

      curr->m_next = newCar;
    }
}

// Name: TrainStatus
// Desc - Displays number of cars, number of passengers, and the route details
// Preconditions - Train is populated
// Postconditions - Displays information at a specific location
void Train::TrainStatus()
{
  Car* curr = m_head; 
    
  cout << "Number of cars: " << m_totalCars << endl;


  while(curr != nullptr)
    {
      cout << "Car " << curr->m_carNumber << " : Number of passengers: " << curr->m_passengers.size() << endl;
      curr = curr->m_next;
    }
  m_route->PrintRouteDetails(); 
}

// Name: LoadPassengers(filename)
// Desc - Iterates through an input file and if the name of the "start location"
//        matches m_curLocation then creates a new passenger and has them BoardPassenger
// **Note - If you run this twice at any stop, it will load the same people over
//          and over again - you do not need to check for this
// Preconditions - Valid input file with first name, last name, age, start location,
//                 end location all comma separated with one passenger on each line
// Postconditions - Loads all passengers at this into cars
void Train::LoadPassengers(string passName)
{
  string line;
  //variable that counts number of passengers
  int count = 0;
  ifstream myFile;
  //Used to count how many passengers are in total so vectors can be made with the same size to store all passenger data
  myFile.open(passName);
  if(myFile.is_open())
    {
      while(getline(myFile, line))
	{
	  count += 1;
	}
    }
  myFile.close();
  //make vectors with appropiate size, create variable that counts amount boarded
  int amtBoard = 0;
  vector <string> fullName(count);
  vector <string> firstName(count);
  vector <string> lastName(count);
  vector <string> strAge(count);
  vector <int> age(count);
  vector <string> start(count);
  vector <string> end(count);
  //iterate through the file again adding the data to the vectors just created 
  myFile.open(passName);
    
  if(myFile.is_open())
    {
      for(int i = 0; i < count; i++)
	{
	  getline(myFile, firstName.at(i), ',');
	  getline(myFile, lastName.at(i), ',');
	  getline(myFile, strAge.at(i), ',');
	  age[i] = stoi(strAge.at(i));
	  getline(myFile, start.at(i), ',');
	  getline(myFile, end.at(i));
	}
    }

  myFile.close();
  
  for(int i = 0; i < count; i++)
    {
      fullName.at(i) = firstName.at(i) + " " + lastName.at(i);
      
      if(start.at(i) == m_route->GetCurrentStop()->GetName())
	{
	  Passenger* newPassenger = new Passenger(fullName.at(i), age.at(i), start.at(i), end.at(i));
	  BoardPassenger(newPassenger);
	  amtBoard += 1;
	}
    }

  cout << "Boarded " << amtBoard << " passengers" << endl;
  //Clearing all the vectors for the next loading 
  fullName.clear();
  firstName.clear();
  lastName.clear();
  strAge.clear();
  age.clear();
  start.clear();
  end.clear();
  
}

// Name: BoardPassenger(Passenger*)
// Desc - Checks to see if the capacity of the train has been met. If so,
//        adds a new car and inserts the passenger in the first open car starting at the front.
//        Also, displays name and final destination of passenger being loaded.
// Preconditions - Passenger object already allocated and data populated. Train exists
// Postconditions - Adds new passenger to a car from front to back
void Train::BoardPassenger(Passenger* newPassenger)
{
  Car* curr = m_head;
  
  if(IsTrainFull())
    {
      //adding a new car to the back and inserting passengers in it 
      AddCar(); 
      while(curr->m_next != nullptr)
	{
	  curr = curr->m_next;;
	}
      curr->AddPassenger(newPassenger);
      cout << newPassenger->GetName() << " Boards the Train (Destination: " << newPassenger->GetFinalDestination() << ")" << endl;
      
    }  
  else
    {
      //finds open spot in train and inserts passenger there 
      while(curr != nullptr)
	{
	  if(!curr->IsFull())
	    {
	      curr->AddPassenger(newPassenger);
	      cout << newPassenger->GetName() << " Boards the Train (Destination: " << newPassenger->GetFinalDestination() << ")" << endl;
	      break;
	    }
	  else
	    {
	      curr = curr->m_next;
	    }
	}
    }
  
}

// Name: DisembarkPassenger()
// Desc - For each passenger on the train, checks to see if they are at their final
//        destination. If they are, the passenger is removed from the train.
// Note - Deallocate the passenger first then you can use the m_passengers.erase command.
// Do not remove cars that are now unnecessary based on capacity (if you
// added a car to hold 17 passengers and one disembarks, do not remove it)
// Preconditions - Passengers reach their destination
// Postconditions - Passengers are removed from the m_passengers, deallocated, and erased
void Train::DisembarkPassengers()
{
  Car* curr = m_head;
  //variable to count total number of passengers disembarking 
  int count = 0;
  
  while(curr != nullptr)
    {
      for(unsigned int i = 0; i < curr->m_passengers.size(); i++)
	{
	  if(curr->m_passengers.at(i)->GetFinalDestination() == m_route->GetCurrentStop()->GetName())
	    {
	      count += 1;
	      cout << curr->m_passengers.at(i)->GetName() << " Disembarks the Train (Destination ";
	      cout << curr->m_passengers.at(i)->GetFinalDestination() << ")" << endl;
	      delete curr->m_passengers.at(i);
	      curr->m_passengers.erase(curr->m_passengers.begin()+i);
	    }
	}
      curr = curr->m_next;
    }
  cout << count << " Passengers disembarked" << endl;
}

// Name: TravelToNextStop
// Desc - When chosen from the menu, moves the train to the next stop. If you have
//        reached last stop, indicates this.
//        When at end, also recommends turning train around.
// Preconditions - Valid train on valid route
// Postconditions - Updates SetCurrentStop to next stop on route
void Train::TravelToNextStop()
{
  Stop* temp = m_route->GetCurrentStop();
  //checking if train is at last stop 
  if(m_route->GetCurrentStop()->GetNextStop() == nullptr)
    {
      cout << "You have reached the last stop! Turn Train around or End" << endl;
    }
  else
    {
      m_route->SetCurrentStop(temp);
    }
}

// Name: TurnTrainAround()
// Desc - When chosen from the menu at the end of the route, reverses the Route by
//        calling ReverseRoute
// Preconditions - Valid train on valid route
// Postconditions - The entire route is reversed and the train starts at the new front
void Train::TurnTrainAround()
{
  m_route->ReverseRoute(); 
}

// Name: IsTrainFull()
// Desc - Iterates through train to see if total number of passengers is greater
//        than maximum capacity
// Preconditions - Valid train on valid route
// Postconditions - Returns true if full else false
bool Train::IsTrainFull()
{
  bool full;
  Car* curr = m_head;

  while(curr != nullptr)
    {
      if(curr->IsFull())
	{
	  curr = curr->m_next;
	  full = true;
	}
      else
	{
	  curr = curr->m_next;
	  full = false;
	}
    }
  
  return full;
}
