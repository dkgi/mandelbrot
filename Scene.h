#ifndef MANDELBROT_SCENE_H
#define MANDELBROT_SCENE_H

#include "Sample.h"
#include <cmath>
#include <iostream>

namespace MB {

	class Scene {
		public:

			/**
			  * Colors the sample.
			  *
			  * @param sample The sample
			  */
			void color(Sample &sample)
			{
				sample.r = (int)(std::abs(sample.world.x) * 100) % 10 / 10.0;
				sample.g = (int)(std::abs(sample.world.y) * 100) % 10 / 10.0;
			}
	};

}

#endif
