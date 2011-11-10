#ifndef MANDELBROT_SAMPLER_H
#define MANDELBROT_SAMPLER_H

#include "Point.h"
#include "Sample.h"
#include <vector>

namespace MB {

	class Sampler {

		public:
			/**
			  * Create a new sampler.
			  *
			  * @param width The width
			  * @param height The height
			  * @param sampleSize The level of detail
			  */
			Sampler(int width, int height, int sampleSize)
			{
				m_width = width / sampleSize; // TODO one-off error
				m_height = height / sampleSize;
				m_sampleSize = sampleSize;

				m_samples.resize(m_width * m_height);

				for (int i = 0; i < m_width; i++) {
					for (int j = 0; j < m_height; j++) {
						m_samples[j*m_width+i].screen.x = i*m_sampleSize;
						m_samples[j*m_width+i].screen.y = j*m_sampleSize;
						m_samples[j*m_width+i].size = m_sampleSize;
					}
				}
			}

			
			/**
			  * Delete the sampler.
			  */
			~Sampler() {}


			/**
			  * Returns an iterator pointing to the first sample
			  * 
			  * @return The iterator
			  */
			std::vector<Sample>::iterator begin()
			{
				return m_samples.begin();
			}


			/**
			  * Generate an iterator pointing to after the samples.
			  * 
			  * @return The samples
			  */
			std::vector<Sample>::iterator end()
			{
				return m_samples.end();
			}

		private:
			int m_width;
			int m_height;
			int m_sampleSize;

			std::vector<Sample> m_samples;

	};

}

#endif
