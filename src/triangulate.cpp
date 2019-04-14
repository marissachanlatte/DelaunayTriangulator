#include "CLI11.hpp"
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
                                           "Algorithm number. 1 for Guibas-Stolfi, 2 for alternation.");

  alg_option->required();
  alg_option->check(CLI::Range(1, 2));

  // Parse Commandline Options
  CLI11_PARSE(app, argc, argv);

  return 0;
}
