#ifndef BALLANCEMMOSERVER_CONSOLE_HPP
#define BALLANCEMMOSERVER_CONSOLE_HPP
#include "../entity/map.hpp"
#include "../utility/command_parser.hpp"
#include <string>
#include <functional>
#include <map>
#include <mutex>

namespace bmmo {
class console {
    bmmo::command_parser parser_;
    std::map<std::string, std::function<void()>> commands_;
    std::string command_name_;
    std::mutex console_mutex_;

public:
    console();

    static inline console *instance;

    const std::string get_help_string() const;
    const std::vector<std::string> get_command_list() const;
    // @param cmd - current command name/prefix, `nullptr` -> local command name.
    // @returns a sorted `std::vector` containing matching command names.
    const std::vector<std::string> get_command_hints(bool fuzzy_matching = false, const char *cmd = nullptr) const;
    
    static void set_completion_callback(std::function<std::vector<std::string>(const std::vector<std::string>&)> func);

    // @returns `true` if the stream doesn't have any errors.
    static bool read_input(std::string &buf);
    static void end_input();
    bool execute(const std::string &cmd);

    // execute a command asynchronously.
    // keeps in mind that this means you won't be able to get the return value.
    void execute_async(const std::string &cmd);

    bool register_command(const std::string &name, const std::function<void()> &handler);
    bool register_aliases(const std::string &name, const std::vector<std::string> &aliases);
    bool unregister_command(const std::string &name);

    bool empty() const noexcept;
    inline const std::string &get_command_name() const noexcept { return command_name_; };

    std::string get_next_word(bool to_lowercase = false);
    std::string get_rest_of_line();

    // style: `level <number> [name]` or `<hash> <number> [name]`.
    // @returns a `bmmo::named_map` which can be cast into a `bmmo::map`.
    bmmo::named_map get_next_map(bool with_name = false);
    // style: client id number, optionally prefixed with a `#`.
    uint32_t get_next_client_id();

    inline int32_t get_next_int() { return std::atoi(get_next_word().c_str()); };
    inline int64_t get_next_long() { return std::atoll(get_next_word().c_str()); };
    inline double get_next_double() { return std::atof(get_next_word().c_str()); };
};

}

#endif // BALLANCEMMOSERVER_CONSOLE_HPP
