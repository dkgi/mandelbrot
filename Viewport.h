#ifndef MANDELBROT_VIEWPORT_H
#define MANDELBROT_VIEWPORT_H

#include "Point.h"
#include "Sample.h"
#include "Buffer.h"

namespace MB {

	typedef enum {NONE, UP, RIGHT, DOWN, LEFT, IN, OUT} direction;

	class Viewport {
		public:

			/**
			  * Create a new viewport.
			  * 
			  * @param width The width
			  * @param heigth The height
			  */
			Viewport()
			{
				m_center.x = 0.0;
				m_center.y = 0.0;
				m_scale = 1.0;
				m_dx = 0.2;
				m_dz = 1.2;
			}


			/**
			  * Delete the viewport.
			  */
			~Viewport() {}


			/**
			  * Move the viewport.
			  *
			  * @param dt The timestep of the animation
			  * @param d The direction to move
			  */
			void move(float dt, direction d)
			{
				float ds = dt * m_dx * m_scale;
				switch(d) {
					case UP:
						m_center.y += ds; break;
					case DOWN:
						m_center.y -= ds; break;
					case LEFT:
						m_center.x -= ds; break;
					case RIGHT:
						m_center.x += ds; break;
					default:
						break;
				}
			}


			/**
			  * Zoom in or out.
			  * 
			  * @param dt The animation timestep
			  * @param d The direction of the zoom
			  */
			void zoom(float dt, direction d)
			{
				switch(d) {
					case IN:
						m_scale = m_scale - m_scale * m_dz * dt;
						break;
					case OUT:
						m_scale = m_scale + m_scale * m_dz * dt;
						break;
					default:
						break;
				}
			}


			/**
			  * Translates a sample into the world space.
			  *
			  * @param sample The sample to translate
			  * @param buffer The buffer to project to
			  */
			void project(Sample &sample, const Buffer &buffer)
			{
				float sx = sample.screen.x + sample.size / 2.0; 	// Sample center
				float sy = sample.screen.y + sample.size / 2.0;

				float dp = (float)m_scale / buffer.width();			// Pixel size in world

				// Project
				sample.world.x = m_center.x + -buffer.width() / 2.0 * dp + sx * dp;
				sample.world.y = m_center.y + -buffer.height() / 2.0 * dp + sy * dp;
			}


		private:
			Point<float> m_center;	// Middle of viewport
			float m_scale;			// Width of viewport
			float m_dx;				// Fraction of viewport that is moved
			float m_dz;				// Fraction of scale moved per step

			friend std::ostream &operator<<(std::ostream &out, const Viewport &v);
	};

	std::ostream& operator<<(std::ostream &out, const Viewport &v)
	{
		return out << "Viewport(" << v.m_center.x << "," << v.m_center.y << "," << v.m_scale << ")";
	}

}

#endif
