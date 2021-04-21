#include "Sphere.h"
#include <map>
#include <glm/geometric.hpp>
#include <assert.h>
#include "GLFW/glfw3.h"
#include <set>
#include <chrono>
#include <numeric>

void glDrawVec3(glm::vec3 x) {
	glVertex3f(x.x, x.y, x.z);
}

template<typename T>
std::tuple<T, T> buildOrderedTuple(T a, T b) {
	return a < b ? std::make_tuple(a, b) : std::make_tuple(b, a);
}

void Sphere::SubdivisionTriangle::draw() const {
	glBegin(GL_TRIANGLES);
	for (const VertexId vertexId : vertices) {
		glDrawVec3(parent->vertices[vertexId]);
	}
	glEnd();
}

glm::vec3 Sphere::SubdivisionTriangle::centroid() const {
	glm::vec3 center{ 0,0,0 };
	for (VertexId vert : vertices) {
		center += parent->vertices[vert];
	}
	return center / (float)vertices.size();
}

template<typename TriangleIter>
void fixParentPointers(Sphere* newParent, TriangleIter begin, TriangleIter end) noexcept {
	for (TriangleIter it = begin; it != end; it = std::next(it)) {
		it->parent = newParent;
	}
}

void Sphere::drawShape() const {
	for (const SubdivisionTriangle& triangle : triangles) {
		triangle.draw();
	}
}

Sphere::VertexId Sphere::addVertex(glm::vec3 vertex) {
	vertices.push_back(vertex);
	return vertices.size() - 1;
}

Sphere::SubdivisionTriangle& Sphere::addTriangle(std::array < VertexId, 3> vertices) {
	TriangleId id = triangles.size();
	triangles.push_back(SubdivisionTriangle{ this, vertices, id });
	return triangles.back();
}

glm::vec3 Sphere::objectCenter() const {
	return std::accumulate(vertices.begin(), vertices.end(), glm::vec3{ 0, 0, 0 }) / (float)vertices.size();
}

void Sphere::subdivide() {
	std::map<std::tuple<VertexId, VertexId>, VertexId> midpoints;
	std::vector<std::array<VertexId, 3>> newTriangles;
	glm::vec3 centerOfMass = objectCenter();
	for (const SubdivisionTriangle& triangle : triangles) {
		size_t colourIdx = 0;
		std::vector<VertexId> midVertices;

		//Generate new vertices (if needed)
		for (unsigned i = 0; i < triangle.vertices.size(); i++) {
			unsigned j = i + 1;
			j %= triangle.vertices.size();

			const VertexId iId = triangle.vertices[i];
			const VertexId jId = triangle.vertices[j];

			auto midpointIter = midpoints.find(buildOrderedTuple(iId, jId));
			if (midpointIter == midpoints.end()) {
				const glm::vec3 iVect = vertices[iId];
				const glm::vec3 jVect = vertices[jId];

				const float currDist = (glm::distance(iVect, centerOfMass) + glm::distance(jVect, centerOfMass)) / 2;

				const glm::vec3 mid = (iVect + jVect) / 2.0f;
				const glm::vec3 vec = glm::normalize(mid - centerOfMass) * currDist;

				const VertexId newId = addVertex(centerOfMass + vec);
				midpoints.emplace(std::make_pair(buildOrderedTuple(iId, jId), newId));
				midVertices.push_back(newId);
			}
			else {
				midVertices.push_back(midpointIter->second);
			}
		}

		assert(midVertices.size() == 3);

		//Generate new triangles
		for (unsigned i = 0; i < triangle.vertices.size(); i++) {
			unsigned j = (i + 2) % 3;
			const VertexId vert1 = triangle.vertices[i];
			const VertexId vert2 = midVertices[i];
			const VertexId vert3 = midVertices[j];

			newTriangles.push_back({ vert1, vert2, vert3 });
		}

		newTriangles.push_back({ midVertices[0], midVertices[1], midVertices[2] });
	}

	triangles.clear();
	triangles.reserve(newTriangles.size());
	for (const auto& newTriangle : newTriangles) {
		addTriangle(newTriangle);
	}
}