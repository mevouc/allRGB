#include <boost/program_options.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include <allrgb.hh>
#include <transformer.hh>

namespace po = boost::program_options;

int
main(int argc, char* argv[])
{
  std::stringstream intro;
  intro << "Transform an image to its equivalent where each RGB color is "
           "present only once,"
        << std::endl
        << "or check that an image respect that predicate." << std::endl
        << std::endl << "Usage";
  po::options_description desc(intro.str());

  desc.add_options()
    ("help,h", "Display this message")
    ("input,i", po::value<std::string>(), "Input image")
    ("output,o", po::value<std::string>(), "Output image (need -i option)")
    ("check,c", po::value<std::string>(), "Image to check (ignored with -i)")
    ("linear,l", "Linear traversal of image")
    ("random,r", "Random traversal of image (default value, optional)")
    ("verbose,v", "Enable useful information output")
    ;

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
  std::string output("allrgb.tiff");

  const bool verbose = vm.count("verbose");

  if (vm.count("help"))
  {
    std::cout << desc << std::endl
              << "Examples:" << std::endl
              << "  allrgb -i example.jpg" << std::endl
              << "  allrgb --input example.jpg -o out.png --verbose"
              << std::endl
              << "  allrgb -i example.tiff --linear" << std::endl
              << "  allrgb --check example.bmp -v" << std::endl;
    return 1;
  }
  else if (vm.count("input"))
    input = vm["input"].as<std::string>();
  else if (vm.count("check"))
    return allrgb::check(vm["check"].as<std::string>(), verbose) ? 0 : 1;
  else
  {
    std::cout << "option '--input' has to be specified" << std::endl
              << desc << std::endl;
    return 1;
  }

  if (vm.count("output"))
    output = vm["output"].as<std::string>();

  return allrgb::run(input, output, !vm.count("linear"), verbose);
}

int
allrgb::run(const std::string& input, const std::string& output,
            const bool random, const bool verbose)
{
  if (verbose)
  {
    std::cout << "Input file is: '" << input << "'" << std::endl
              << "Output file is: '" << output << "'" << std::endl;
    if ((output.size() > 4 && output.substr(output.size() - 4, 4) == ".jpg")
        || (output.size() > 5 && output.substr(output.size() - 5, 5) == ".jpeg"))
    {
      std::cout << "WARNING: JPEG and other lossy compression formats should "
                   "be avoided for output image if you want a perfect allRGB "
                   "result."
                << std::endl;
    }
  }

  cv::Mat img = cv::imread(input);

  cv::Mat scaled = img.total() == 4096 * 4096 ? img : scale(img, verbose);
  Transformer tsfm(scaled, random);

  tsfm(verbose);

  cv::imwrite(output, tsfm.img_get());
  if (verbose)
    std::cout << "Output successfully written." << std::endl;

  return 0;
}

cv::Mat
allrgb::scale(const cv::Mat& img, const bool verbose)
{
  assert(img.dims == 2);
  cv::Mat scaled;

  if (verbose)
  {
    std::cout << "Initial image size: (" << img.cols << ", " << img.rows << ")"
              << std::endl;
    std::cout << "Scaling…" << std::endl;
  }

  std::vector<std::pair<const double, const cv::Size>> sizes;
  sizes.reserve(25);

  size_t nb_pix = 4096 * 4096;
  for (size_t w = 1; w <= nb_pix; w *= 2)
  {
    size_t h = nb_pix / w;
    sizes.emplace_back((double)w / h, cv::Size(w, h));
  }

  double scale = img.cols / img.rows;
  auto it = get_closest_size(sizes.begin(), sizes.end(), scale);
  const auto next = it + 1;

  if (next != sizes.end()
      && std::abs(scale - it->first) > std::abs(scale - next->first))
    it = next;

  if (verbose)
    std::cout << "Output size: (" << it->second.width << ", "
              << it->second.height << ")" << std::endl;
  cv::resize(img, scaled, it->second);
  return scaled;
}

allrgb::pair_vect_it
allrgb::get_closest_size(pair_vect_it begin, pair_vect_it end, double scale)
{
  if (begin >= end - 1)
    return begin;
  auto mid = begin + (end - begin) / 2;
  if (scale < mid->first)
    return get_closest_size(begin, mid, scale);
  return get_closest_size(mid, end, scale);
}

bool
allrgb::check(const std::string& input, const bool verbose)
{
  cv::Mat img = cv::imread(input);
  if (verbose)
    std::cout << "Input file is: '" << input << "'" << std::endl;

  bool res = check(img, verbose);

  if (verbose)
  {
    if (res)
      std::cout << "'" << input << "' does contain all RGB colors only once."
                << std::endl;
    else
      std::cout << "'" << input << "' does not contain all RGB colors only once."
                << std::endl;
  }
  return res;
}

bool
allrgb::check(const cv::Mat& img, const bool verbose)
{
  if (verbose)
    std::cout << "Checking: started" << std::endl;
  if (img.dims == 2 && (img.rows != img.cols || img.total() != 4096 * 4096))
  {
    if (verbose)
      std::cerr << "This image does not have " << 4096 * 4096 << " pixels."
                << std::endl << "Checking: done" << std::endl;
    ;
    return false;
  }

  bool res = true;
  std::unordered_set<size_t> colors(4096 * 4096);
  for (int y = 0; y < img.rows; ++y)
    for (int x = 0; x < img.cols; ++x)
    {
      cv::Vec3b pix = img.at<cv::Vec3b>(cv::Point(x, y));
      size_t color = pix[0] + (pix[1] << 8) + (pix[2] << 16);

      if (!colors.insert(color).second)
      {
        std::cerr << "Color (" << (int)pix[2] << ", " << (int)pix[1] << ", "
                  << (int)pix[0] << "), position (" << x << ", " << y
                  << "), is already present in this picture." << std::endl;
        res = false;
      }
    }

  if (verbose)
    std::cout << "Checking: done" << std::endl;
  return res;
}
