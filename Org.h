#ifndef ORG_H
#define ORG_H

#include "CPU.h"
#include "OrgState.h"
#include "emp/Evolve/World_structure.hpp"

class Organism
{
  CPU cpu;
  double mutationRate;
  int gender;

public:
  Organism(OrgWorld *world, double points = 0.0) : cpu(world)
  {
    SetPoints(points);
  }
  
  void SetGender(double _in) { gender = _in; }
  void SetPoints(double _in) { cpu.state.points = _in; }
  void AddPoints(double _in) { cpu.state.points += _in; }
  double GetPoints() { return cpu.state.points; }

  // These functions return whether an organism has solves a specific task
  bool GetEQESolved() { return cpu.state.GetHasSolvedTaskAtIndex(0); }
  bool GetXORSolved() { return cpu.state.GetHasSolvedTaskAtIndex(1); }
  bool GetNORSolved() { return cpu.state.GetHasSolvedTaskAtIndex(2); }
  bool GetANDNSolved() { return cpu.state.GetHasSolvedTaskAtIndex(3); }
  bool GetORSolved() { return cpu.state.GetHasSolvedTaskAtIndex(4); }
  bool GetANDSolved() { return cpu.state.GetHasSolvedTaskAtIndex(5); }
  bool GetORNSolved() { return cpu.state.GetHasSolvedTaskAtIndex(6); }
  bool GetNANDSolved() { return cpu.state.GetHasSolvedTaskAtIndex(7); }
  bool GetNOTSolved() { return cpu.state.GetHasSolvedTaskAtIndex(8); }

  // Returns whether or not an organism has solve the task at a given index
  bool GetTaskSolvedAtIndex(int index) {
    return cpu.state.GetHasSolvedTaskAtIndex(index);
  }

  // Returns the organism's gender
  int getGender()
  {
    return gender;
  }

  // Returns whether or not the organism has solved any tasks in this time instance
  bool AnyTaskSolved()
  {
    return cpu.state.GetNumSolvedTasks() > 0;
  }

  // Returns the total number of tasks the organism was able to solve at this time instance
  int GetTotalTasksSolved() {
    return cpu.state.GetTotalTasksSolved();
  }

  // Resets the organism's cpu
  void Reset()
  {
    cpu.Reset();
  }

  // Simulates the effects of genetic mutation
  void Mutate(double _mutationRate)
  {
    cpu.Mutate(mutationRate);
  }

  // Returns an offspring of the organism if reproduction is successful, otherwise returns a null object
  std::optional<Organism> CheckReproduction(double _mutationRate)
  {
    if (GetPoints() >= 16)
    {
      Organism offspring = *this;
      offspring.Reset();
      offspring.Mutate(_mutationRate);
      emp::Random random3{8};
      int gender = random3.GetInt(0, 2);
      offspring.SetGender(gender);
      AddPoints(-16);
      return offspring;
    }
    return {};
  }

  // Simulates an organism and its attempts to solve tasks in a single time instance
  void Process(emp::WorldPosition current_location)
  {
    cpu.state.current_location = current_location;
    // TODO: Run the CPU!
    cpu.state.anyTaskSolved = false;
    cpu.RunCPUStep(10);
  }

  // Removes points if an organism hasn't solved anytasks
  void Reaper() {
    if(!(cpu.state.anyTaskSolved)) {
      AddPoints(-0.0625);
    }
  }

  // Returns the number of different tasks an organism has solved out of the 9 available tasks at this time instance
  int GetNumTasksSolved()
  {
    return cpu.state.GetNumSolvedTasks();
  }

  void PrintGenome()
  {
    std::cout << "program ------------" << std::endl;
    cpu.PrintGenome();
    std::cout << "end ---------------" << std::endl;
  }
};

#endif
