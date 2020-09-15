#include <string>
#include <vector>

enum option{user,program,invalid};

inline bool exists_file (const std::string& name);

class cmd{

    std::vector<std::string> hosts;
    option op;
    std::string command;
    bool contains_path; //if the command has a path in it
    std::string device; //device on which the path exists

    public:

    cmd(std::string input);

    std::vector<std::string> get_hosts();
    option get_option();
    std::string get_command();
    bool is_path();
    std::string get_device_in_path();
    std::string get_string();
    
};
