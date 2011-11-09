#ifndef MANDELBROT_SAMPLE_H
#define MANDELBROT_SAMPLE_H

#include "Point.h"

namespace MB {

	class Sample {
		public:

			Sample()
			{
				size = 1;
			}

			Point<int> screen;
			int size;
			Point<float> world;
			float r, g, b;
	};

}

#endif
