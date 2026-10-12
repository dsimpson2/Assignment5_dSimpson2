#include "pixel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


// Calculate average RGB values
void average_colors(std::vector<Pixel> &pixel_list)
{
    if (pixel_list.empty())
    {
        std::cout << "No pixels loaded.\n";
        return;
    }

    double r_total = 0;
    double g_total = 0;
    double b_total = 0;

    for (const Pixel &p : pixel_list)
    {
        r_total += p.r;
        g_total += p.g;
        b_total += p.b;
    }

    double count = pixel_list.size();

    std::cout << "Average red:   " << r_total / count << "\n";
    std::cout << "Average green: " << g_total / count << "\n";
    std::cout << "Average blue:  " << b_total / count << "\n";
}

// Flip pixels vertically
void flip_vertically(std::vector<Pixel> &pixel_list) // Function to flip the pixels vertically
{
    if (pixel_list.empty())
        return;

    // Find min and max Y values
    int min_y = pixel_list[0].y;
    int max_y = pixel_list[0].y;

    for (const Pixel &p : pixel_list)
    {
        if (p.y < min_y) min_y = p.y;
        if (p.y > max_y) max_y = p.y;
    }

    // Flip each pixel
    for (Pixel &p : pixel_list)
    {
        p.y = min_y + max_y - p.y;
    }

    // Sort pixels again by x and then by y to maintain order
    std::sort(pixel_list.begin(), pixel_list.end(),
              [](const Pixel &a, const Pixel &b)
              {
                  if (a.x != b.x) return a.x < b.x;
                  return a.y < b.y;
              });
}

int main(int argc, char *argv[]) // Main function to read pixel data from a file, process it, and write the output to a new file
{
    if (argc != 2) // Check if the correct number of command-line arguments is provided
    {
        std::cout << "Usage: " << argv[0] << " <input-file>\n"; // Print usage message if the number of arguments is incorrect
        return 1;
    }

    std::ifstream input(argv[1]); // Open the input file specified in the command-line arguments
    if (!input.is_open())
    {
        std::cout << "Could not open input file.\n";
        return 1;
    }

    std::vector<Pixel> pixel_list;
    std::string line;
    int line_number = 0;

    while (std::getline(input, line)) // Read each line from the input file
    {
        line_number++;

        Pixel p;
        std::string fields[5];
        int field_index = 0;
        size_t start = 0;

        // Extract comma-separated values
        for (int i = 0; i < 4; i++)
        {
            size_t comma = line.find(',', start);
            if (comma == std::string::npos)
            {
                std::cout << "Invalid data on line " << line_number << "\n";
                return 1;
            }
            fields[i] = line.substr(start, comma - start);
            start = comma + 1;
        }

        fields[4] = line.substr(start);

        // Convert strings to numbers
        p.x = std::stoi(fields[0]);
        p.y = std::stoi(fields[1]);
        p.r = std::stof(fields[2]);
        p.g = std::stof(fields[3]);
        p.b = std::stof(fields[4]);

        pixel_list.push_back(p);
    }

    // Process data
    average_colors(pixel_list);
    flip_vertically(pixel_list);

    // Write output file
    std::ofstream output("flipped.dat");
    if (!output.is_open())
    {
        std::cout << "Could not create output file.\n";
        return 1;
    }

    for (const Pixel &p : pixel_list)
    {
        output << p.x << "," << p.y << ","
               << p.r << "," << p.g << "," << p.b << "\n"; // Write each pixel's data to the output file
    }

    return 0;
}
