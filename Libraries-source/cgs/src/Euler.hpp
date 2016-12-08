#ifndef _EULER_HPP

#define _EULER_HPP
#include "mat4.hpp"

namespace cgs {
	struct Euler {
		Euler() = default;
		Euler(const float heading, const float pitch, const float bank) : m_heading(heading), m_pitch(pitch), m_bank(bank)  {}
		
		Euler & add(const float delta_heading, const float delta_pitch, const float delta_bank);
		cgm::mat4    get_rotation_mat4() const;
		
		float m_heading  = 0.0f;
		float m_pitch    = 0.0f;
		float m_bank     = 0.0f;
	};
}

#endif // !_EULER_HPP
