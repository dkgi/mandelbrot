#ifndef MANDELBROT_BUFFER_H
#define MANDELBROT_BUFFER_H

#include "Point.h"
#include "Sample.h"
#include <cstdlib>

namespace MB
{

	class Buffer {
		public:

			/**
			  * Create a new empty buffer.
			  *
			  * @param width The width
			  * @param height The height
			  */
			Buffer(int width, int height)
			{
				m_width = width;
				m_height = height;
				m_data = (float*) malloc(sizeof(float) * width * height * 3);
			}
			

			/**
			  * Delete the buffer
			  */
			~Buffer()
			{
				free(m_data);
			}


			/**
			  * Put rgb values at coordinates.
			  * 
			  * @param i, j The buffer coordinates
			  * @param r, g, b The color values
			  */
			inline void put(int i, int j, float r, float g, float b)
			{
				m_data[3 * (j * m_width + i) + 0] = r;
				m_data[3 * (j * m_width + i) + 1] = g;
				m_data[3 * (j * m_width + i) + 2] = b;
			}

		
			/**
			  * Put a sample into the buffer.
			  * 
			  * @param sample The sample
			  */
			inline void put(const Sample &sample)
			{
				for (int i = 0; i < sample.size; i++) {
					for (int j = 0; j < sample.size; j++) {
						put(sample.screen.x + i, sample.screen.y + j, sample.r, sample.g, sample.b);
					}
				}
			}


			/**
			  * The width of the buffer.
			  *
			  * @return The width
			  */
			const int &width() const
			{
				return m_width;
			}


			/**
			  * The height of the buffer.
			  *
			  * @return The height
			  */
			const int &height() const
			{
				return m_height;
			}


			/**
			  * The underlying buffer structure.
			  *
			  * @return The buffer
			  */
			const float *data()
			{
				return m_data;
			}


		private:
			int m_width;
			int m_height;
			float *m_data;
	};

}

#endif 
