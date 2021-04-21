#ifndef OBJECT_SPHERE_H
#define OBJECT_SPHERE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <array>

#include "Editable.h"

class Sphere : public Editable {
	using VertexId = size_t;
	using TriangleId = size_t;

	struct SubdivisionTriangle {
		Sphere* parent;
		std::array<VertexId, 3> vertices;
		TriangleId id;

		SubdivisionTriangle(Sphere* parent, std::array < VertexId, 3> vertices, TriangleId id) :
			parent{ parent }, vertices{ vertices }, id{ id } {}
		void draw() const;
		glm::vec3 centroid() const;
	};

	std::vector<glm::vec3> vertices;
	std::vector<SubdivisionTriangle> triangles;
public:
	Sphere(glm::vec3 p, glm::vec3 c) : Editable(p, c), vertices{}, triangles{} {
		this->addVertex({ 0, 0, 1 });
		this->addVertex({ 1, 0, 0 });
		this->addVertex({ 0, 1, 0 });
		this->addVertex({ 0, -1, 0 });
		this->addVertex({ -1, 0, 0 });
		this->addVertex({ 0, 0, -1 });

		this->addTriangle({ 0, 1, 2 });
		this->addTriangle({ 3, 1, 0 });
		this->addTriangle({ 4, 0, 2 });
		this->addTriangle({ 3, 0, 4 });

		this->addTriangle({ 5, 1, 3 });
		this->addTriangle({ 2, 1, 5 });
		this->addTriangle({ 3, 4, 5 });
		this->addTriangle({ 5, 4, 2 });

		this->subdivide();
		this->subdivide();
		this->subdivide();
	}

	VertexId addVertex(glm::vec3 vertex);
	SubdivisionTriangle& addTriangle(std::array <VertexId, 3> vertices);

	glm::vec3 objectCenter() const;

	void subdivide();
protected:
	void drawShape() const override;
};

#endif