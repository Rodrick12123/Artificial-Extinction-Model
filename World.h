#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"

#include "Org.h"
#include "OrgState.h"
#include "Task.h"

class OrgWorld : public emp::World<Organism>
{
  emp::Random &random;
  std::vector<emp::WorldPosition> reproduce_queue;

  std::vector<Task *> tasks{new EQETask(), new XORTask(), new NORTask(), new ANDNTask(), new ORTask(), new ORNTask(), new ANDTask(), new NANDTask(), new NOTTask()};
  //std::vector<Task *> tasks{new NOTTask()};

  int updateNumber = 0;
  int TOTAL_TASKS_SOLVED;

  double globalMutationRate = 0.005;

  emp::Ptr<emp::DataMonitor<int>> data_node_symcount;

    /* Returns an unoccupied position in the world*/
    int GetUnoccupiedPosition() {
        int position = random.GetInt(0, GetSize());
        if (IsOccupied(position)){
            position = GetFirstUnoccupiedPosition();
        }
        return position;
    }

    /* Returns the first unoccupied position in the world*/
    int GetFirstUnoccupiedPosition() {
        for(int i = 0; i < GetSize(); i++) {
            if (!IsOccupied(i)) {
                return i;
            }
        }
        return 0;
    }

  // Performs the pulse extinction event
  void DoExtinction()
  {
    int numToExtinct = GetNumOrgs() * 0.999;
    int numExtinct = 0;
    emp::vector<size_t> extinctSchedule = emp::GetPermutation(random, GetSize());
    for (int i : extinctSchedule)
    {
      if (!IsOccupied(i))
      {
        continue;
      }
      RemoveOrgAt(i);
      numExtinct ++;
      if (numExtinct == numToExtinct) {return;}
    }
  }
  
  // Process each organism
  void ProcessPopulation()
  {
    TOTAL_TASKS_SOLVED = 0;

    emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());

    for (int i : schedule)
    {
      if (!IsOccupied(i))
      {
        continue;
      }
      pop[i]->Process(i);
      TOTAL_TASKS_SOLVED += pop[i]->GetTotalTasksSolved();
      if (pop[i]->GetPoints() < -32.0)
      {
        RemoveOrgAt(i);
      }
    }
    std::cout << "Total solved: " << TOTAL_TASKS_SOLVED << std::endl;
  }
  //takes points away from organisms that don't complete tasks.
  void Reap() {
    for (int i = 0; i < GetSize(); i++)
    {
      if (IsOccupied(i)){pop[i]->Reaper();}
    }
  }

  Organism *ExtractOrganism(int i)
  {
    Organism *p = pop[i];
    pop[i] = nullptr;
    return p;
  }

  // Time to allow reproduction for any organisms that ran the reproduce instruction
  void AttemptReproduction()
  {
    // Time to allow reproduction for any organisms that ran the reproduce instruction
    for (emp::WorldPosition location : reproduce_queue)
    {
      if (!IsOccupied(location))
      {
        return;
      }
      std::optional<Organism> offspring =
          pop[location.GetIndex()]->CheckReproduction(globalMutationRate);
      if (offspring.has_value())
      {
        DoBirth(offspring.value(), location.GetIndex());
      }
    }
    reproduce_queue.clear();
  }



public:
  OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random) {}

  ~OrgWorld()
  {
    if (data_node_symcount)
    {
      data_node_symcount.Delete();
    }
  }

  /* Adds organisms to simulation*/
  void AddOrganisms(int initial_orgs) {
      for(int i = 0; i < initial_orgs; i++) {
          int org_position = GetUnoccupiedPosition();
          Organism* new_org = new Organism(this, 0);
          int gender = random.GetInt(0, 2);
          new_org->SetGender(gender);
          AddOrgAt(new_org, org_position);
      }
  }

  void ReproduceOrg(emp::WorldPosition location)
  {
    // Wait until after all organisms have been processed to perform
    // reproduction. If reproduction happened immediately then the child could
    // ovewrite the parent, and then we would be running the code of a deleted
    // organism
    reproduce_queue.push_back(location);
  }

  // Sets up data node that tracks functional activity of the population
  emp::DataMonitor<int> &GetSymCountDataNode()
  {
    if (!data_node_symcount)
    {
      data_node_symcount.New();
      OnUpdate([this](size_t)
               {
        data_node_symcount -> Reset();
        data_node_symcount->AddDatum(TOTAL_TASKS_SOLVED); });
    }
    return *data_node_symcount;
  }

  // Sets up file to track functional activity of the population over time
  emp::DataFile &SetupOrgFile(const std::string &filename)
  {
    auto &file = SetupFile(filename);
    auto &node1 = GetSymCountDataNode();
    file.AddVar(update, "update", "Update");
    file.AddTotal(node1, "total number of tasks solved", "Total number of Tasks Solved");

    file.PrintHeaderKeys();

    return file;
  }

  // Returns the population of organisms
  const pop_t &GetPopulation() { return pop; }

  // Sets the mutation rate of organisms
  void SetGlobalMutationRate(double _globalMutationRate)
  {
    globalMutationRate = _globalMutationRate;
  }

  // moves organisms around the field in pseudo-sexual reproduction
  void moveOrg()
  {
    for (int i = 0; i < GetSize(); i++)
    {

      if (IsOccupied(i))
      {

        Organism *org = ExtractOrganism(i);
        emp::WorldPosition position = GetRandomNeighborPos(i);
        // space is open
        if (!IsOccupied(position))
        {
          AddOrgAt(org, position);
        }
        else if (IsOccupied(position))
        {
          size_t id = position.GetIndex();
          Organism org2 = GetOrg(id);
          // same position case
          if (org2.getGender() != org->getGender())
          {
            AddOrgAt(org, position);
            int chance = random.GetInt(0, 2);
            // simulates the chance of reproducing
            if (chance == 1)
            {
              org->AddPoints(16);
            }
          }
        }
      }
    }
  }

  // Simulates one time instance in the world
  void Update()
  {
    emp::World<Organism>::Update();
    updateNumber++;
    if (updateNumber == 100000)
    {
      DoExtinction();
    }
    ProcessPopulation();
    // commenting this out will disable the sexual reproduction extension
    //moveOrg();
    AttemptReproduction();
    Reap();
  }
  
  // Returns the number of organisms who have solved the task at a given index
  int GetNumOrgsSolvedTaskAtIndex(int index) {
    int total = 0;
    emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());

    for (int i : schedule) {
      if (!IsOccupied(i))
      {
        continue;
      }
      if(pop[i]->GetTaskSolvedAtIndex(index)) {
        total++;
      }
    }
    return total;
  }

  // Returns the current functional activity
  int GetSumTotalTasksSolved() {
    return TOTAL_TASKS_SOLVED;
  }

  // Returns the number of organisms in the population
  int GetPopulationSize() {
    int total = 0;
    emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());

    for (int i : schedule) {
      if (!IsOccupied(i))
      {
        continue;
      }
      total ++;
    }
    return total;
  }

  // Checks whether organisms have solved tasks and rewards points accordingly
  void CheckOutput(float output, OrgState &state)
  {
    for (int i = 0; i<tasks.size(); i++)
    {
      double points_gained = tasks.at(i)->CheckOutput(output, state.last_inputs);
      if(state.GetAllSolved()){
        state.ResetSolved();
      }
      if (state.GetHasSolvedTaskAtIndex(i))
      {
        state.SetHasSolvedTaskAtIndex(i, false);
        continue;
      }
      if (points_gained > 0.0)
      {
        state.anyTaskSolved = true;
        state.SetHasSolvedTaskAtIndex(i, true);
        state.points += points_gained;
        state.IncrementTotalTasksSolved();
        return;
      }
    }
  }

};

#endif
