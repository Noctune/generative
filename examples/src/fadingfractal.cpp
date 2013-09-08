#include <array>
#include <random>
#include <cmath>
#include <glm/glm.hpp>
#include <coherent/coherent.hpp>
#include <boost/program_options.hpp>

#include "fadingfractal.hpp"

std::string coherent::examples::FadingFractal::description()
{
	return "Creates fractal noise";
}

void coherent::examples::FadingFractal::options(boost::program_options::options_description& description)
{
	namespace po = boost::program_options;
	description.add_options()
		("seed,s",         po::value<int>(&seed)         ->default_value(42))
		("frequency,f",    po::value<float>(&frequency)  ->default_value(5.0f))
		("lacunarity,l",   po::value<float>(&lacunarity) ->default_value(2.0f))
		("persistence,p",  po::value<float>(&persistence)->default_value(0.5f));
}

void coherent::examples::FadingFractal::draw(TGAImage& image)
{
	using namespace std::placeholders;
	std::minstd_rand prng(seed);
	coherent::Permutation perm(prng);
	std::array<glm::vec2, 8> offsets;

	coherent::offsets(offsets.begin(), offsets.end(), prng, 1000.0f);

	auto simplex = std::bind(coherent::Simplex(), std::cref(perm), _1);

	auto begin = coherent::make_offset_iterator(offsets.begin(), simplex),
	     end   = coherent::make_offset_iterator(offsets.end(), simplex);
	

	for (int y = 0; y < image.height(); y++)
		for (int x = 0; x < image.width(); x++)
		{
			float rx = (float)x / image.width(), ry = (float)y / image.height();
			float value = coherent::fractal(begin,
					                        end,
					                        lacunarity,
					                        persistence * ry,
					                        glm::vec2(rx,ry) * frequency) * 0xFF;
			
			image.set(x,y, value, value, value);
		}
}

#ifndef COHERENT_EXAMPLES_NO_MAIN
int main(int argc, char** argv)
{
	return coherent::examples::FadingFractal().execute(argc, argv);
}
#endif
