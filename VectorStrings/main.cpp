#include "pixel.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

void average_colors(std::vector<Pixel> &pixel_list) 
{
  if (pixel_list.empty()) // Check if the pixel list is empty
  {
    std::cerr << "No pixels were loaded; averages cannot be calculated.\n"; // Print an error message
    return;
  }

  double red_total = 0.0; // Initialize a variable to accumulate the total red color value
  double green_total = 0.0; // Initialize a variable to accumulate the total green color value
  double blue_total = 0.0; // Initialize a variable to accumulate the total blue color value

  for (const Pixel &pixel : pixel_list) // Iterate through each pixel in the pixel list
  {
    red_total += pixel.r; // Accumulate the red color value of the current pixel
    green_total += pixel.g; // Accumulate the green color value of the current pixel
    blue_total += pixel.b; // Accumulate the blue color value of the current pixel
  }

  const double pixel_count = static_cast<double>(pixel_list.size());
  std::cout << std::setprecision(6)
            << "Average red: " << red_total / pixel_count << '\n' // Print the average red color value
            << "Average green: " << green_total / pixel_count << '\n' // Print the average green color value
            << "Average blue: " << blue_total / pixel_count << '\n'; // Print the average blue color value
}

void flip_vertically(std::vector<Pixel> &pixel_list)
{
  if (pixel_list.empty())
  {
    return;
  }

  const auto y_bounds = std::minmax_element( // Find the minimum and maximum y coordinates in the pixel list
      pixel_list.begin(), pixel_list.end(),
      [](const Pixel &left, const Pixel &right) { return left.y < right.y; });
  const int top_y = y_bounds.first->y;
  const int bottom_y = y_bounds.second->y;

  for (Pixel &pixel : pixel_list) // Iterate through each pixel in the pixel list
  {
    pixel.y = top_y + bottom_y - pixel.y;
  }

  // Restore ascending coordinate order after the y coordinates are remapped.
  std::sort(pixel_list.begin(), pixel_list.end(),
            [](const Pixel &left, const Pixel &right) {
              if (left.x != right.x)
              {
                return left.x < right.x;
              }
              return left.y < right.y;
            });
}

int main(int argc, char *argv[]) 
{
  if (argc != 2) // Check if the number of command-line arguments is correct
  {
    std::cerr << "Usage: " << argv[0] << " <input-file>\n"; // Print usage information if the number of command-line arguments is incorrect
    return 1;
  }

  std::ifstream input_file(argv[1]); // Open the input file specified as a command-line argument
  if (!input_file)
  {
    std::cerr << "Could not open input file: " << argv[1] << '\n'; // Print an error message if the input file cannot be opened
    return 1;
  }

  std::vector<Pixel> pixel_list;
  std::string line;
  std::size_t line_number = 0;

  try
  {
    while (std::getline(input_file, line)) // Read each line from the input file
    {
      ++line_number;
      Pixel pixel{};
      std::string fields[5];
      std::size_t field_start = 0;

      // find() locates each comma and substr() extracts the field before it;
      // the final field is the remainder. stoi/stof convert those strings.
      for (int field = 0; field < 4; ++field)
      {
        const std::size_t comma = line.find(',', field_start); // Find the position of the next comma in the line
        if (comma == std::string::npos)
        {
          throw std::runtime_error("expected five comma-separated values"); // Throw an error if there are not enough comma-separated values in the line
        }
        fields[field] = line.substr(field_start, comma - field_start); // Extract the field before the comma and store it in the fields array
        field_start = comma + 1;
      }
      if (line.find(',', field_start) != std::string::npos) // Check if there are more than five values in the line
      {
        throw std::runtime_error("found more than five values");
      }
      fields[4] = line.substr(field_start);

      pixel.x = std::stoi(fields[0]);
      pixel.y = std::stoi(fields[1]);
      pixel.r = std::stof(fields[2]);
      pixel.g = std::stof(fields[3]);
      pixel.b = std::stof(fields[4]);
      pixel_list.push_back(pixel);
    }
  }
  catch (const std::exception &error)
  {
    std::cerr << "Invalid pixel data on line " << line_number << ": "
              << error.what() << '\n';
    return 1;
  }

  average_colors(pixel_list);
  flip_vertically(pixel_list);

  std::ofstream output_file("flipped.dat"); 
  if (!output_file)
  {
    std::cerr << "Could not create flipped.dat\n"; // Print an error message if the output file cannot be created
    return 1;
  }

  output_file << std::setprecision(std::numeric_limits<float>::max_digits10); // Set the precision of the output file to the maximum number of digits for a float
  for (const Pixel &pixel : pixel_list)
  {
    output_file << pixel.x << ',' << pixel.y << ',' << pixel.r << ','
                << pixel.g << ',' << pixel.b << '\n'; // Write the pixel data to the output file in the specified format
  }

  return 0;
}