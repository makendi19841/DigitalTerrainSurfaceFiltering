#pragma once

#include "..\geom\Point3.h"

#include <fstream>
#include <iomanip>
#include <vector>

class TXTWriter
{

public:
	static void WritePointXYZ(std::string path, const std::vector<Point3> &points)
	{
		if (points.size() == 0)
			return;

		std::ofstream file;
		file.open(path, std::ios::out);

		file << std::fixed << std::showpoint;
		file << std::setprecision(6);

		for (const Point3 &p : points)
		{
			file << p << std::endl;
		}

		file.close();
	}

	static void WritePointXYZ(std::string path, const std::vector<std::vector<Point3>> &points)
	{
		if (points.size() == 0)
			return;

		std::ofstream file;
		file.open(path, std::ios::out);

		file << std::fixed << std::showpoint;
		file << std::setprecision(6);

		for (const std::vector<Point3> &pts : points)
		{
			for (const Point3 &p : pts)
			{
				file << p << std::endl;
			}
		}

		file.close();
	}

};