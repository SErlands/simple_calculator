#include "Simple_calculator.h"
#include <iostream>
#include <string>
#include <algorithm> //For making lowercase string
#include <cctype> //For making lowecase string
#include <sstream>
#include <fstream> //For taking in input_file
#include <stdio.h>

using namespace std;

// ------ Calculator --------
void Calculator::run()
{
    do{
        get_command();
        handle_input();
        
        // If we shall quit
        if(exit)
            break;
    } while (1);
    return;
}

void Calculator::run(string file_name)
{
    ifstream in(file_name);
    
    if(!in)
    {
        cout << "Cannot open file: " << file_name << "\n" << "Exiting\n";
    }
    else
    {
        while (getline(in, command_string))
        {
            // Make string all lowecase
            transform(command_string.begin(), command_string.end(), command_string.begin(),
                    [](unsigned char c){ return std::tolower(c); }); 
            //Quick easy fix to make sure that command_string end with ' ' so 
            //extract_expression gets simpler
            command_string += ' ';

            handle_input();

            if(exit)
                break;
        }
    }
    return;
}

void Calculator::handle_input()
{
        extract_expression();
        
        //Check for quit
        if(left_command.compare("quit") == 0)
        {
           exit = 1; 
        }
        //Check for valid left_operand
        else if(is_int(left_command))
            cout << "Left operand cant be a value, left_operand: " << left_command << "\n";
        //Check if we shall print
        else if(left_command.compare("print") == 0)
        {
           print_expression(); 
        }
        //Do operation 
        else if(is_int(right_command) && is_operator(middle_command))
        {
            reg_op_value();
        }
        //Save operation
        else if(!is_int(right_command) && is_operator(middle_command))
        {
            reg_op_reg();
        }
        else
            cout << "Invalid input: " << left_command << " " << middle_command << " " << right_command << "\n";

        if(error_happned != 0)
        {
           cout << "ERROR nr: " << error_happned << " Exiting\n";
           exit = 1; 
        }
}

void Calculator::get_command()
{
    cout << "Enter command: ";

    //Make string all lowercase
    getline(cin, command_string);
    transform(command_string.begin(), command_string.end(), command_string.begin(),
            [](unsigned char c){ return std::tolower(c); }); 
    //Quick easy fix to make sure that command_string end with ' ' so 
    //extract_expression gets simpler
    command_string += ' ';
    return;
}

void Calculator::extract_expression()
{   
    int count = 0;  
    bool error = false;
    string tmp = "";
    
    left_command = "";
    middle_command = "";
    right_command = "";

    // Pick out words in a string, char by char
    for (auto x : command_string) 
    { 
        if (x == ' ') 
        { 
            switch (count)
            {
                case 0: left_command = tmp;     break;
                case 1: middle_command = tmp;   break;
                case 2: right_command = tmp;    break;
                default: error = true; break; //If there are more than three words throw error
            } 
            count++;
            tmp = ""; 
        } 
        else
        { 
            tmp = tmp + x; 
        } 
    }

    if(error)
    {
        cout << "WARNING: Too many inputs, only first three will bee used\n";
        return;
    }
}

void Calculator::print_expression()
{
    list<Integer>::iterator middle_op = possition_in_list(middle_command);

    calculate_at_print(middle_op);

    // Dont print out value if error occured
    if(error_happned)
        return;

    cout << middle_op->get_value() << "\n";
    return;
}

void Calculator::reg_op_value()
{
    int right_value = stoi(right_command);
    list<Integer>::iterator left_op = possition_in_list(left_command);
    // Long line but it simply takes all the values needed, calculates them and store
    left_op->set_value(calculate(left_op->get_value(), right_value, middle_command));
    return;
}

void Calculator::reg_op_reg()
{
    // Create Integer-objects
    list<Integer>::iterator left_op = possition_in_list(left_command);
    list<Integer>::iterator right_op = possition_in_list(right_command);
    //Store string to be evaluated at print
    left_op->add_at_print(command_string);
    return;
}

list<Integer>::iterator Calculator::possition_in_list(const string& command)
{
    // Search allready existing operands
    list<Integer>::iterator it;
    for(it = operands.begin(); it != operands.end(); ++it)
    {
        if(it->get_name() == command)
            return it;
    }
    // If no obejct in list has the name command, add to list
    operands.push_back(Integer(command));
    it = operands.end();
    --it;
    return it;
}

bool Calculator::is_int(const string& i_string)
{
    string::const_iterator it = i_string.begin();
    while (it != i_string.end() && std::isdigit(*it)) 
        ++it; 
    return !i_string.empty() && it == i_string.end();
}

int Calculator::calculate(const int left_value, const int right_value, const string op)
{
    if(op == "add")
        return left_value + right_value;
    else if(op == "subtract")
        return left_value - right_value;
    else if(op == "multiply")
        return left_value * right_value;
    //
    //Here another operand can be added.
    //Just make sure that special cases is handled correctly
    //For example division by 0, set error_happned to value
    //if something goes wrong.
    //
    //Remeber to add the new operator to is_op() also
    //
    else
    {
        error_happned = 1;
        return 0;
    }
}

void Calculator::calculate_at_print(list<Integer>::iterator op)
{
    command_string = op->get_ev_at_print();

    while(command_string != " \n ")
    {
        extract_expression();
        
        // Store old commands so they wont be overwriten
        string store_left_command = left_command;
        string store_middle_command = middle_command;

        // Take out the right_reg and call the same function
        list<Integer>::iterator right_op = possition_in_list(right_command);
        calculate_at_print(right_op);

        //Restore commands
        left_command    =  store_left_command; 
        right_command   =  to_string(right_op->get_value()); 
        middle_command  =  store_middle_command;
        
        //Evaluat expression
        reg_op_value();
        
        // Break loop if error happned
        if(error_happned != 0)
        {
            break;
        }

        command_string = op->get_ev_at_print();
    }
    
    return;  
}

bool Calculator::is_operator(const string& i_string)
{
    // add to list of new operators are created
    return (i_string == "add" || i_string == "subtract" || i_string == "multiply");
}
// -------------------------
// -------- Integer --------
Integer::Integer(string new_name) : value{0}, name{new_name}
{}

void Integer::print_value()
{
    stringstream ss;
    ss << value;
    cout << "print Value, Fix this" << "\n";
    return;
}

void Integer::set_value(int new_value)
{
    value = new_value;
}

int Integer::get_value()
{
    return value;
}

string Integer::get_name()
{
    return name;
}

void Integer::add_at_print(string expression)
{
    ev_at_print.push_back(expression);
    return;
}

string Integer::get_ev_at_print()
{
    string tmp = ev_at_print.back();
    // Dont take out the end of list marker
    if(tmp != " \n ")
        ev_at_print.pop_back();
    return tmp;
}
// -------------------------

int main(int argc, char *argv[])
{
    
    Calculator calc;
    
    if(argc == 1)
    {
        calc.run();
    }
    else if(argc == 2)
    {
        calc.run(argv[1]);
    }
    else
        cout << "Can only handle one inputfile \n";
      
    return 0;
}
