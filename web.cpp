#define UIT_VENDORIZE_EMP
#define UIT_SUPPRESS_MACRO_INSEEP_WARNINGS

#include "emp/math/Random.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/Document.hpp"
#include "emp/web/web.hpp"
#include "World.h"
#include "Org.h"
#include "emp/config/ArgManager.hpp"
#include "emp/prefab/ConfigPanel.hpp"
#include "emp/web/UrlParams.hpp"
#include "ConfigSetup.h"

emp::web::Document doc("target");
emp::web::Document tracking("tracking");
emp::web::Document settings("settings");
emp::web::Document buttons("buttons");
MyConfigType config;

class AEAnimator : public emp::web::Animate
{

    // area width and height 
    int num_h_boxes = 60;
    int num_w_boxes = 60;
    int total_boxes = num_h_boxes * num_h_boxes;
    const double RECT_SIDE = 10;
    double width{num_w_boxes * RECT_SIDE};
    double height{num_h_boxes * RECT_SIDE};
    emp::Random random{7};
    OrgWorld world{random};
    int repoToggle;

    emp::web::Canvas canvas{width, height, "canvas"};
    emp::web::Canvas text_canvas{width, height/2, "text_canvas"};

    int current_time = 0;

public:

    // Initializes program and GUI
    AEAnimator()
    {
        world.SetupOrgFile("example.data").SetTimingRepeat(10);

        doc << canvas;
        tracking << text_canvas;
        buttons << GetToggleButton("Toggle");
        buttons << GetStepButton("Step");

        // apply configuration query params and config files to config
        auto specs = emp::ArgManager::make_builtin_specs(&config);
        emp::ArgManager am(emp::web::GetUrlParams(), specs);
        am.UseCallbacks();
        if (am.HasUnused())
            std::exit(EXIT_FAILURE);

        InitializeParameters();
        InitializeConfigPanel();
        world.AddOrganisms(config.INITIAL_ORG_NUM());
    }

    // Initializes the values of all variables dependent on parameters
    void InitializeParameters() {
        random.ResetSeed(config.SEED());
        world.SetGlobalMutationRate(config.MUT_PROB());
        repoToggle = config.SR();
        num_w_boxes = config.WORLD_WIDTH();        
        num_h_boxes = config.WORLD_HEIGHT();
        width = num_w_boxes * RECT_SIDE;
        height = num_h_boxes * RECT_SIDE;
        total_boxes = num_w_boxes * num_h_boxes;
        canvas.SetSize(width, height);
        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
    }

    // Sets up the config panel
    void InitializeConfigPanel() {
        emp::prefab::ConfigPanel config_panel(config);
        settings << config_panel;

        config_panel.SetRange("INITIAL_ORG_NUM", "1", std::to_string(total_boxes));
        config_panel.SetRange("SR", "0", "1", "1");
        config_panel.SetRange("WORLD_WIDTH", "1", "100", "1");
        config_panel.SetRange("WORLD_HEIGHT", "1", "100", "1");
        config_panel.SetRange("MUT_PROB", "0", "1", "0.001");
        config_panel.ExcludeSetting("FILE_NAME");
        config_panel.ExcludeSetting("FILE_PATH");
        settings.SetCSS("max-width", "500px");
        config_panel.SetRange("INITIAL_ORG_NUM", "1", std::to_string(total_boxes));
    }

    // Creates the GUI for a single time instance in the program
    void DoFrame() override
    {
        canvas.Clear();
        text_canvas.Clear();
        world.Update();
        current_time++;
        DrawOrganisms();
        if (repoToggle == 1)
        {
            world.moveOrg();
        }
        DrawText();
    }

    // Draws the grid and organsisms in the program
    void DrawOrganisms() {
        int org_num = 0;
        for (int x = 0; x < num_w_boxes; x++)
        {
            for (int y = 0; y < num_h_boxes; y++)
            {
                if (world.IsOccupied(org_num))
                {
                    if (world.GetOrg(org_num).GetNumTasksSolved() == 0)
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "black", "black");
                    }
                    else if(world.GetOrg(org_num).GetEQESolved()){
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "violet", "black");
                    }  
                    else if (world.GetOrg(org_num).GetXORSolved())
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "indigo", "black");
                    }
                    else if (world.GetOrg(org_num).GetNORSolved())
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "blue", "black");
                    }
                    else if (world.GetOrg(org_num).GetANDNSolved())
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "green", "black");
                    }
                    else if (world.GetOrg(org_num).GetORSolved())
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "yellow", "black");
                    }
                    else if (world.GetOrg(org_num).GetORNSolved())
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "orange", "black");
                    }
                    else if (world.GetOrg(org_num).GetANDSolved())
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "red", "black");
                    }
                    else if (world.GetOrg(org_num).GetNANDSolved())
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "maroon", "black");
                    }
                    else if (world.GetOrg(org_num).GetNOTSolved())
                    {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "grey", "black");
                    }
                }
                else
                {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
                }
                org_num++;
            }
        }
    }

    // Returns a string indicating whether or not the extinction event has occurred
    std::string GetExctinctionOccurred() {
        if(current_time >= 100000) {
            return "True";
        }
        return "False";
    }

    // Draws the informational text about population data
    void DrawText() {
        text_canvas.CenterText(200.0, 20.0, "Current time instance: " + std::to_string(current_time));
        text_canvas.CenterText(200.0, 40.0, "Extinction event has occurred: " + GetExctinctionOccurred());
        text_canvas.CenterText(200.0, 60.0, "Total organisms: " + std::to_string(world.GetPopulationSize()));
        text_canvas.CenterText(200.0, 80.0, "Total tasks solved by organisms in this instance (Functional Activity): " + std::to_string(world.GetSumTotalTasksSolved()));
        text_canvas.CenterText(200.0, 100.0, "Number of organisms who have solved the equals task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(0)));
        text_canvas.CenterText(200.0, 120.0, "Number of organisms who have solved the xor task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(1)));
        text_canvas.CenterText(200.0, 140.0, "Number of organisms who have solved the nor task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(2)));
        text_canvas.CenterText(200.0, 160.0, "Number of organisms who have solved the andn task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(3)));
        text_canvas.CenterText(200.0, 180.0, "Number of organisms who have solved the or task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(4)));
        text_canvas.CenterText(200.0, 200.0, "Number of organisms who have solved the and task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(5)));
        text_canvas.CenterText(200.0, 220.0, "Number of organisms who have solved the orn task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(6)));
        text_canvas.CenterText(200.0, 240.0, "Number of organisms who have solved the nand task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(7)));
        text_canvas.CenterText(200.0, 260.0, "Number of organisms who have solved the not task: " + std::to_string(world.GetNumOrgsSolvedTaskAtIndex(8)));
    }
};

AEAnimator animator;

int main() { animator.Step(); }
