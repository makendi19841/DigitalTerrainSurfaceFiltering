#include "geom\Point3.h"
#include "io\TXTReader.h"
#include "io\TXTWriter.h"
#include <iostream>
#include <vector>


Point3 MinXYZ(const std::vector<Point3> &points) {

	Point3 result(std::numeric_limits<double>::max(),
		std::numeric_limits<double>::max(),
		std::numeric_limits<double>::max());
	for (const Point3 &p : points) {
		if (p.x < result.x)
			result.x = p.x;
		if (p.y < result.y)
			result.y = p.y;
		if (p.z < result.z)
			result.z = p.z;
	}

	return result;
}

Point3 MaxXYZ(const std::vector<Point3> &points) {

	Point3 result(-std::numeric_limits<double>::max(),
		-std::numeric_limits<double>::max(),
		-std::numeric_limits<double>::max());

	for (const Point3 &p : points) {
		if (p.x > result.x)
			result.x = p.x;
		if (p.y > result.y)
			result.y = p.y;
		if (p.z > result.z)
			result.z = p.z;
	}

	return result;
}


std::vector<std::vector<std::vector<Point3>>> AssignPointsToGrid(std::vector<Point3> points, Point3 extent, double cell_size) {

	std::vector<std::vector<std::vector<Point3>>> result;

	// define cell size based extent and cell size info
	int cell_num_x = (int)(extent.x / cell_size) + 1;
	int cell_num_y = (int)(extent.y / cell_size) + 1;


	// create grid
	for (unsigned int i = 0; i < cell_num_x; i++) {
		result.push_back(std::vector<std::vector<Point3>>());

		for (unsigned int j = 0; j < cell_num_y; j++) {
			result[i].push_back(std::vector<Point3>());
		}
	}

	// feed grid with points
	for (const Point3 p : points) {
		result[(int)(p.x / cell_size)][(int)(p.y / cell_size)].push_back(p);
	}

	return result;
}


std::vector<std::vector<Point3>> ConvertGridToInterpolatedPoints(std::vector<std::vector<std::vector<Point3>>> &grid) {

	std::vector<std::vector<Point3>> result;

	for (unsigned int i = 0; i < grid.size(); i++) {

		result.push_back(std::vector<Point3>(grid[i].size()));
		
		for (unsigned int j = 0; j < grid[i].size(); j++) {

			if (grid[i][j].size() > 0) {

				Point3 averagePoint = Point3(0, 0, 0);

				for (const Point3 &p: grid[i][j])
				{
					averagePoint += p;
				}

				averagePoint /= grid[i][j].size();
				result[i][j] = averagePoint;

			}
			
			
		}
		
	}
	return result;
}


double WeightCalculator(double z_initial, double z_reference) {

	// assign weight to point by comparing either initial height from point cloud 
	// to the reference height obtained from grid
	
	double weight, z_new;
	

	if (z_initial <= z_reference) {  // if initial height is below the reference one

		weight = 1.0;
	}

	else if (z_initial > z_reference + 1) {    // if initial height is above the reference one

	    weight = 0.0;

	}

	else {    // if the gap between the initial and reference height is very small

		weight = 1 / (1 + pow(z_initial - z_reference, 4));
	}

	return z_new = z_initial * weight + z_reference * (1 - weight);
	
}


std::vector<Point3> SurfaceBasedFiltering(std::vector<Point3> &points, std::vector<std::vector<Point3>> reference_grid, double cell_size) {
	
	// ConvertGridToInterpolatedPoints = reference_grid
	std::vector<Point3> result;

	for (Point3 &p : points) {

		// assign weight and adjust height value of Z 
		result.push_back(Point3(p.x, p.y, WeightCalculator(p.z, reference_grid[(int)(p.x / cell_size)][(int)(p.y / cell_size)].z)));
	}

	return result;
}
 
int main()
{
	// all points belong to the DSM
	std::vector<Point3> points = TXTReader::ReadPointXYZ("data\\input\\data.xyz");

	const Point3 OFFSET = MinXYZ(points);
	const Point3 EXTENT = MaxXYZ(points) - OFFSET;

	for (Point3 &p : points) {
		p.x -= OFFSET.x;
		p.y -= OFFSET.y;
	}


	/*double cell_size = 2.0;
	std::vector<std::vector<std::vector<Point3>>> grid = AssignPointsToGrid(points, EXTENT, cell_size);
	std::vector<std::vector<Point3>> result = ConvertGridToInterpolatedPoints(grid);
	
	for (std::vector<Point3> &pts : result) {

		for (Point3 &p : points) {
			p.x += OFFSET.x;
			p.y += OFFSET.y;
		}
	}*/

	double initial_cell_size = 49.5;

	for (int i = 0; i < 50; i++)
	{
		std::vector<std::vector<std::vector<Point3>>> grid = AssignPointsToGrid(points, EXTENT, initial_cell_size);
		std::vector<std::vector<Point3>> reference_grid_points = ConvertGridToInterpolatedPoints(grid);
		points = SurfaceBasedFiltering(points, reference_grid_points, initial_cell_size);

		initial_cell_size -= 1.0;

	}
   
		for (Point3 &p : points) {
			p.x += OFFSET.x;
			p.y += OFFSET.y;
		}
	

	TXTWriter::WritePointXYZ("data\\output\\result.xyz", points);

	std::system("PAUSE");
	return 0;
}
