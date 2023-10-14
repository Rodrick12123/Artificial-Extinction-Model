#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "OrgState.h"
#include "sgpl/library/OpLibraryCoupler.hpp"
#include "sgpl/library/prefab/ArithmeticOpLibrary.hpp"
#include "sgpl/operations/flow_global/Anchor.hpp"
#include "sgpl/program/Instruction.hpp"
#include "sgpl/program/Program.hpp"
#include "sgpl/spec/Spec.hpp"
#include "sgpl/library/prefab/NopOpLibrary.hpp"

struct IOInstruction
{
  template <typename Spec>
  static void run(sgpl::Core<Spec> &core, const sgpl::Instruction<Spec> &inst,
                  const sgpl::Program<Spec> &,
                  typename Spec::peripheral_t &state) noexcept
  {
    uint32_t output = core.registers[inst.args[0]];
    state.world->CheckOutput(output, state);
    uint32_t input = sgpl::tlrand.Get().GetUInt();
    // uint32_t input = (uint32_t)sgpl::tlrand.Get().GetDouble();
    core.registers[inst.args[0]] = input;
    state.add_input(input);
  }

  static std::string name() { return "IO"; }
  static size_t prevalence() { return 1; }
};

struct ReproduceInstruction
{
  template <typename Spec>
  static void run(sgpl::Core<Spec> &core, const sgpl::Instruction<Spec> &inst,
                  const sgpl::Program<Spec> &,
                  typename Spec::peripheral_t &state) noexcept
  {
    state.world->ReproduceOrg(state.current_location);
  }

  static std::string name() { return "Reproduce"; }
  static size_t prevalence() { return 1; }
};

struct NAND {
    template <typename Spec>
    static void run(sgpl::Core<Spec> &core, const sgpl::Instruction<Spec> &inst,
                  const sgpl::Program<Spec> &,
                  typename Spec::peripheral_t &state) noexcept 
    {
        uint32_t reg_b = core.registers[inst.args[1]];
        uint32_t reg_c = core.registers[inst.args[2]];
        uint32_t nand_val = ~(reg_b & reg_c);
        core.registers[inst.args[0]] = nand_val;
    }
    static std::string name() { return "NAND"; }
    static size_t prevalence() { return 1; }
};

struct NOT {
    template <typename Spec>
    static void run(sgpl::Core<Spec> &core, const sgpl::Instruction<Spec> &inst,
                  const sgpl::Program<Spec> &,
                  typename Spec::peripheral_t &state) noexcept 
    {
        uint32_t reg_b = core.registers[inst.args[1]];
        uint32_t not_val = ~(reg_b);
        core.registers[inst.args[0]] = not_val;
    }
    static std::string name() { return "NOT"; }
    static size_t prevalence() { return 1; }
};

//NopOpLibrary is not in here
using Library =
    sgpl::OpLibraryCoupler<sgpl::NopOpLibrary, sgpl::BitwiseShift, 
                           sgpl::Increment, sgpl::Decrement, sgpl::Add, 
                           sgpl::Subtract, sgpl::global::JumpIfNot, 
                           sgpl::local::JumpIfNot, sgpl::global::Anchor, NAND, NOT, IOInstruction, ReproduceInstruction>;

using Spec = sgpl::Spec<Library, OrgState>;

#endif
