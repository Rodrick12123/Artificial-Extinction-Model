#ifndef TASK_H
#define TASK_H

#include <cmath>
#include <string>
#include <iostream>

class Task
{
public:
    virtual double CheckOutput(uint32_t output, uint32_t inputs[4]) = 0;
    virtual std::string name() const = 0;
};

// A task representing the logical operand equals
class EQETask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate EQE of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (i != j)
                {   
                    uint32_t val = ~(inputs[i] ^ inputs[j]);
                    if (val == output)
                    {
                        // 16 points for success
                        return 16.0;
                    }
                }
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "EQE"; }
};

// A task representing the logical operand xor
class XORTask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate XOR of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (i != j)
                {
                    uint32_t val = inputs[i] ^ inputs[j];
                    if (val == output)
                    {
                        // 8 point for success
                        return 8.0;
                    }
                }
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "XOR"; }
};

// A task representing the logical operand nor
class NORTask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate NOR of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (i != j)
                {
                    uint32_t val = ~(inputs[i] | inputs[j]);
                    if (val == output)
                    {
                        // 8 points for success
                        return 8.0;
                    }
                }
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "NOR"; }
};

// A task representing the logical operand andn
class ANDNTask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate ANDN of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (i != j)
                {
                    uint32_t val = (inputs[i] & ~(inputs[j]));
                    if (val == output)
                    {
                        // 4 points for success
                        return 4.0;
                    }
                }
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "ANDN"; }
};

// A task representing the logical operand or
class ORTask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate OR of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (i != j)
                {
                    uint32_t val = (inputs[i] | inputs[j]);
                    if (val == output)
                    {
                        // 4 point for success
                        return 4.0;
                    }
                }
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "OR"; }
};

// A task representing the logical operand orn
class ORNTask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate ORN of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (i != j)
                {
                    uint32_t val = inputs[i] | ~(inputs[j]);
                    if (val == output)
                    {
                        // 2 point for success
                        return 2.0;
                    }
                }
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "ORN"; }
};

// A task representing the logical operand and
class ANDTask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate AND of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (i != j)
                {
                    uint32_t val = (inputs[i] & inputs[j]);
                    if (val == output)
                    {
                        // 2 points for success
                        return 2.0;
                    }
                }
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "AND"; }
};

// A task representing the logical operand nand
class NANDTask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate NAND of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (i != j)
                {
                    uint32_t val = ~(inputs[i] & inputs[j]);
                    if (val == output)
                    {
                        // 1 point for success
                        return 1.0;
                    }
                }
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "NAND"; }
};

// A task representing the logical operand not
class NOTTask : public Task
{
public:
    double CheckOutput(uint32_t output, uint32_t inputs[4]) override
    {
        // did they calculate NOT of a number?
        if(output == 0) {return 0.0;}
        for (int i = 0; i < 4; i++)
        {
            uint32_t not_val = ~inputs[i]; //~ is bitwise NOT in C++
            if (not_val == output)
            {
                // 1 point for success
                return 1.0;
            }
        }
        // 0 points for failure
        return 0.0;
    }

    std::string name() const override { return "NOT"; }
};

#endif
