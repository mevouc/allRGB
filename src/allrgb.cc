#include <boost/program_options.hpp>
#include <iostream>

#include <allrgb.hh>
#include <transformer.hh>

namespace po = boost::program_options;

int
main(int argc, char* argv[])
{
  po::options_description desc("Usage");

  desc.add_options()
    ("help,h", "Display this message")
    ("input,i", po::value<std::string>(), "Input image")
    ("output,o", po::value<std::string>(), "Output image")
    ("check,c", po::value<std::string>(), "Image to check (does not work with"
    " other options)");

  po::variables_map vm;
  try
  {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << std::endl
              << desc << std::endl;
    return 1;
  }

  std::string input;
  std::string output("out.png");

  if (vm.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
  }
  else if (vm.count("input"))
    input = vm["input"].as<std::string>();
  else if (vm.count("check"))
    return allrgb::check(vm["check"].as<std::string>()) ? 0 : 1;
  else
  {
    std::cout << "option '--input' has to be specified" << std::endl
              << desc << std::endl;
    return 1;
  }

  if (vm.count("output"))
    output = vm["output"].as<std::string>();

  return allrgb::run(input, output);
}

int
allrgb::run(const std::string& input, const std::string& output)
{
  std::cout << "Input file is: " << input << std::endl
            << "Output file is: " << output << std::endl;

  cv::Mat input_img = cv::imread(input);

  cv::Mat square4096;
  cv::resize(input_img, square4096, cv::Size(4096, 4096)); 

  Transformer tsfm(square4096);

  tsfm();

  cv::imwrite(output, tsfm.img_get());

  return 0;
}

bool
allrgb::check(const std::string& input)
{
  bool res = true;// TODO
  if (res)
    std::cout << "'" << input << "' does contain all RGB colors only once."
              << std::endl;
  else
    std::cout << "'" << input << "' does contain all RGB colors only once."
              << std::endl;
  return res;
}
