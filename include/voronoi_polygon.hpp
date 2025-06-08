// ���� �������� ���������� ��������� ���������� �������������� ��������

#ifndef VORONOI_POLYGON_HPP
#define VORONOI_POLYGON_HPP

#include <iostream>
#include <vector>
#include "../include/edge.hpp"	
#include "../include/point.hpp"
#include "../include/polygon.hpp"
#include "../include/half_plane.hpp"


// ������ ������������� �������� ��� ����� centerPoint ������������ ����� points
// ���� ��������� ���������� �������������� centerPoint, points[i] � ����� ����������� ��������������
namespace geometry {
	geometry::Polygon<double, std::vector<Point<double>>> VoronoiPolygon(Point<double> centerPoint, std::vector <Point<double>> points) {
		int size = points.size();
		std::vector <HalfPlane> halfPlanes;
		HalfPlane currentHalfPlane;
		Point<double> currentNormal;
		Edge<double> edge;

		// ������� ������ �� ������ ��������������

		for (int i = 0; i < size; i++) {
			edge = Edge<double>(centerPoint, points[i]);
			edge = edge.Rotate();	// ������ ��� ���������� ������������� � ������� centerPoint, points[i]


			Point<double> A = edge.Origin(), B = edge.Destination();

			// ������ ������� ������� � ����� centerPoint
			currentNormal = centerPoint - points[i];

			currentHalfPlane = HalfPlane(edge, currentNormal);

			// currentHalfPlane.PrintEquation();	// ������� ��� github

			halfPlanes.push_back(currentHalfPlane);
		}


		return HalfPlaneIntersection(halfPlanes);
	}
}

#endif // VORONOI_POLYGON_HPP