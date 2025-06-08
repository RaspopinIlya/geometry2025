// ����������� ������ ������������� � ������� ����������� ��������������

#ifndef INCLUDE_HALF_PLANE_HPP_
#define INCLUDE_HALF_PLANE_HPP_

#include "../include/edge.hpp"	// ������ ������ �������� ������ ���������, �� � ���� ����� ������������ ���� � ����������� edge_impl.hpp
#include "../include/point.hpp"
#include "../include/polygon.hpp"
#include <iostream>
#include <vector>

namespace geometry {
	double precision = 1e-7;

	class HalfPlane {
	private:
		double a, b, c;

		Edge<double> boundary;// ��� ������ ax+by+c=0 - ������� �������������. ������������� ����� ��������� ax+by+c>=0

	public:
		// ��-��������� ������������� ������ ��� x >= 0, �.�. boundary �������� ����� (0, 0), (1, 0), � ������ ������� (1, 0)
		HalfPlane():	
			HalfPlane(Edge<double> (Point<double> (0, 0), Point<double> (0, 1)), Point<double> (1, 0))
		{}

		Edge<double> Boundary() const { return boundary; }

		// ������ ������������� �� ������ � ������ �������
		HalfPlane(Edge<double> _boundary, Point<double> normal) :
			boundary(_boundary)
		{
			double x1, y1, x2, y2, det;

			x1 = boundary.Origin().X();
			y1 = boundary.Origin().Y();
			x2 = boundary.Destination().X();
			y2 = boundary.Destination().Y();

			// ������� a, b, c ��� ������ boundary
			// ������� det = 0 �������� ���������� Origin � Destination � boundary
			// � ��� ������������� �� �������� �������������� ������ boundary, �.�. ����� ����� det != 0
			det = x1 * y2 - x2 * y1; 
			
			if (std::fabs(x2 - x1) < precision) {	// ����� x1=x2 � ������� ��� ������������ ������ x = x1
				a = 1;
				b = 0;
				c = -x1;
			}
			else if (std::fabs(y2 - y1) < precision) {	// ����� y1=y2  � ������� ��� �������������� ������ y=y1
				a = 0;
				b = 1;
				c = -y1;
			}
			else { // ������ x1 != x2, y1 != y2 � ����� �������� ��������� ������ x-x1 / x2-x1 = y-y1 / y2-y1
				a = y2 - y1;
				b = -(x2 - x1);
				c = -det;
			}
			
			// ��������� ��������� ������������� ax+by+c>=0, ������ ���� �� ���� ��������� ��� �������������, ���� �������
			Point<double> point;
			point = boundary.Origin() + normal;
			
			if (!ContainPoint(point)) { // ������ �� �������� ���������� ax+by+c>=0, � ���� ������� -ax-by-c>=0
				a *= (-1);
				b *= (-1);
				c *= (-1);
			}
		}

		bool ContainPoint(Point<double> point) { // ����������: �������� �� ������������� �����?
			if (a * point.X() + b * point.Y() + c >= 0)
				return true;
			else
				return false;
		}

		/*void PrintEquation() {	// ������ ��� github
			std::cout << "equation for half plane: ";
			std::cout << a << "x + " << b << "y + " << c << " >= 0" << std::endl;
		}*/
	};



	/*void DrawPolygonOnScreen(std::vector<Point<double>> verticies) {	// ������ ��� github
		int size = verticies.size();
			int* tmpArray = new int[2 * (size + 1)];

			for (int i = 0; i < size + 1; i++) {
				if (i != size) {
					tmpArray[2 * i] = verticies[i].X();
					tmpArray[2 * i + 1] = verticies[i].Y();
				}
				else if (i == size) {
					tmpArray[2 * i] = verticies[0].X();
					tmpArray[2 * i + 1] = verticies[0].Y();
				}
			}

			initwindow(500, 500);
			fillpoly(size + 1, tmpArray);
			getch();
			closegraph();

			delete[] tmpArray;
			return;
	}*/

	Polygon<double, std::vector<Point<double>>> HalfPlaneIntersection(std::vector <HalfPlane> halfPlanes) {
		double boundarySize = 500;	// ��� ���� ��������� ��� ������� �� �������� 500
		int size;	// ������ ������� ans
		Point<double> prev, next;	// prev, next - ��� ������� ����� 
		Point<double> mid;			// mid - ����������� ����� � �������� ������������� (���� ���� �����������)

		// ��� ������� ����������� �������������� - �������������, ���������� � ���� ������� �����
		// �� ����� ���������� ������� � ���������������, ������� �������� �� ��������
		std::vector <Point<double>> currentAnswerPolygon = { 
			Point<double>(0, 0), 
			Point<double>(boundarySize, 0), 
			Point<double>(boundarySize, boundarySize), 
			Point<double>(0, boundarySize) 
		};

		std::vector <Point<double>> newAnswerPolygon;	// ��������� ����������� �������������� ans � ����� ��������������
		HalfPlane currentHalfPlane;

		for (HalfPlane currentHalfPlane : halfPlanes) {	// ���������� ������������� ans � currentHalfPlane

			// ��� ���������� ������ ������ - ����������� �������, ��-�� ���� ��������� ����� ������� ����� � ������� �� ����
			// ������������ ������� ���������� ��-�� ����, ��� ������� ��������� �������� ��������� ����� ������� ��������������

			newAnswerPolygon.resize(0);
			int size = currentAnswerPolygon.size();

			for (int i = 0; i < size; i++) {	// ���������� ������� ������������� � ������ (prev, next)
				if (i != size - 1) {
					prev = currentAnswerPolygon[i];
					next = currentAnswerPolygon[i + 1];
				}
				else {
					prev = currentAnswerPolygon[size - 1];
					next = currentAnswerPolygon[0];
				}

				// ���������� ��� ������: Edge(prev, next) � ������� currentHalfPlane
				double t; // � ��� ���������� ��������� ���������� ����� �� �����, ��������������� �����������
				Intersection intersectionType;	// ���� ��������� ��� ����������� - �����������?
				// parallel - �����������. collinear - ���� � �� �� ������ - ��� �������� ������� ��� ����� �������
				intersectionType = Edge<double>(prev, next).Intersect(currentHalfPlane.Boundary(), &t, precision);
				mid = prev + t * (next - prev);

				if (intersectionType != Intersection::Parallel && intersectionType != Intersection::Collinear) {
					if (currentHalfPlane.ContainPoint(prev) && currentHalfPlane.ContainPoint(next))
						newAnswerPolygon.push_back(next);	// ������������� �������� ������� ����� (prev, next). ����� mid �� �����
					else if (!currentHalfPlane.ContainPoint(prev) && !currentHalfPlane.ContainPoint(next)) {
					}	// ������������� ������� �� �������� ����� (prev, next). �� ������ ������
						// � ��������� ������� ������������� ����� ������������ � ������ ����� �� �����
					else if (currentHalfPlane.ContainPoint(prev) && !currentHalfPlane.ContainPoint(next))
						newAnswerPolygon.push_back(mid);
					else if (!currentHalfPlane.ContainPoint(prev) && currentHalfPlane.ContainPoint(next)) {
						newAnswerPolygon.push_back(mid);
						newAnswerPolygon.push_back(next);
					}
				}
				else if (intersectionType == Intersection::Parallel || intersectionType == Intersection::Collinear) {
					if (currentHalfPlane.ContainPoint(next))
						newAnswerPolygon.push_back(next);
				}


			}


			currentAnswerPolygon = newAnswerPolygon;

			// DrawPolygonOnScreen(currentAnswerPolygon);	// ������ ��� github

			newAnswerPolygon.resize(0);
		}
		return Polygon<double, std::vector<Point<double>> >(currentAnswerPolygon);

	}


} // namespace geometry

#endif // INCLUDE_HALF_PLANE_HPP_
