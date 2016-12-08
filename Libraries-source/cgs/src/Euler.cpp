#include "Euler.hpp"
#include "vec3.hpp"
#include "mat4.hpp"
#include "matrix_transform.hpp"

cgs::Euler & cgs::Euler::add(const float delta_heading, const float delta_pitch, const float delta_bank) 
{
	m_heading +=  delta_heading;
	m_pitch   +=  delta_pitch;
	m_bank    +=  delta_bank;

	return *this;
}

cgm::mat4 cgs::Euler::get_rotation_mat4() const
{
	cgm::mat4 H, P, B;

	H = cgm::rotate(cgm::vec3(0.0f, 1.0f, 0.0f), m_heading);
	P = cgm::rotate(cgm::vec3(1.0f, 0.0f, 0.0f), m_pitch);
	B = cgm::rotate(cgm::vec3(0.0f, 0.0f, 1.0f), m_bank);

	return cgm::concat_mat4(cgm::concat_mat4(B, P), H);
}