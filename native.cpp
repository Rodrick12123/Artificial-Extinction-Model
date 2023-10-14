#include <iostream>

#include "World.h"
#include "ConfigSetup.h"

// This is the main function for the NATIVE version of this project.
int main(int argc, char *argv[]) {

  MyConfigType config;

  bool success = config.Read("MySettings.cfg");
  if(!success) config.Write("MySettings.cfg");

  auto args = emp::cl::ArgManager(argc, argv);
  if (args.ProcessConfigOptions(config, std::cout, "MySettings.cfg") == false) {
    std::cerr << "There was a problem in processing the options file." << std::endl;
    exit(1);
  }
  if (args.TestUnknown() == false) {
    std::cerr << "Leftover args no good." << std::endl;
    exit(1);
  }

  emp::Random random{7};
  OrgWorld world(random);
  random.ResetSeed(config.SEED());
  world.SetGlobalMutationRate(config.MUT_PROB());
  int repoToggle = config.SR();
  int num_w_boxes = config.WORLD_WIDTH();
  int num_h_boxes = config.WORLD_HEIGHT();

  world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
  world.SetupOrgFile(config.FILE_PATH()+config.FILE_NAME()).SetTimingRepeat(100);
  world.AddOrganisms(config.INITIAL_ORG_NUM());

  for(int update=0; update<200001; update++){
    world.Update();
    std::cout << "Seed: " << config.SEED() << ". Mut prob: " << config.MUT_PROB() << ". Width: " << config.WORLD_WIDTH() << ". Height: " << config.WORLD_HEIGHT() << ". Starting Orgs: " << config.INITIAL_ORG_NUM() << std::endl;
    std::cout << (double)(((double)update)/2000.0) << "% finished" << std::endl; 
    if(repoToggle == 1) {world.moveOrg();}
  }
}
