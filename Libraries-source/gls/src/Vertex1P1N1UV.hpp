#ifndef _VERTEX1P1N1UV_HPP

#define _VERTEX1P1N1UV_HPP
#include "vec3.hpp"
#include "vec2.hpp"

namespace gls {
	struct Vertex1P1N1UV {
		Vertex1P1N1UV() = default;
		Vertex1P1N1UV(const cgm::vec3 & position, const cgm::vec3 & normal , const cgm::vec2 & texture_coord)
		{
			m_p = position;
			m_n = normal;
			m_uv = texture_coord;
		}
		cgm::vec3 m_p; //vertex position
		cgm::vec3 m_n; // vertex normal
		cgm::vec2 m_uv; // (u, v) texture coordinate
	};
}

#endif // !_VERTEX1P1N1UV_HPP
