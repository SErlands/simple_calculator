#include <string>
#include <list>

class Integer 
{
    public:
        Integer(std::string new_name);
        
        ~Integer() = default;

        // Prints value to cout
        void print_value();

        // Sets value to new_value
        void set_value(int new_value);

        // Returns value
        int get_value();

        // Returns name
        std::string get_name();
        
        // Add expression to ev_at_print
        void add_at_print(std::string expression);

        // Get the LAST string in ev_at_print AND delets it from the list
        std::string get_ev_at_print();
    private:
        int value;
        std::string name;
        
        // Create the ev_at_print list, adds " \n " too have a marker ti now 
        // when the list is empty
        std::list<std::string> ev_at_print{" \n "};
};


class Calculator
{
    public:
        Calculator() = default;
        ~Calculator() = default;
        // Run program, either with inputfile or not
        void run();
        void run(std::string file_name);

        // Main function to handle input, uses alot of the other functions
        void handle_input();

        // Read a line from cin and puts it in operands, clears cin after
        void get_command();

        // Diveds command_string into left_, right_ and middle_command
        void extract_expression();

        // Main function to print, uses calculate_at_print
        void print_expression();

        // Do: left_command->value = 
        //      left_command->value middle_command(+,-,*) right_command->value
        void reg_op_value();

        // Saves the current expression into left_commands->ev_at_print
        // Used for lazy evaluation at print
        void reg_op_reg();
        
        // Finds Integer in operands{}, if it dosnt exist it adds a new Integer with name=command and value=0
        std::list<Integer>::iterator possition_in_list(const std::string& command);  

        // Returns true if i_string is a int
        bool is_int(const std::string& i_string);

        // Dose the calculation depending on what operator middle_op is
        int calculate(const int left_value, const int right_value, const std::string op); 
        
        // Function to do the lazy evaluation at print
        void calculate_at_print(std::list<Integer>::iterator op);

        // Returns true if i_string is one of the implemented operators
        bool is_operator(const std::string& i_string);

    private:
        // Place to store current commands before evaluation
        std::string command_string{};
        std::string left_command{};
        std::string right_command{};
        std::string middle_command{};

        std::list<Integer> operands{};
        
        // Values used for exiting and detect errors
        int error_happned{};
        int exit{};
};

