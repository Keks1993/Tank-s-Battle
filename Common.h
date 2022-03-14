#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <iterator>
#include <filesystem>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

namespace fs = std::filesystem;
namespace pt = boost::property_tree;
namespace ch = std::chrono;