#ifndef ORGSTATE_H
#define ORGSTATE_H

#include "emp/Evolve/World_structure.hpp"
#include <cstddef>

class OrgWorld;

struct OrgState
{
  OrgWorld *world;

  // Array for tracking the four most recent input values
  uint32_t last_inputs[4];
  // Index of the most recent input value in array
  size_t last_input_idx;
  // How many points this organism has currently
  double points;

  //How many total tasks the organism has solved
  int totalTasksSolved = 0;

  // if the organism has solved a task during the last 10 cycles of the CPU
  bool anyTaskSolved;

  std::vector<bool> has_solved{false, false, false, false, false, false, false, false, false};

  // Needs to know current location for possible reproduction
  emp::WorldPosition current_location;

  /**
   * Adds an input number to the tracker
   */
  void add_input(uint32_t input)
  {
    last_inputs[last_input_idx] = input;
    last_input_idx = (last_input_idx + 1) % 4;
  }

  // Resets which tasks the organism has solved
  void ResetSolved() {
    has_solved = {false, false, false, false, false, false, false, false, false};
  }

  // Returns whether or not the organism has solved all tasks
  bool GetAllSolved() {
    bool allSolved = true;
    for(bool task : has_solved) {
      if(task == false) {
        allSolved = false;
      }
    }
    return allSolved;
  }

  // Returns the number of tasks the organism has solved out of the 9 available tasks at a given time instance
  int GetNumSolvedTasks() {
    int num_tasks_solved = 0;
    for(bool has_solved_task : has_solved) {
      if(has_solved_task){
        num_tasks_solved++;
      }
    }
    return num_tasks_solved;
  }

  // Returns whether the organism has solved the task at a given index
  bool GetHasSolvedTaskAtIndex(int index) {
    return has_solved.at(index);
  }

  // Sets whether or not the organism has solved the task at a given index
  void SetHasSolvedTaskAtIndex(int index, bool value) {
    has_solved.at(index) = value;
  }

  // Increments the amount of tasks an organism has solved
  void IncrementTotalTasksSolved () {totalTasksSolved++;}

  // Returns the total amount of tasks an organism has solved
  int GetTotalTasksSolved () {return totalTasksSolved;}

  // Resets the total amount of tasks an organism has solved
  void ResetTotalTasksSolved () {totalTasksSolved = 0;}
};

#endif