#include <iostream>
#include <string>
#include "CLI11.hpp"
#include "Sites.h"
#include "Triangulator.h"
using namespace std;

int main(int argc, const char* argv[])
{
  CLI::App app;

  string input_path;
  CLI::Option* path_option = app.add_option("--input", input_path,
                                            "Input file name.");
  path_option->required();
  path_option->check(CLI::ExistingFile);

  int alg_number;
  CLI::Option* alg_option = app.add_option("--alg", alg_number,
                                           "Algorithm number. 1 for vertical, 2 for alternation.");

  alg_option->required();
  alg_option->check(CLI::Range(1, 2));

  // Parse Commandline Options
  CLI11_PARSE(app, argc, argv);

  exactinit();
  Sites sites = Sites(input_path);

  /// Perform triangulation
  Triangulator triangulation = Triangulator(sites, alg_number);
  vector<array<int, 3>>  triangles = triangulation.triangles;

  // /// Write to ele file
  size_t found = input_path.find_last_of("/\\");
  string filename = input_path.substr(found + 1);
  filename.replace(filename.end()-4, filename.end(), "ele");

  ofstream elefile;
  elefile.open (filename);
  elefile << triangles.size() << " " << "3" << endl;
  int tri_id = 1;
  for(auto &it : triangles){
    elefile << tri_id << " " << it[0] << " " << it[1] << " " << it[2] << endl;
    tri_id = tri_id + 1;
  }
  elefile.close();
  return 0;
}
