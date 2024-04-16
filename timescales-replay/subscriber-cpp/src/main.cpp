#include "boost/program_options.hpp"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <string_view>

#include "zenoh.h"

namespace po = boost::program_options;

void listener_callback(const z_sample_t* sample, void* arg)
{
  printf(">> [Subscriber] Received '%.*s')\n", (int)sample->payload.len, sample->payload.start);
}

int main(int argc, char** argv)
{
  po::variables_map vm;
  po::options_description cmdline("Generic options");
  cmdline.add_options()                                                //
    ("help", "produce help message")                                   //
    ("key,k",                                                          //
      po::value<std::string>()->default_value("bounverif/timescales"), //
      "specify config filepath")                                       //
    ("config,c", po::value<std::string>(), "specify config filepath")  //
    ;
  auto parsed_cmd = po::parse_command_line(argc, argv, cmdline);
  po::store(parsed_cmd, vm);
  po::notify(vm);

  if(vm.count("help") != 0) {
    std::cout << cmdline << "\n";
    return EXIT_SUCCESS;
  }

  // zenoh
  z_owned_config_t config = z_config_default();
  z_owned_session_t session = z_open(z_move(config));

  const char* expr = vm["key"].as<std::string>().c_str();

  if(!z_check(session)) {
    std::cout << "Unable to open session!" << "\n";
    exit(-1);
  }

  z_owned_closure_sample_t callback = z_closure(listener_callback);
  z_owned_subscriber_t sub = z_declare_subscriber(
    z_loan(session), z_keyexpr(expr), z_move(callback), NULL
  );
  if (!z_check(sub)) {
    std::cout << "Unable to declare subscriber" << "\n";
    return EXIT_FAILURE;
  } else {
    std::cout << "Subscriber declared on " << expr << "\n";
  }

  int c = 0;
  while (c != 'q') {
    c = getchar();
    if (c == -1) {
      sleep(1);
    }
  }

  z_undeclare_subscriber(z_move(sub));
  z_close(z_move(session));

  return EXIT_SUCCESS;
}
