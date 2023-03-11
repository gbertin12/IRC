#include <csignal>
#include <iostream>
#include <string>
#include "bot.hpp"

bool sigint_bot = false;

static void handle_sigint_bot(int)
{
    std::cout << "SIGINT detected" << std::endl;
    sigint_bot = true;
}

int main(int argc, char const *argv[])
{
    if (argc == 3)
    {
        try
        {
            Bot *bot = new Bot(argv[1], argv[2]);
            signal(SIGINT, handle_sigint_bot);
            bot->run();
            delete bot;
            return 0;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            delete bot;
            return 1;
        }
    }
    else
    {
        std::cerr << "Usage: ./bot <port> <password>" << std::endl;
        return 1;
    }
}