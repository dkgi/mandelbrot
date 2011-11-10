#ifndef MANDELBROT_SCENE_H
#define MANDELBROT_SCENE_H

#include "Sample.h"
#include <cmath>
#include <iostream>
#include <complex>

namespace MB {

	class Scene {
		public:

			/**
			  * Colors the sample.
			  *
			  * @param sample The sample
			  */
			virtual void color(Sample &sample)
			{
				sample.r = (int)(std::abs(sample.world.x) * 100) % 10 / 10.0;
				sample.g = (int)(std::abs(sample.world.y) * 100) % 10 / 10.0;
			}
	};

	class Mandelbrot : public Scene {
		public:
			Mandelbrot()
			{
				m_maxIterations = 400;
				m_maxAbs = 2.0;
			}


			virtual void color(Sample &sample)
			{
				std::complex<float> c(sample.world.x, sample.world.y);
				std::complex<float> zn;
				std::complex<float> zo;
				int iterations = 0;

				while (iterations++ <= m_maxIterations) {
					zn = zo * zo + c;
					if (std::abs(zn) > m_maxAbs || zn == zo) break;
					zo = zn;
				}

				float v = (std::abs(zn) <= m_maxAbs) ? 1.0 : (float)iterations / m_maxIterations;
				sample.r = sample.g = sample.b = 1.0 - v;
			}


			int m_maxIterations;
			float m_maxAbs;
	};


}

#endif
