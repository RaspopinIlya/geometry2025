// ���� �������� �������, ������� ��������� ������� VoronoiPolygon. ��������� � ���������� ������ � ������� json


#include "../include/json.hpp"
#include "../include/point.hpp"
#include "../include/polygon.hpp"
#include "../include/voronoi_polygon.hpp"
#include <vector>

namespace geometry {
	int VoronoiPolygonMethod(const nlohmann::json& input, nlohmann::json* output) {
		Point<double> point;
		std::vector < Point<double>> points;
		geometry::Polygon<double, std::vector<Point<double>> > outputPolygon;


		// ��������� point �� json
		point.X() = input.at("point")[0];
		point.Y() = input.at("point")[1];

		// ��������� ������ points �� json
		for (auto& currentJsonPoint : input.at("points"))
			points.push_back(Point<double>(currentJsonPoint[0], currentJsonPoint[1]));

		outputPolygon = VoronoiPolygon(point, points);

		// ��������� �������� ������ � ������� json
		
		int size = outputPolygon.Size();
		std::vector<Point<double>> outputVerticies = outputPolygon.Vertices();

		for (int i = 0; i < size; i++) {
			(*output)["points"][i][0] = outputVerticies[i].X();
			(*output)["points"][i][1] = outputVerticies[i].Y();
		}


		return 0;	// �������, ��� ������ ��� ����� �� ���������. ���� ������-��, ������� ������ ����� ���� ���������, � � ��� ��� ��������.
	}
} // namespace geometry

/*
��������� ������:
�����, ������������ ������� ������ ������������� ��������
����� ������, ������������ ������� ������ ������������� ��������

input json:
{
	"point": [200, 200],
	"points": [
		[100, 200],
		[200, 300],
		[300, 200],
		[200, 100]
	]
}

���������� ������: ������� �������������� ��������.
output json:
{
	"points": [
		[150, 250],
		[150, 150],
		[250, 150],
		[250, 250]
	]	
}

*/
