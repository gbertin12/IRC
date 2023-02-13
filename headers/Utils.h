#ifndef UTILS_H
# define UTILS_H

# define USER 0
# define OPERATOR 1
# define TIMEOUT 5000
# define MAX_CLIENTS 100
# define BUFFER_SIZE 1024

std::vector<std::string> separateCmd(std::string cmd);

#endif