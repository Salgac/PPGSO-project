
#include "window.cpp"

const unsigned int SIZE = 1024;

int main()
{
    // Create our window
    auto window = ProjectWindow{SIZE};
    // Main execution loop
    while (window.pollEvents())
    {
    }

    return EXIT_SUCCESS;
}
