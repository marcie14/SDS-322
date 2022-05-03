//object oriented program for Disease Propagation - Final Project
//Author: Dhanny Indrakusuma, Marcie Legarde, Santhosh Saravanan

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
        string status = "healthy";
        int sick_days;
        bool susceptible = false;
        bool isolate = false;
        
    public:
        Person() {} //default constructor

        Person(string stat) { //secondary constructor
            status = "infected";
            sick_days = 5;
        }
        //methods
        void prob_sick() { //10% chance of getting sick
            if (status == "healthy" && !susceptible) {
                if (rand()%10 == 0) {
                    if (rand()%5 == 0) {
                        status = "asymptomatic";
                    }
                else {
                    status = "infected";
                    sick_days = 5; 
                }
            }   
            else if(status == "healthy" && susceptible) { //if susceptible 50% chance of getting sick
                if (rand()%80 == 0) {
                    status = "infected";
                    sick_days = 5;
                }
            }
            }
        }
        
        string vaccine() { return status = "vaccinated"; } //getting vaccinated

        void stat_update() { //status update when infected, vaccinated upon recovery
            if (status == "infected") {
                sick_days--;
                if (sick_days <= 0) { vaccine(); } //once recovered, person becomes immune and can never get sick again
            }
        }

        //getters
        int get_sickdays() { return sick_days; }
        string get_status() { return status; }
        bool get_isolate() {return isolate;}
        bool get_susceptible() { return susceptible; }

        //setters
        void set_isolate() { isolate = true; }
        void set_susceptible() { susceptible = true; }
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

            // create vector of susceptible people
            vector<int> susceptible_People;
            for(int i = 0; i < (population_size/4); i++) { // generate People indices of 25% of the population
                susceptible_People.push_back(rand() % (population_size - 1) + 1);
            }
            for(auto &p : susceptible_People) { // a person becomes susceptible at each entry of susceptible vector
                People.at(p).set_susceptible();
            }

            // create vector of isolating people
            vector<int> isolating_People;
            for(int i = 0; i < (population_size/2); i++) { // generate People indices of 50% of the population
                isolating_People.push_back(rand() % (population_size - 1) + 1);
            }
            for(auto &p : isolating_People) {// a person becomes isolating at each entry of isolating vector
                People.at(p).set_isolate();
            }
        }

        //getter for people vector
        vector<Person> get_People() { return People; }

        //methods
        void pop_update(int interactions_num) { //update who sick people interact with
            int r = 0;
            vector<Person> Sick;
            day++;

            cout << "Day: " << day << endl;
            cout << "Infected: " << sick_count() << endl;
            cout << "Vaccinated: " << vacc_count() << endl << endl;

            for (auto p: People) { //temporary push sick people into a sick vector
                if (p.get_status() == "infected" || p.get_status() == "asymptomatic") { Sick.push_back(p); }
            }
        
            for (auto &s: Sick) { //getting sick ppl to interact with healthy ppl
                if (s.get_isolate() == false) {
                    for (int i=0; i<interactions_num; i++) {
                        if (s.get_status() == "infected"){
                            r = rand()%population_size;
                            Person &p = People.at(r); //getting ppl from interaction & infect them
                            p.prob_sick();
                        }
                    }
                }
            }
            pop_recover();
        }

        void pop_recover() { //get each person to recover
            for (auto &p: People) {
                if (p.get_sickdays() > 0) { p.stat_update(); }
            }
        }

        int get_day() { return day; }
        //counters
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

        int sus_count() {//counts susceptible people
            int sus_pop = 0;
            for (auto &p : People) {
                if (p.get_susceptible()) { sus_pop++; }
            }
            return sus_pop;
        }

        int iso_count() {//counts isolating people
            int iso_pop = 0;
            for (auto &p : People) {
                if (p.get_isolate()) { iso_pop++; }
            }
            return iso_pop;
        }

        int asym_count() {//counts asymptomatic (infected) people
            int asym_pop = 0;
            for (auto &p : People) {
                if (p.get_status() == "asymptomatic") { asym_pop++; }
            }
            return asym_pop;
        }
};

int main() {
    int pop_size = 40000;
    int interaction = 40;
    int max_sick = 0;
    int d = 1;

    cout << "***** Starting Disease Propagation Model Simulation ***** " << endl;
    //cout << "Enter population size: ";
    //cin >> pop_size;

    //cout << "Enter daily interaction: ";
    //cin >> interaction;
    //cout << endl;

    Population pop1(pop_size);

    while (pop1.sick_count() > 0 ) { //re-run everyday till there is no sick person
        pop1.pop_update(interaction);
        if (pop1.sick_count() > max_sick) { max_sick = pop1.sick_count(); d++; }
    }

    pop1.pop_update(interaction); //last status

    cout << "For population size: " << pop_size << " people" << endl;
    cout << "With daily interactions of: " << interaction << " people" << endl;
    cout << pop1.sus_count() << " people are susceptible" << endl;
    cout << pop1.iso_count() << " people isolated" << endl;
    cout << pop1.asym_count() << " people are infected but asymptomatic" << endl;
    cout << "Maximum number of sick people is: " << max_sick << ", on day " << d << endl;

    cout << "It took " << pop1.get_day() << " days for the virus to be eradicated, "
        << pop_size - pop1.vacc_count() << " people were never sick" << endl << endl;
    cout << "***** end of simulation ***** " << endl;
    cout << "Project by: Dhanny Indrakusuma, Marcie Legarde & Santhosh Saravanan" << endl;

    return 0;
}
