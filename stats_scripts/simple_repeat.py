#a script to run several replicates of several treatments locally

directory = "Example/"
seeds = range(10, 21)
mut_prob = 0.005
num_starting_orgs = 8
sexual_reproduction = 0
world_width = 60
world_height = 60

import subprocess

def cmd(command):
    '''This wait causes all executions to run in series.                          
    For parralelization, remove .wait() and instead delay the                      
    R script calls unitl all neccesary data is created.'''
    return subprocess.Popen(command, shell=True).wait()

def silent_cmd(command):
    '''This wait causes all executions to run in series.                          
    For parralelization, remove .wait() and instead delay the                      
    R script calls unitl all neccesary data is created.'''
    return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).wait()

print("Copying MySettings.cfg to "+directory)
cmd("cp MySettings.cfg "+directory)

for a in seeds:
    command_str = './sgp_lab -SEED '+str(a)+' -MUT_PROB '+str(mut_prob)+' -SR '+str(sexual_reproduction)+' -FILE_PATH '+directory+' -FILE_NAME SEED:'+str(a)+'SR:'+str(sexual_reproduction) + 'Width:' + str(world_width) + 'Height:' + str(world_height) + '.data'+' -INITIAL_ORG_NUM '+str(num_starting_orgs)+' -WORLD_WIDTH '+str(world_width)+' -WORLD_HEIGHT '+str(world_height)
        
    print(command_str)
    cmd(command_str)