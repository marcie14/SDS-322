//object oriented program for SIR Model (Susceptible, Infected, Recovered)

#include <iostream>
#include <time.h>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;


class Person {
   protected:
      string status = "healthy"; //Susceptible
      int sick_days;

   public:
      Person() {} //default constructor

      Person(string stat) { //secondary constructor
         status = "infected";
         sick_days = 5;
      }

      //methods
      void prob_sick() { //10% chance of getting sick
         if (status == "healthy") {
            if (rand()%10 == 0) {
               status = "infected";
               sick_days = 5; 
            }
         }
      }

      //getting vaccinated
      string vaccine() { return status = "vaccinated"; }

      void stat_update() { //status update when infected, vaccinated upon recovery
         if (status == "infected") {
            sick_days--;
            if (sick_days <= 0) { vaccine(); } //once recovered, person becomes immune and can never get sick again
         }
      }

      //get sick days  
      int get_sickdays() { return sick_days; }
      //get current status
      string get_status() { return status; }
};


class Population : Person { //inherit from "Person" class
   protected:
      vector<Person> People; //vector of people
      int population_size;
      int day;

   public:
      Population() { srand(time(NULL)); } //default constructor

      Population(int size) { //create population
         srand(time(NULL));
         population_size = size;
         day = 0;

         //community vector
         Person p("infected"); //adding patient zero to "People" vector
         People.push_back(p);

         for (int i=1; i<size; i++) { //adding persons into "People" vector
            Person p;
            People.push_back(p);
         }
      }

      //getter for people vector
      vector<Person> get_People() { return People; }

      //methods
      void pop_update(int interactions_num) { //update who sick people interact with
         int r = 0;
         vector<Person> Sick;
         day++;

         cout << "----- Day: " << day << " -----"<< endl;
         cout << "Infected: " << sick_count() << endl;
         cout << "Vaccinated: " << vacc_count() << endl << endl;

         for (auto p: People) { //temporary push sick people into a sick vector
            if (p.get_status() == "infected") { Sick.push_back(p); }
         }

         for (auto &s: Sick) { //getting sick ppl to interact with healthy ppl
            for (int i=0; i<interactions_num; i++) {
               r = rand()%population_size;
               Person &p = People.at(r); //getting ppl from interaction & infect them
               p.prob_sick();
               //cout << r << " " << p.get_status() << endl;
            }
         }
         //for (Person &p : People) { cout << p.get_status() << endl;}         
         pop_recover();            
      }

      void pop_recover() { //get each person to recover
         for (auto &p: People) { 
            if (p.get_sickdays() > 0) { p.stat_update(); }
         }
      }

      int sick_count() { //counts sick people
         int sick_pop = 0;
         for (auto &p : People) {
            if (p.get_status() == "infected") { sick_pop++; }
         }
         return sick_pop; 
      }

      int vacc_count() {//counts vaccinated people
         int vacc_pop = 0;  
         for (auto &p : People) {
            if (p.get_status() == "vaccinated") { vacc_pop++; }
         }
         return vacc_pop;
      }

      int healthy_count() {//counts healthy people
         int healthy_pop = 0;  
         for (auto &p : People) {
            if (p.get_status() == "healthy") { healthy_pop++; }
         }
         return healthy_pop;
      }

      int get_day() { return day;}
};

 
int main()
{
   int pop_size = 40000;
   int interaction = 40;
   int max_sick = 0;
   int d = 1;
   
   cout << "***** Starting SIR Model Simulation ***** " << endl;
   //cout << "Enter population size: ";
   //cin >> pop_size;
   //cout << endl << endl;
   Population pop1(pop_size);

   while (pop1.sick_count() > 0 ) { //re-run everyday till there is no sick person
      pop1.pop_update(interaction);
      if (pop1.sick_count() > max_sick) { max_sick = pop1.sick_count(); d++; }
   }

   pop1.pop_update(interaction); //last status

   cout << "For population size: " << pop_size << endl;
   cout << "Maximum number of sick people is: " << max_sick << ", on day " << d << endl;
   cout << "It took " << pop1.get_day() << " days for the virus to be eradicated, "
      << pop_size - pop1.vacc_count() << " people were never sick" << endl << endl;
   cout << "***** end of simulation ***** " << endl; 

   return 0;
}
