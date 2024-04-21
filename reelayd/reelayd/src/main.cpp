
#include "reelay/monitors.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "simdjson.h"
#include "zenoh.h"

namespace reelay {

template<typename T>
struct timefield<T, simdjson::dom::element> {
  using input_t = simdjson::dom::element;
  inline static T get_time(const input_t& container)
  {
    return container.at_key("time");
  }
};

template<>
struct datafield<simdjson::dom::element> {
  using input_t = simdjson::dom::element;

  inline static input_t at(const input_t& container, const std::string& key)
  {
    return container.at_key(key);
  }

  inline static input_t at(const input_t& container, std::size_t index)
  {
    return container.at(index);
  }

  inline static bool contains(const input_t& container, const std::string& key)
  {
    for(simdjson::dom::object::iterator field =
          simdjson::dom::object(container).begin();
        field != simdjson::dom::object(container).end();
        ++field) {
      if(key == field.key()) {
        return true;
      }
    }
    return false;
  }

  inline static bool as_bool(const input_t& container, const std::string& key)
  {
    return container.at_key(key).get<bool>();
  }

  inline static int64_t as_integer(
    const input_t& container, const std::string& key)
  {
    return container.at_key(key).get<int64_t>();
  }

  inline static double as_floating(
    const input_t& container, const std::string& key)
  {
    return container.at_key(key).get<double>();
  }

  inline static std::string as_string(
    const input_t& container, const std::string& key)
  {
    std::string_view sv = container.at_key(key).get<std::string_view>();
    return std::string(sv);
  }

  inline static bool contains(const input_t& container, std::size_t index)
  {
    return index < simdjson::dom::array(container).size();
  }

  inline static bool as_bool(const input_t& container, std::size_t index)
  {
    return container.at(index).get<bool>();
  }

  inline static int as_integer(const input_t& container, std::size_t index)
  {
    return container.at(index).get<int64_t>();
  }

  inline static double as_floating(const input_t& container, std::size_t index)
  {
    return container.at(index).get<double>();
  }

  inline static std::string as_string(
    const input_t& container, std::size_t index)
  {
    std::string_view sv = container.at(index).get<std::string_view>();
    return std::string(sv);
  }
};

}  // namespace reelay

namespace ry = reelay;
namespace sj = simdjson;
namespace po = boost::program_options;

using input_t = simdjson::dom::element;
using output_t = reelay::json;

simdjson::dom::parser parser;
simdjson::dom::element json_element;

void monitor_on_sample(const z_sample_t* sample, void* context)
{
  auto* monitor = (reelay::monitor<input_t, output_t>*)context;
  auto json_string = simdjson::padded_string(
    (const char*)sample->payload.start, sample->payload.len);
  json_element = parser.parse(json_string);
  auto result = monitor->update(json_element);
  if(not result.empty()) {
    std::cout << result << std::endl;
  }
}

int main(int argc, char** argv)
{
  // argparse
  po::variables_map vm;
  po::options_description cmdline("Generic options");
  cmdline.add_options()                                                //
    ("help", "produce help message")                                   //
    ("key,k",                                                          //
     po::value<std::string>()->default_value("bounverif/timescales"),  //
     "specify the zenoh key exprression to subscribe")                         //
    ("expr,r", po::value<std::string>(), "specify reelay expression to monitor") //
    ("config,c", po::value<std::string>(), "specify config filepath")  //
    ;
  auto parsed_cmd = po::parse_command_line(argc, argv, cmdline);
  po::store(parsed_cmd, vm);
  po::notify(vm);

  if(vm.count("help") != 0) {
    std::cout << cmdline << "\n";
    return EXIT_SUCCESS;
  }

  // monitor construction
  using input_t = simdjson::dom::element;
  using output_t = reelay::json;
  auto manager = std::make_shared<reelay::binding_manager>();
  auto monitor = reelay::monitor<input_t, output_t>();

  auto monitor_opts =
    reelay::discrete_timed<int64_t>::monitor<input_t, output_t>::options()
      .with_condensing(true)
      .with_data_manager(manager);

  auto ryexpr = vm["expr"].as<std::string>();
  monitor = reelay::make_monitor(ryexpr, monitor_opts);

  // zenoh
  z_owned_config_t config = z_config_default();
  z_owned_session_t session = z_open(z_move(config));

  if(!z_check(session)) {
    printf("Unable to open session!\n");
    exit(-1);
  }

  const char* keyexpr = vm["key"].as<std::string>().c_str();
  z_owned_closure_sample_t callback =
    z_closure(monitor_on_sample, nullptr, &monitor);
  z_owned_subscriber_t sub = z_declare_subscriber(
    z_loan(session), z_keyexpr(keyexpr), z_move(callback), nullptr);

  int c = 0;
  while(c != 'q') {
    c = getchar();
    if(c == -1) {
      sleep(1);
    }
  }

  z_undeclare_subscriber(z_move(sub));
  z_close(z_move(session));

  return 0;
}
